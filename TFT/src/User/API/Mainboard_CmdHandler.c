#include "Mainboard_CmdHandler.h"
#include "includes.h"
#include "RRFStatusControl.h"

#define CMD_QUEUE_SIZE  20
#define CMD_RETRY_COUNT 3

typedef struct
{
  CMD gcode;
  SERIAL_PORT_INDEX port_index;  // 0: for SERIAL_PORT, 1: for SERIAL_PORT_2 etc.
} GCODE_INFO;

typedef struct
{
  GCODE_INFO queue[CMD_QUEUE_SIZE];
  uint8_t index_r;  // ring buffer read position
  uint8_t index_w;  // ring buffer write position
  uint8_t count;    // count of commands in the queue
} GCODE_QUEUE;

typedef struct
{
  uint32_t line_number;    // line number used as matching key with value reported by mainboard on "Resend: " ACK message
  bool retry;              // flag set to "true" to trigger a command resend. Initially set to "false" (no need to resend)
  int8_t retry_attempts;   // remaining retry attempts. Initially set to default max value CMD_RETRY_COUNT
  GCODE_INFO gcode_info;   // command to resend
} GCODE_RETRY_INFO;

typedef enum
{
  NO_WRITING = 0,
  TFT_WRITING,
  ONBOARD_WRITING
} WRITING_MODE;

static GCODE_QUEUE cmdQueue;                    // command queue where commands to be sent are stored
static GCODE_RETRY_INFO cmdRetryInfo = {0};     // command retry info. Required COMMAND_CHECKSUM feature enabled in TFT

static char * cmd_ptr;
static uint8_t cmd_len;
static SERIAL_PORT_INDEX cmd_port_index;        // index of serial port originating the gcode
static uint8_t cmd_base_index;                  // base index in case the gcode has checksum ("Nxx " is present at the beginning of gcode)
static uint8_t cmd_index;
static WRITING_MODE writing_mode = NO_WRITING;  // writing mode. Used by M28 and M29
#ifdef SERIAL_PORT_2
  static FIL file;                              // used with writing mode
#endif

uint8_t getQueueCount(void)
{
  return cmdQueue.count;
}

bool isPendingCmd(void)
{
  return (infoHost.tx_count != 0);
}

bool isFullCmdQueue(void)
{
  return (cmdQueue.count >= CMD_QUEUE_SIZE);
}

bool isNotEmptyCmdQueue(void)
{
  return (cmdQueue.count != 0 || infoHost.tx_slots == 0);  // if queue not empty or no available gcode tx slot
}

bool isEnqueued(const CMD cmd)
{
  for (int i = 0; i < cmdQueue.count; i++)
  {
    if (strcmp(cmd, cmdQueue.queue[(cmdQueue.index_r + i) % CMD_QUEUE_SIZE].gcode) == 0)
      return true;
  }

  return false;
}

bool isWritingMode(void)
{
  return (writing_mode != NO_WRITING);
}

// common store cmd
static void commonStoreCmd(GCODE_QUEUE * pQueue, const char * format, va_list va)
{
  vsnprintf(pQueue->queue[pQueue->index_w].gcode, CMD_MAX_SIZE, format, va);

  pQueue->queue[pQueue->index_w].port_index = PORT_1;  // port index for SERIAL_PORT
  pQueue->index_w = (pQueue->index_w + 1) % CMD_QUEUE_SIZE;
  pQueue->count++;
}

// store gcode cmd to cmdQueue queue.
// This command will be sent to the printer by sendQueueCmd().
// If the cmdQueue queue is full, a reminder message is displayed and the command is discarded
bool storeCmd(const char * format, ...)
{
  if (format[0] == 0) return false;

  if (cmdQueue.count >= CMD_QUEUE_SIZE)
  {
    setReminderMsg(LABEL_BUSY, SYS_STATUS_BUSY);
    return false;
  }

  va_list va;
  va_start(va, format);
  commonStoreCmd(&cmdQueue, format, va);
  va_end(va);

  return true;
}

// store gcode cmd to cmdQueue queue.
// This command will be sent to the printer by sendQueueCmd().
// If the cmdQueue queue is full, a reminder message is displayed
// and it will wait for the queue to be able to store the command
void mustStoreCmd(const char * format, ...)
{
  if (format[0] == 0) return;

  if (cmdQueue.count >= CMD_QUEUE_SIZE)
  {
    setReminderMsg(LABEL_BUSY, SYS_STATUS_BUSY);
    TASK_LOOP_WHILE(isFullCmdQueue());  // wait for a free slot in the queue in case the queue is currently full
  }

  va_list va;
  va_start(va, format);
  commonStoreCmd(&cmdQueue, format, va);
  va_end(va);
}

// store Script cmd to cmdQueue queue.
// For example: "M502\nM500\n" will be split into two commands "M502\n", "M500\n"
void mustStoreScript(const char * format, ...)
{
  if (format[0] == 0) return;

  char script[256];
  va_list va;
  va_start(va, format);
  vsnprintf(script, 256, format, va);
  va_end(va);

  char * p = script;
  uint16_t i = 0;
  CMD cmd;

  for (;;)
  {
    char c = *p++;
    if (!c) return;
    cmd[i++] = c;

    if (c == '\n')
    {
      cmd[i] = 0;
      mustStoreCmd("%s", cmd);
      i = 0;
    }
  }
}

// store gcode cmd received from UART (e.g. ESP3D, OctoPrint, other TouchScreen etc.) to cmdQueue queue.
// This command will be sent to the printer by sendQueueCmd().
// If the cmdQueue queue is full, a reminder message is displayed and the command is discarded
bool storeCmdFromUART(const CMD cmd, const SERIAL_PORT_INDEX portIndex)
{
  if (cmd[0] == 0) return false;

  if (cmdQueue.count >= CMD_QUEUE_SIZE)
  {
    setReminderMsg(LABEL_BUSY, SYS_STATUS_BUSY);
    return false;
  }

  strncpy_no_pad(cmdQueue.queue[cmdQueue.index_w].gcode, cmd, CMD_MAX_SIZE);

  cmdQueue.queue[cmdQueue.index_w].port_index = portIndex;
  cmdQueue.index_w = (cmdQueue.index_w + 1) % CMD_QUEUE_SIZE;
  cmdQueue.count++;

  return true;
}

// clear all gcode cmd in cmdQueue queue
void clearCmdQueue(void)
{
  cmdQueue.count = cmdQueue.index_w = cmdQueue.index_r = 0;
}

// strip out any leading space from the passed command.
// Furthermore, skip any N[-0-9] (line number) and return a pointer to the beginning of the command
static char * stripCmd(char * cmdPtr)
{
  // skip leading spaces
  while (*cmdPtr == ' ') cmdPtr++;           // e.g. "  N1   G28*18\n" -> "N1   G28*18\n"

  // skip N[-0-9] (line number) if included in the command line
  if (*cmdPtr == 'N' && NUMERIC(cmdPtr[1]))  // e.g. "N1   G28*18\n" -> "G28*18\n"
  {
    cmdPtr += 2;                             // skip N[-0-9]
    while (NUMERIC(*cmdPtr)) cmdPtr++;       // skip [0-9]*
    while (*cmdPtr == ' ') cmdPtr++;         // skip [ ]*
  }

  return cmdPtr;
}

// get the data of the next to be sent command in cmdQueue
// and return "true" if sent from TFT, otherwise "false"
static inline bool getCmd(void)
{
  cmd_ptr = &cmdQueue.queue[cmdQueue.index_r].gcode[0];          // gcode
  cmd_port_index = cmdQueue.queue[cmdQueue.index_r].port_index;  // index of serial port originating the gcode

  // strip out any leading space from the passed command.
  // Furthermore, skip any N[-0-9] (line number) and return a pointer to the beginning of the command
  //
  // set cmd_base_index with index of gcode command
  //
  cmd_base_index = stripCmd(cmd_ptr) - cmd_ptr;                 // e.g. "N1   G28*18\n" -> "G28*18\n"

  // set cmd_index with index of gcode value
  cmd_index = cmd_base_index + 1;                                // e.g. "G28*18\n" -> "28*18\n"

  cmd_len = strlen(cmd_ptr);                                     // length of gcode

  return (cmd_port_index == PORT_1);  // if gcode is originated by TFT (SERIAL_PORT), return "true"
}

static inline void getCmdFromCmdRetryInfo(void)
{
  cmd_ptr = cmdRetryInfo.gcode_info.gcode;
  cmd_port_index = cmdRetryInfo.gcode_info.port_index;
  cmd_len = strlen(cmd_ptr);
}

static inline void setCmdRetryInfo(uint32_t lineNumber)
{
  cmdRetryInfo.line_number = lineNumber;          // set line number to the provided value
  cmdRetryInfo.retry = false;                     // set retry flag to "false" (no need to resend)
  cmdRetryInfo.retry_attempts = CMD_RETRY_COUNT;  // set retry attempts to default max value CMD_RETRY_COUNT

  strncpy_no_pad(cmdRetryInfo.gcode_info.gcode, cmd_ptr, CMD_MAX_SIZE);  // copy command
  cmdRetryInfo.gcode_info.port_index = cmd_port_index;                   // copy port index
}

// purge gcode cmd or send it to the printer and then remove it from cmdQueue queue
static bool sendCmd(bool purge, bool avoidTerminal)
{
  char * purgeStr = "[Purged] ";

  // if TFT is in listening mode, purge the command
  if (infoHost.listening_mode)
    purge = true;

  #if defined(SERIAL_DEBUG_PORT) && defined(DEBUG_SERIAL_COMM)
    // dump serial data sent to debug port
    Serial_Put(SERIAL_DEBUG_PORT, serialPort[cmd_port_index].id);  // serial port ID (e.g. "2" for SERIAL_PORT_2)
    Serial_Put(SERIAL_DEBUG_PORT, ">>");
  #endif

  if (!purge)  // if command is not purged, send it to printer
  {
    // if the message under processing is from command queue and COMMAND_CHECKSUM feature is enabled,
    // apply line number and checksum and store the new gcode in the retry buffer
    if (!cmdRetryInfo.retry && GET_BIT(infoSettings.general_settings, INDEX_COMMAND_CHECKSUM) == 1)
      setCmdRetryInfo(addCmdLineNumberAndChecksum(cmd_ptr, cmd_base_index, &cmd_len));  // cmd_ptr and cmd_len are updated

    // send or resend command

    UPD_TX_KPIS(cmd_len);  // debug monitoring KPI

    Serial_Put(SERIAL_PORT, cmd_ptr);

    setCurrentAckSrc(cmd_port_index);
  }
  #if defined(SERIAL_DEBUG_PORT) && defined(DEBUG_SERIAL_COMM)
    else  // if command is purged
    {
      Serial_Put(SERIAL_DEBUG_PORT, purgeStr);
    }

    Serial_Put(SERIAL_DEBUG_PORT, cmd_ptr);
  #endif

  if (!cmdRetryInfo.retry)  // if the message under processing is from command queue, dequeue the command
  {
    cmdQueue.count--;
    cmdQueue.index_r = (cmdQueue.index_r + 1) % CMD_QUEUE_SIZE;
  }
  else  // if there is a pending command to resend
  {
    cmdRetryInfo.retry = false;     // disable command resend flag
    cmdRetryInfo.retry_attempts--;  // update remaining retry attempts
  }

  if (!avoidTerminal && MENU_IS(menuTerminal))
  {
    if (purge)
      terminalCache(purgeStr, strlen(purgeStr), cmd_port_index, SRC_TERMINAL_GCODE);

    terminalCache(cmd_ptr, cmd_len, cmd_port_index, SRC_TERMINAL_GCODE);
  }

  return !purge;  // return "true" if command was sent. Otherwise, return "false"
}

// check the presence of the specified "keyword" string in the current gcode command
// starting the search from index "index"
static bool cmd_seen_from(uint8_t index, const char * keyword)
{
  if (index >= cmd_len)
    return false;

  for (uint16_t i = 0; cmd_ptr[index] != '\0'; index++, i = 0)
  {
    while (keyword[i] == cmd_ptr[index + i])
    {
      if (keyword[++i] == '\0')
      {
        cmd_index = index + i;

        return true;
      }
    }
  }

  return false;
}

// check the presence of the specified "code" character in the current gcode command
static bool cmd_seen(const char code)
{
  cmd_index = cmd_base_index;

  while (cmd_ptr[cmd_index] != '\0')
  {
    if (cmd_ptr[cmd_index++] == code)
      return true;
  }

  return false;
}

// get the int after "code". Call after cmd_seen(code)
static int32_t cmd_value(void)
{
  return (strtol(&cmd_ptr[cmd_index], NULL, 10));
}

// get the int after "/", if any
static int32_t cmd_second_value(void)
{
  char * secondValue = strchr(&cmd_ptr[cmd_index], '/');

  if (secondValue != NULL)
    return (strtol(secondValue + 1, NULL, 10));
  else
    return -0.5;
}

// get the float after "code". Call after cmd_seen(code)
static float cmd_float(void)
{
  return (strtod(&cmd_ptr[cmd_index], NULL));
}

#ifdef SERIAL_PORT_2

static bool initRemoteTFT(void)
{
  // examples:
  //
  // "cmd_ptr" = "N1 M23 SD:/test/cap2.gcode*12\n"
  // "cmd_ptr" = "N1 M23 S /test/cap2.gcode*82\n"
  //
  // "infoFile.path" = "SD:/test/cap2.gcode"

  // e.g. "N1 M23 SD:/test/cap2.gcode*12\n" -> "SD:/test/cap2.gcode*12\n"
  //
  if (cmd_seen_from(cmd_base_index, "SD:") || cmd_seen_from(cmd_base_index, "S "))
    infoFile.source = FS_TFT_SD;   // set source first
  else if (cmd_seen_from(cmd_base_index, "U:") || cmd_seen_from(cmd_base_index, "U "))
    infoFile.source = FS_TFT_USB;  // set source first
  else
    return false;

  // present just to make the code robust. It should never be used printing from remote TFT media
  infoFile.onboardSource = BOARD_SD;

  CMD path;  // temporary working buffer (cmd_ptr buffer must always remain unchanged)

  // cmd_index was set by cmd_seen_from() function
  strcpy(path, &cmd_ptr[cmd_index]);  // e.g. "N1 M23 SD:/test/cap2.gcode*12\n" -> "/test/cap2.gcode*12\n"
  stripCmdChecksum(path);             // e.g. "/test/cap2.gcode*12\n" -> /test/cap2.gcode"

  resetInfoFile();                  // then reset infoFile (source is restored)
  enterFolder(stripCmdHead(path));  // set path as last

  return true;
}

static bool openRemoteTFT(bool writingMode)
{
  bool open = false;

  Serial_Forward(cmd_port_index, "echo:Now fresh file: ");
  Serial_Forward(cmd_port_index, infoFile.path);
  Serial_Forward(cmd_port_index, "\n");

  if (!writingMode)  // if reading mode
  {
    // mount FS and open the file (infoFile.source and infoFile.path are used)
    if (mountFS() == true && f_open(&file, infoFile.path, FA_OPEN_EXISTING | FA_READ) == FR_OK)
    {
      char buf[10];

      sprintf(buf, "%d", f_size(&file));
      f_close(&file);

      Serial_Forward(cmd_port_index, "File opened: ");
      Serial_Forward(cmd_port_index, infoFile.path);
      Serial_Forward(cmd_port_index, " Size: ");
      Serial_Forward(cmd_port_index, buf);
      Serial_Forward(cmd_port_index, "\nFile selected\n");

      open = true;
    }
  }
  else  // if writing mode
  {
    // mount FS and open the file (infoFile.source and infoFile.path are used)
    if (mountFS() == true && f_open(&file, infoFile.path, FA_OPEN_ALWAYS | FA_WRITE) == FR_OK)
    {
      Serial_Forward(cmd_port_index, "Writing to file: ");
      Serial_Forward(cmd_port_index, infoFile.path);
      Serial_Forward(cmd_port_index, "\n");

      open = true;
    }
  }

  if (!open)
  {
    Serial_Forward(cmd_port_index, "open failed, File: ");
    Serial_Forward(cmd_port_index, infoFile.path);
    Serial_Forward(cmd_port_index, "\n");
  }

  Serial_Forward(cmd_port_index, "ok\n");

  return open;
}

static inline void writeRemoteTFT(void)
{
  // examples:
  //
  // "cmd_ptr" = "N1 G28*18\n"
  // "cmd_ptr" = "N2 G29*16\n"
  // "cmd_ptr" = "N3 M29*27\n"

  // if M29, stop writing mode. cmd_index (used by cmd_value() function) was set by sendQueueCmd() function
  if (cmd_ptr[cmd_base_index] == 'M' && cmd_value() == 29)  // e.g. "N3 M29*27\n" -> "M29*27\n"
  {
    f_close(&file);

    Serial_Forward(cmd_port_index, "Done saving file.\n");

    writing_mode = NO_WRITING;
  }
  else
  {
    UINT br;
    CMD cmd;  // temporary working buffer (cmd_ptr buffer must always remain unchanged)

    strcpy(cmd, &cmd_ptr[cmd_base_index]);  // e.g. "N1 G28*18\n" -> "G28*18\n"
    stripCmdChecksum(cmd);                  // e.g. "G28*18\n" -> "G28"

    f_write(&file, cmd, strlen(cmd), &br);

    // "\n" is always removed by stripCmdChecksum() function even if there is no checksum,
    // so we need to write it on file separately
    f_write(&file, "\n", 1, &br);
    f_sync(&file);
  }

  Serial_Forward(cmd_port_index, "ok\n");
}

#endif  // SERIAL_PORT_2

static void setWaitHeating(uint8_t index)
{
  if (cmd_seen('R'))
  {
    cmd_ptr[cmd_index - 1] = 'S';
    heatSetIsWaiting(index, true);
  }
  else if (cmd_seen('S'))
  {
    heatSetIsWaiting(index, (cmd_value() > heatGetCurrentTemp(index) - TEMPERATURE_RANGE));
  }
}

static void syncTargetTemp(uint8_t index)
{
  uint16_t temp;

  if (cmd_seen('S'))
  {
    temp = cmd_value();

    if (temp != heatGetTargetTemp(index))
      heatSetTargetTemp(index, temp, FROM_CMD);
  }
}

void handleCmdLineNumberMismatch(const uint32_t lineNumber)
{
  // if no buffered command with the requested line number is found or it is found but the maximum number of retry
  // attempts is reached, reset the line number with M110 just to try to avoid further retransmission requests for
  // the same line number or for any out of synch command already sent to the mainboard (e.g. in case ADVANCED_OK
  // feature is enabled in TFT)
  if (cmdRetryInfo.line_number != lineNumber || cmdRetryInfo.retry_attempts == 0)
  {
    if (getCmdLineNumberBase() != lineNumber)  // if notification not already displayed for the same line number
    {
      char msgText[MAX_MSG_LENGTH];

      snprintf(msgText, MAX_MSG_LENGTH, "line: cur=%lu, exp=%lu", cmdRetryInfo.line_number, lineNumber);

      addNotification(DIALOG_TYPE_ERROR, "Cmd not found", msgText, false);
    }

    setCmdLineNumberBase(lineNumber);  // set base line number of next command sent by the TFT to the requested line number

    CMD cmd;

    sprintf(cmd, "M110 N%lu", lineNumber);

    sendEmergencyCmd(cmd);  // immediately send M110 command to set new base line number on mainboard
  }
  else  // if a command with the requested line number is present in the buffer and
  {     // not already resent for the maximum retry attempts, mark it as to be sent
    cmdRetryInfo.retry = true;
  }
}

// check if the received gcode is an emergency command or not
// (M108, M112, M410, M524, M876) and parse it accordingly.
// Otherwise, store the gcode on command queue
void handleCmd(CMD cmd, const SERIAL_PORT_INDEX portIndex)
{
  // strip out any leading space from the passed command.
  // Furthermore, skip any N[-0-9] (line number) and return a pointer to the beginning of the command
  //
  char * cmdPtr = stripCmd(cmd);  // e.g. "  N1   G28*18\n" -> "G28*18\n"

  // check if the received gcode is an emergency command and parse it accordingly

  if (cmdPtr[0] == 'M')
  {
    switch (strtol(&cmdPtr[1], NULL, 10))
    {
      case 108:  // M108
      case 112:  // M112
      case 410:  // M410
      case 876:  // M876
        sendEmergencyCmd(cmd, portIndex);
        return;

      case 524:  // M524
        abortPrint();

        if (portIndex != PORT_1)  // if gcode is not generated from TFT
          Serial_Forward(portIndex, "ok\n");
        return;
    }
  }

  // the received gcode is not an emergency command.
  // If not an empty gcode, we can loop on the following storeCmdFromUART() function to store the gcode on cmdQueue

  if (cmd[0] != '\0')
    TASK_LOOP_WHILE(!storeCmdFromUART(cmd, portIndex));
}

// send emergency command now.
// The command parameter must be a clear command, not formatted
//
// NOTE: Make sure that the printer can receive the command
//
void sendEmergencyCmd(const CMD emergencyCmd, const SERIAL_PORT_INDEX portIndex)
{
  #if defined(SERIAL_DEBUG_PORT) && defined(DEBUG_SERIAL_COMM)
    // dump serial data sent to debug port
    Serial_Put(SERIAL_DEBUG_PORT, serialPort[portIndex].id);  // serial port ID (e.g. "2" for SERIAL_PORT_2)
    Serial_Put(SERIAL_DEBUG_PORT, ">>");
    Serial_Put(SERIAL_DEBUG_PORT, emergencyCmd);
  #endif

  uint8_t cmdLen = strlen(emergencyCmd);

  UPD_TX_KPIS(cmdLen);  // debug monitoring KPI

  Serial_Put(SERIAL_PORT, emergencyCmd);

  setCurrentAckSrc(portIndex);

  if (MENU_IS(menuTerminal))
    terminalCache(emergencyCmd, cmdLen, portIndex, SRC_TERMINAL_GCODE);
}

// parse and send gcode cmd in cmdQueue queue
void sendQueueCmd(void)
{
  if (infoHost.tx_slots == 0 || (cmdQueue.count == 0 && !cmdRetryInfo.retry)) return;

  bool avoid_terminal = false;

  if (cmdRetryInfo.retry)  // if there is a pending command to resend
  {
    getCmdFromCmdRetryInfo();  // retrieve gcode from cmdRetryInfo

    goto send_cmd;  // send the command
  }

  bool fromTFT = getCmd();  // retrieve leading gcode in the queue and check if it is originated by TFT or other hosts

  #ifdef SERIAL_PORT_2
    if (writing_mode != NO_WRITING)  // if writing mode (previously triggered by M28)
    {
      if (fromTFT)  // ignore any command from TFT media
      {
        sendCmd(true, avoid_terminal);  // skip the command
      }
      else if (writing_mode == TFT_WRITING)  // if the command is from remote to TFT media
      {
        writeRemoteTFT();

        sendCmd(true, avoid_terminal);  // skip the command
      }
      else  // otherwise, if the command is from remote to onboard media
      {
        if (cmd_ptr[cmd_base_index] == 'M' && cmd_value() == 29)  // if M29, stop writing mode
          writing_mode = NO_WRITING;

        goto send_cmd;  // send the command
      }

      return;
    }
  #endif

  switch (cmd_ptr[cmd_base_index])
  {
    // parse M-codes
    case 'M':
      switch (cmd_value())
      {
        case 0:  // M0
        case 1:  // M1
          if (isPrinting() && infoMachineSettings.firmwareType != FW_REPRAPFW)  // abort printing by "M0" in RepRapFirmware
          {
            // pause if printing from TFT media and purge M0/M1 command
            if (infoFile.source < FS_ONBOARD_MEDIA)
            {
              // TODO: get message and display
              sendCmd(true, avoid_terminal);
              if(cmd_seen('A'))
                pausePrint(true,PAUSE_ATTACH_PROBE);
              else if(cmd_seen('D'))
                pausePrint(true,PAUSE_DETTACH_PROBE);
              else if(cmd_seen('T'))
                pausePrint(true,PAUSE_TOOL_ONOFF);
              else
              pausePrint(true, PAUSE_M0);
              return;
            }
          }
          break;
        case 3: // M3
        case 4: // M4
          if (cmd_seen('S'))
            laserSetCurSpeed(cmd_value());
          break;
        case 5: // M5
            laserSetCurSpeed(0);
          break;
        case 18:  // M18 disable steppers
        case 84:  // M84 disable steppers
          // do not mark coordinate as unknown in case of a M18/M84 S<timeout> command that
          // doesn't disable the motors right away but will set their idling timeout
          if (!(cmd_seen('S') && !cmd_seen('Y') && !cmd_seen('Z') && !cmd_seen('E')))
          {
            // this is something else than an "M18/M84 S<timeout>", this will disable at least one stepper,
            // set coordinate as unknown
            coordinateSetKnown(false);
            setAxisKnown(X_AXIS,false);
            setAxisKnown(Y_AXIS,false);
            setAxisKnown(Z_AXIS,false);
          }
          break;

        #ifdef SERIAL_PORT_2
          case 20:  // M20
            if (!fromTFT)
            {
              if (initRemoteTFT())  // examples: "M20 SD:/test\n", "M20 S /test\n"
              {
                Serial_Forward(cmd_port_index, "Begin file list\n");

                // then mount FS and scan for files (infoFile.source and infoFile.path are used)
                if (mountFS() == true && scanPrintFiles() == true)
                {
                  for (uint16_t i = 0; i < infoFile.fileCount; i++)
                  {
                    Serial_Forward(cmd_port_index, infoFile.file[i]);
                    Serial_Forward(cmd_port_index, "\n");
                  }

                  for (uint16_t i = 0; i < infoFile.folderCount; i++)
                  {
                    Serial_Forward(cmd_port_index, "/");
                    Serial_Forward(cmd_port_index, infoFile.folder[i]);
                    Serial_Forward(cmd_port_index, "/\n");
                  }
                }

                Serial_Forward(cmd_port_index, "End file list\nok\n");

                sendCmd(true, avoid_terminal);
                return;
              }
            }
            break;

          case 23:  // M23
            if (!fromTFT)
            {
              if (initRemoteTFT())  // examples: "M23 SD:/test/cap2.gcode\n", "M23 S /test/cap2.gcode\n"
              {
                openRemoteTFT(false);

                sendCmd(true, avoid_terminal);
                return;
              }
            }
            break;

          case 24:  // M24
            if (!fromTFT)
            {
              // NOTE: If the file was selected (with M23) from onboard media, infoFile.source will be set to
              //       FS_ONBOARD_MEDIA_REMOTE by the startPrintingFromRemoteHost() function called in
              //       Mainboard_AckHandler.c during M23 ACK parsing

              if (infoFile.source < FS_ONBOARD_MEDIA)  // if a file was selected from TFT media with M23
              {
                // firstly purge the gcode to avoid a possible reprocessing or infinite nested loop in
                // case the function loopProcess() is invoked by the following function pausePrint()
                sendCmd(true, avoid_terminal);

                if (!isPrinting())  // if not printing, start a new print and open Printing menu
                  startPrinting();
                else                // if printing, resume the print, in case it is paused, or continue to print
                  pausePrint(false, PAUSE_NORMAL);

                Serial_Forward(cmd_port_index, "ok\n");
                return;
              }
            }
            break;

          case 25:   // M25
          case 125:  // M125
          case 524:  // M524
            if (!fromTFT)
            {
              if (isPrintingFromTFT())  // if printing from TFT media
              {
                // firstly purge the gcode to avoid a possible reprocessing or infinite nested loop in
                // case the function loopProcess() is invoked by the following function pausePrint() / abortPrint()
                sendCmd(true, avoid_terminal);

                if (cmd_value() != 524)            // if M25 or M125
                  pausePrint(true, PAUSE_NORMAL);
                else                               // if M524
                  abortPrint();

                Serial_Forward(cmd_port_index, "ok\n");
                return;
              }
            }
            break;

          case 27:  // M27
            if (rrfStatusIsMacroBusy())
            {
              sendCmd(true, avoid_terminal);
              return;
            }

            if (!fromTFT)
            {
              if (isPrintingFromTFT())  // if printing from TFT media
              {
                char buf[55];

                if (cmd_seen('C'))
                {
                  Serial_Forward(cmd_port_index, "Current file: ");
                  Serial_Forward(cmd_port_index, infoFile.path);
                  Serial_Forward(cmd_port_index, ".\n");
                }

                sprintf(buf, "%s printing byte %d/%d\n", (infoFile.source == FS_TFT_SD) ? "TFT SD" : "TFT USB", getPrintDataCur(), getPrintDataSize());
                Serial_Forward(cmd_port_index, buf);
                Serial_Forward(cmd_port_index, "ok\n");

                sendCmd(true, avoid_terminal);
                return;
              }
            }
            else
            {
              printClearSendingWaiting();
            }
            break;

          case 28:  // M28
            if (!fromTFT)
            {
              if (initRemoteTFT())  // examples: "M28 SD:/test/cap2.gcode\n", "M28 S /test/cap2.gcode\n"
              {
                if (openRemoteTFT(true))  // if file was successfully open, switch to TFT writing mode
                {
                  writing_mode = TFT_WRITING;
                  setReminderMsg(LABEL_LISTENING, SYS_STATUS_LISTENING);
                }

                sendCmd(true, avoid_terminal);
                return;
              }
              else  // if it's a request to onboard media, switch to onboard writing mode and forward the command to onboard
              {
                writing_mode = ONBOARD_WRITING;
                setReminderMsg(LABEL_LISTENING, SYS_STATUS_LISTENING);
              }
            }
            break;

          case 29:  // M29
            if (!fromTFT)
            {
              // NOTE: this scenario is reachable only if not already in writing mode (no M28 was previously received).
              //       So, we only need to send back and ACK message

              Serial_Forward(cmd_port_index, "ok\n");
              sendCmd(true, avoid_terminal);
              return;
            }
            break;

          case 30:  // M30
            if (!fromTFT)
            {
              if (initRemoteTFT())  // examples: "M30 SD:/test/cap2.gcode\n", "M30 S /test/cap2.gcode\n"
              {
                // then mount FS and delete the file (infoFile.source and infoFile.path are used)
                if (mountFS() == true && f_unlink(infoFile.path) == FR_OK)
                  Serial_Forward(cmd_port_index, "File deleted: ");
                else
                  Serial_Forward(cmd_port_index, "Deletion failed, File: ");

                Serial_Forward(cmd_port_index, infoFile.path);
                Serial_Forward(cmd_port_index, ".\nok\n");

                sendCmd(true, avoid_terminal);
                return;
              }
            }
            break;

          case 98:  // M98 RRF macro execution, do not wait for it to complete
            sendCmd(false, avoid_terminal);
            return;

          case 115:  // M115
            if (!fromTFT && cmd_seen_from(cmd_base_index, "TFT"))  // "M115 TFT"
            {
              char buf[50];

              Serial_Forward(cmd_port_index,
                             "FIRMWARE_NAME: " FIRMWARE_NAME
                             " SOURCE_CODE_URL:https://github.com/bigtreetech/BIGTREETECH-TouchScreenFirmware\n");
              sprintf(buf, "Cap:HOTEND_NUM:%d\n", infoSettings.hotend_count);
              Serial_Forward(cmd_port_index, buf);
              sprintf(buf, "Cap:EXTRUDER_NUM:%d\n", infoSettings.ext_count);
              Serial_Forward(cmd_port_index, buf);
              sprintf(buf, "Cap:FAN_NUM:%d\n", infoSettings.fan_count);
              Serial_Forward(cmd_port_index, buf);
              sprintf(buf, "Cap:FAN_CTRL_NUM:%d\n", infoSettings.ctrl_fan_en ? MAX_CTRL_FAN_COUNT : 0);
              Serial_Forward(cmd_port_index, buf);
              Serial_Forward(cmd_port_index, "ok\n");

              sendCmd(true, avoid_terminal);
              return;
            }
            break;

          case 118:  // M118
          {
            CMD msg;
            CMD rawMsg;
            const char * msgText;
            bool hasE, hasA;

            // make a copy to work on
            strncpy_no_pad(rawMsg, &cmd_ptr[cmd_base_index + 4], CMD_MAX_SIZE);

            // retrieve message text and flags of M118 gcode
            msgText = parseM118(rawMsg, &hasE, &hasA);

            // format: <E prefix> + <A prefix> + <text> + "\n"
            snprintf(msg, CMD_MAX_SIZE, "%s%s%s\n", (hasE == true) ? "echo:" : "", (hasA == true) ? "//" : "", msgText);

            int32_t fwdPort = cmd_seen('P') ? cmd_value() : SUP_PORTS;

            if (fwdPort == 0)  // if P value is 0 (all ports), map it to the TFT all supplementary serial ports
              fwdPort = SUP_PORTS;

            // forward the message to all the supplementary or the provided enabled serial ports
            Serial_Forward(fwdPort, msg);
            break;
          }

        #else  // not SERIAL_PORT_2
          case 27:  // M27
            printClearSendingWaiting();
            break;
        #endif  // SERIAL_PORT_2

        case 73:  // M73
          if (cmd_seen('P'))
          {
            setPrintProgressSource(PROG_SLICER);
            setPrintProgressPercentage(cmd_value());
          }

          if (cmd_seen('R'))
          {
            setPrintRemainingTime(cmd_value() * 60);
            setTimeFromSlicer(true);  // disable parsing remaning time from gcode comments

            if (getPrintProgressSource() < PROG_TIME && infoSettings.prog_source == 1)
              setPrintProgressSource(PROG_TIME);
          }

          if (!infoMachineSettings.buildPercent)  // if M73 is not supported by Marlin, skip it
          {
            sendCmd(true, avoid_terminal);
            return;
          }
          break;

        #ifdef PS_ON_PIN
          case 80:  // M80
            PS_ON_On();
            break;

          case 81:  // M81
            PS_ON_Off();
            break;
        #endif

        case 82:  // M82
          eSetRelative(false);
          break;

        case 83:  // M83
          eSetRelative(true);
          break;

        case 105:  // M105
        case 155:  // M155
          if (rrfStatusIsMacroBusy())
          {
            sendCmd(true, avoid_terminal);
            return;
          }

          if (fromTFT)
          {
            heatClearSendingWaiting();

            if (cmd_value() == 105)  // if M105
            {
              avoid_terminal = !infoSettings.terminal_ack;
            }
            else  // if M155
            {
              if (cmd_seen('S'))
                heatSyncUpdateSeconds(cmd_value());
            }
          }
          break;

        case 106:  // M106
          fanSetCurSpeed(cmd_seen('P') ? cmd_value() : 0, cmd_seen('S') ? cmd_value() : 100);
          break;

        case 107:  // M107
          fanSetCurSpeed(cmd_seen('P') ? cmd_value() : 0, 0);
          break;

        case 109:  // M109
          if (fromTFT)
          {
            if (GET_BIT(infoSettings.general_settings, INDEX_EMULATED_M109_M190) == 0)  // if emulated M109 / M190 is disabled
              break;

            cmd_ptr[cmd_base_index + 3] = '4';  // avoid to send M109 to Marlin, send M104
            setWaitHeating(cmd_seen('T') ? cmd_value() : heatGetCurrentHotend());
          }
        // no break here. The data processing of M109 is the same as that of M104 below
        case 104:  // M104
          if (fromTFT)
          {
            syncTargetTemp(cmd_seen('T') ? cmd_value() : heatGetCurrentHotend());
          }
          break;

        case 110:  // M110
          setCmdLineNumberBase(cmd_seen('N') ? (uint32_t)cmd_value() : 0);
          break;

        case 114:  // M114
          if (fromTFT)
          {
            if (!cmd_seen('E'))
              coordinateQueryClearSendingWaiting();
            #ifdef FIL_RUNOUT_PIN
              else
                FIL_PosE_ClearSendingWaiting();
            #endif
          }
          break;

        case 117:  // M117
          if (cmd_seen_from(cmd_base_index, "Time Left"))  // parsing printing time left
          {
            // format: Time Left <XX>h<YY>m<ZZ>s (e.g. Time Left 02h04m06s)
            parsePrintRemainingTime(&cmd_ptr[cmd_index]);  // cmd_index was set by cmd_seen_from() function
            setTimeFromSlicer(true);
          }
          else if (cmd_seen_from(cmd_base_index, "Layer Left"))  // parsing printing layer left
          {
            // format: Layer Left <XXXX>/<YYYY> (e.g. Layer Left 51/940)
            setPrintLayerNumber(cmd_value());
            setPrintLayerCount(cmd_second_value());
          }
          else if (cmd_seen_from(cmd_base_index, "Data Left"))  // parsing printing data left
          {
            // format: Data Left <XXXX>/<YYYY> (e.g. Data Left 123/12345)
            setPrintProgressData(cmd_value(), cmd_second_value());
          }
          else
          {
            CMD rawMsg;
            const char * msgText;

            // make a copy to work on
            strncpy_no_pad(rawMsg, &cmd_ptr[cmd_base_index + 4], CMD_MAX_SIZE);

            // retrieve message text
            stripCmdChecksum(rawMsg);
            msgText = stripCmdHead(rawMsg);

            statusSetMsg((uint8_t *)"M117", (uint8_t *)msgText);

            if (MENU_IS_NOT(menuStatus))
              addToast(DIALOG_TYPE_INFO, (char *)msgText);
          }
          break;

        case 190:  // M190
          if (fromTFT)
          {
            if (GET_BIT(infoSettings.general_settings, INDEX_EMULATED_M109_M190) == 0)  // if emulated M109 / M190 is disabled
              break;

            cmd_ptr[cmd_base_index + 2] = '4';  // avoid to send M190 to Marlin, send M140
            setWaitHeating(BED);
          }
        // no break here. The data processing of M190 is the same as that of M140 below
        case 140:  // M140
          if (fromTFT)
          {
            syncTargetTemp(BED);
          }
          break;

        case 191:  // M191
          if (fromTFT)
          {
            cmd_ptr[cmd_base_index + 2] = '4';  // avoid to send M191 to Marlin, send M141
            setWaitHeating(CHAMBER);
          }
        // no break here. The data processing of M191 is the same as that of M141 below
        case 141:  // M141
          if (fromTFT)
          {
            syncTargetTemp(CHAMBER);
          }
          break;

        case 200:  // M200 filament diameter
        {
          if (cmd_seen('S')) setParameter(P_FILAMENT_DIAMETER, 0, cmd_float());

          uint8_t i = (cmd_seen('T')) ? cmd_value() : 0;

          if (cmd_seen('D')) setParameter(P_FILAMENT_DIAMETER, 1 + i, cmd_float());

          if (infoMachineSettings.firmwareType == FW_SMOOTHIEWARE)
          {
            // filament_diameter > 0.01 to enable volumetric extrusion. Otherwise (<= 0.01), disable volumetric extrusion
            setParameter(P_FILAMENT_DIAMETER, 0, getParameter(P_FILAMENT_DIAMETER, 1) > 0.01f ? 1 : 0);
          }
          break;
        }

        case 92:   // M92 axis steps-per-unit (steps/mm)
        case 201:  // M201 max acceleration (units/s2)
        case 203:  // M203 max feedrate (units/s)
        {
          PARAMETER_NAME param = P_STEPS_PER_MM;  // default value

          // using consecutive "if" instead of "if else if" on the following two lines just to reduce code
          // instead of optimizing performance (code typically not executed during a print)
          if (cmd_value() == 201) param = P_MAX_ACCELERATION;
          if (cmd_value() == 203) param = P_MAX_FEED_RATE;

          if (cmd_seen('X')) setParameter(param, AXIS_INDEX_X, cmd_float());
          if (cmd_seen('Y')) setParameter(param, AXIS_INDEX_Y, cmd_float());
          if (cmd_seen('Z')) setParameter(param, AXIS_INDEX_Z, cmd_float());

          uint8_t i = (cmd_seen('T')) ? cmd_value() : 0;

          if (cmd_seen('E')) setParameter(param, AXIS_INDEX_E0 + i, cmd_float());
          break;
        }

        case 204:  // M204 acceleration (units/s2)
          if (cmd_seen('P')) setParameter(P_ACCELERATION, 0, cmd_float());
          if (cmd_seen('R')) setParameter(P_ACCELERATION, 1, cmd_float());
          if (cmd_seen('T')) setParameter(P_ACCELERATION, 2, cmd_float());
          break;

        case 205:  // M205 advanced settings
          if (cmd_seen('X')) setParameter(P_JERK, AXIS_INDEX_X, cmd_float());
          if (cmd_seen('Y')) setParameter(P_JERK, AXIS_INDEX_Y, cmd_float());
          if (cmd_seen('Z')) setParameter(P_JERK, AXIS_INDEX_Z, cmd_float());
          if (cmd_seen('E')) setParameter(P_JERK, AXIS_INDEX_E0, cmd_float());
          if (cmd_seen('J')) setParameter(P_JUNCTION_DEVIATION, 0, cmd_float());
          break;

        case 206:  // M206 home offset
        case 218:  // M218 hotend offset
        case 851:  // M851 probe offset
        {
          PARAMETER_NAME param = P_HOME_OFFSET;  // default value

          // using consecutive "if" instead of "if else if" on the following two lines just to reduce code
          // instead of optimizing performance (code typically not executed during a print)
          if (cmd_value() == 218) param = P_HOTEND_OFFSET;
          if (cmd_value() == 851) param = P_PROBE_OFFSET;

          if (cmd_seen('X')) setParameter(param, AXIS_INDEX_X, cmd_float());
          if (cmd_seen('Y')) setParameter(param, AXIS_INDEX_Y, cmd_float());
          if (cmd_seen('Z')) setParameter(param, AXIS_INDEX_Z, cmd_float());
          break;
        }

        case 207:  // M207 FW retraction
        case 208:  // M208 FW recover
        {
          PARAMETER_NAME param = (cmd_value() == 207) ? P_FWRETRACT : P_FWRECOVER;

          if (cmd_seen('S')) setParameter(param, 0, cmd_float());
          if (cmd_seen('W')) setParameter(param, 1, cmd_float());
          if (cmd_seen('F')) setParameter(param, 2, cmd_float());

          if (param == P_FWRETRACT)
          {
            if (cmd_seen('Z')) setParameter(param, 3, cmd_float());
          }
          else  // P_FWRECOVER
          {
            if (cmd_seen('R')) setParameter(param, 3, cmd_float());
          }
          break;
        }

        case 209:  // M209 auto retract
          if (cmd_seen('S')) setParameter(P_AUTO_RETRACT, 0, cmd_float());
          break;

        case 220:  // M220
          if (cmd_seen('S'))
            speedSetCurPercent(0, cmd_value());

          if (fromTFT)
            speedQueryClearSendingWaiting();
          break;

        case 221:  // M221
          if (cmd_seen('S'))
            speedSetCurPercent(1, cmd_value());

          if (fromTFT)
            speedQueryClearSendingWaiting();
          break;

        #ifdef BUZZER_PIN
          case 300:  // M300
          {
            if (fromTFT || cmd_seen_from(cmd_base_index, "TFT"))  // if M300 issued from TFT or "M300 TFT", play sound on TFT
            {
              uint16_t hz = 260;   // default Marlin tone frequency: 260Hz
              uint16_t ms = 1000;  // default Marlin tone duration: 1000ms

              if (cmd_seen('S'))
                hz = cmd_value();

              if (cmd_seen('P'))
                ms = cmd_value();

              Buzzer_HandleMute(ms == 0 ? true : false);  // if sound duration is 0, mute the TFT. Otherwise, unmute the TFT
              Buzzer_AddSound(hz, ms);

              if (!fromTFT)
                Serial_Forward(cmd_port_index, "ok\n");

              sendCmd(true, avoid_terminal);
              return;
            }
            break;  // if here, forward sound to mainboard
          }
        #endif

        case 301:  // Hotend PID
        case 304:  // Bed PID
        {
          PARAMETER_NAME param = (cmd_value() == 301) ? P_HOTEND_PID : P_BED_PID;

          if (cmd_seen('P')) setParameter(param, 0, cmd_float());
          if (cmd_seen('I')) setParameter(param, 1, cmd_float());
          if (cmd_seen('D')) setParameter(param, 2, cmd_float());
          break;
        }

        // case 306:  // M306
        //   if (getMpcTuningStatus() == REQUESTED && cmd_seen('T'))  // only if requested by GUI
        //     setMpcTuningStatus(STARTED);
        //   break;

        case 355:  // M355
          if (cmd_seen('S'))
            caseLightSetState(cmd_value() > 0);

          if (cmd_seen('P'))
            caseLightSetPercent(cmd_value());
          break;

        case 376:  // M376 (RepRap firmware)
          if (infoMachineSettings.firmwareType == FW_REPRAPFW && cmd_seen('H'))
            setParameter(P_ABL_STATE, 1, cmd_float());
          break;

        case 292:  // M292
        case 408:  // M408
          // RRF does not send "ok" while executing M98
          if (rrfStatusIsMacroBusy())
          {
            sendCmd(false, avoid_terminal);
            return;
          }
          break;

        //case 420:  // M420
        //  // ABL state and Z fade height will be set through parsACK.c after receiving confirmation
        //  // message from the printer to prevent wrong state and/or value in case of error
        //  break;

        case 593:  // M593 Input Shaping (only for Marlin)
        {
          if (infoMachineSettings.firmwareType == FW_MARLIN)
          {
            // M593 accepts its parameters in any order,
            // if both X and Y axis are missing than the rest
            // of the parameters are referring to each axis

            enum
            {
              SET_NONE = 0B00,
              SET_X = 0B01,
              SET_Y = 0B10,
              SET_BOTH = 0B11
            } setAxis = SET_NONE;

            float pValue;

            if (cmd_seen('X')) setAxis |= SET_X;
            if (cmd_seen('Y')) setAxis |= SET_Y;
            if (setAxis == SET_NONE) setAxis = SET_BOTH;

            if (cmd_seen('F'))
            {
              pValue = cmd_float();

              if (setAxis & SET_X) setParameter(P_INPUT_SHAPING, 0, pValue);
              if (setAxis & SET_Y) setParameter(P_INPUT_SHAPING, 2, pValue);
            }

            if (cmd_seen('D'))
            {
              pValue = cmd_float();

              if (setAxis & SET_X) setParameter(P_INPUT_SHAPING, 1, pValue);
              if (setAxis & SET_Y) setParameter(P_INPUT_SHAPING, 3, pValue);
            }
          }
        }

        case 569:  // M569 TMC stepping mode
        {
          float isStealthChop = (cmd_seen('S')) ? cmd_value() : 0;  // integer type value also casted to float type
          int8_t stepperIndex = (cmd_seen('I')) ? cmd_value() : 0;

          // if index is missing or set to -1 (meaning all indexes) then it must be converted to 0
          // to make sure array index is never negative
          if (stepperIndex < 0)
            stepperIndex = 0;

          if (cmd_seen('X')) setParameter(P_STEALTH_CHOP, STEPPER_INDEX_X + stepperIndex, isStealthChop);
          if (cmd_seen('Y')) setParameter(P_STEALTH_CHOP, STEPPER_INDEX_Y + stepperIndex, isStealthChop);
          if (cmd_seen('Z')) setParameter(P_STEALTH_CHOP, STEPPER_INDEX_Z + stepperIndex, isStealthChop);

          stepperIndex = (cmd_seen('T')) ? cmd_value() : 0;

          // if index is missing or set to -1 (meaning all indexes) then it must be converted to 0
          // to make sure array index is never negative
          if (stepperIndex < 0)
            stepperIndex = 0;

          if (cmd_seen('E')) setParameter(P_STEALTH_CHOP, STEPPER_INDEX_E0 + stepperIndex, isStealthChop);
          break;
        }

        case 600:  // M600 filament change
        #ifdef NOZZLE_PAUSE_M601
          case 601:  // M601 print pause (PrusaSlicer)
        #endif
          if (isPrinting())
          {
            // purge and pause only if emulated M600 is enabled.
            // if emulated M600 is disabled then let the printer pause the print to avoid premature pause
            if (GET_BIT(infoSettings.general_settings, INDEX_EMULATED_M600) == 1)
            {
              sendCmd(true, avoid_terminal);
              pausePrint(true, PAUSE_NORMAL);
              return;
            }
          }
          break;

        case 665:  // Delta configuration / Delta tower angle
        case 666:  // Delta endstop adjustments
        {
          PARAMETER_NAME param = (cmd_value() == 665) ? P_DELTA_TOWER_ANGLE : P_DELTA_ENDSTOP;

          if (param < P_DELTA_ENDSTOP)  // options not supported by M666
          {
            if (cmd_seen('H')) setParameter(P_DELTA_CONFIGURATION, 0, cmd_float());
            if (cmd_seen('S')) setParameter(P_DELTA_CONFIGURATION, 1, cmd_float());
            if (cmd_seen('R')) setParameter(P_DELTA_CONFIGURATION, 2, cmd_float());
            if (cmd_seen('L')) setParameter(P_DELTA_CONFIGURATION, 3, cmd_float());
            if (cmd_seen('A')) setParameter(P_DELTA_DIAGONAL_ROD, AXIS_INDEX_X, cmd_float());
            if (cmd_seen('B')) setParameter(P_DELTA_DIAGONAL_ROD, AXIS_INDEX_Y, cmd_float());
            if (cmd_seen('C')) setParameter(P_DELTA_DIAGONAL_ROD, AXIS_INDEX_Z, cmd_float());
          }

          if (cmd_seen('X')) setParameter(param, AXIS_INDEX_X, cmd_float());
          if (cmd_seen('Y')) setParameter(param, AXIS_INDEX_Y, cmd_float());
          if (cmd_seen('Z')) setParameter(param, AXIS_INDEX_Z, cmd_float());
          break;
        }

        case 710:  // M710 controller fan
          if (cmd_seen('S'))
            fanSetCurSpeed(MAX_COOLING_FAN_COUNT, cmd_value());

          if (cmd_seen('I'))
            fanSetCurSpeed(MAX_COOLING_FAN_COUNT + 1, cmd_value());

          if (fromTFT)
            ctrlFanQueryClearSendingWaiting();
          break;

        case 900:  // M900 linear advance factor
        {
          uint8_t i = 0;

          if (cmd_seen('T')) i = cmd_value();
          if (cmd_seen('K')) setParameter(P_LIN_ADV, i, cmd_float());
          break;
        }

        case 906:  // M906 stepper motor current
        case 913:  // M913 TMC hybrid threshold speed
        case 914:  // M914 TMC bump sensitivity
        {
          PARAMETER_NAME param = P_CURRENT;  // default value

          // using consecutive "if" instead of "if else if" on the following two lines just to reduce code
          // instead of optimizing performance (code typically not executed during a print)
          if (cmd_value() == 913) param = P_HYBRID_THRESHOLD;
          if (cmd_value() == 914) param = P_BUMPSENSITIVITY;

          int8_t stepperIndex = (cmd_seen('I')) ? cmd_value() : 0;

          // if index is missing or set to -1 (meaning all indexes) then it must be converted to 0
          // to make sure array index is never negative
          if (stepperIndex < 0)
            stepperIndex = 0;

          // for M913 and M914, provided index is:
          //   1->"X1", 2->"X2", 1->"Y1", 2->"Y2", 1->"Z1", 2->"Z2", 3->"Z3", 4->"Z4"
          // and it must be converted to:
          //   0->"X1", 1->"X2", 0->"Y1", 1->"Y2", 0->"Z1", 1->"Z2", 2->"Z3", 3->"Z4"
          // to make sure array index is properly accessed
          if (param > P_CURRENT && stepperIndex > 0)
            stepperIndex--;

          if (cmd_seen('X')) setParameter(param, STEPPER_INDEX_X + stepperIndex, cmd_value());
          if (cmd_seen('Y')) setParameter(param, STEPPER_INDEX_Y + stepperIndex, cmd_value());
          if (cmd_seen('Z')) setParameter(param, STEPPER_INDEX_Z + stepperIndex, cmd_value());

          if (param < P_BUMPSENSITIVITY)  // T and E options not supported by M914
          {
            stepperIndex = (cmd_seen('T')) ? cmd_value() : 0;

            // if index is missing or set to -1 (meaning all indexes) then it must be converted to 0
            // to make sure array index is never negative
            if (stepperIndex < 0)
              stepperIndex = 0;

            if (cmd_seen('E')) setParameter(param, STEPPER_INDEX_E0 + stepperIndex, cmd_value());
          }
          break;
        }
      }
      break;  // end parsing M-codes

    case 'G':
      switch (cmd_value())
      {
        case 0:  // G0
        case 1:  // G1
        case 2:  // G2
        case 3:  // G3
        {
          for (AXIS i = X_AXIS; i < TOTAL_AXIS; i++)
          {
            if (cmd_seen(axis_id[i]))
              coordinateSetAxisTarget(i, cmd_float());
          }
          if (cmd_seen('S'))
            laserSetCurSpeed(cmd_value());
          if (cmd_seen('F'))
            coordinateSetFeedRate(cmd_value());
          break;
        }

        case 28:  // G28
          coordinateSetKnown(true);
          babystepSetValue(BABYSTEP_DEFAULT_VALUE);  // reset babystep
          if (cmd_seen('X')) setAxisKnown(X_AXIS,true);
          if (cmd_seen('Y')) setAxisKnown(Y_AXIS,true);
          if (cmd_seen('Z')) setAxisKnown(Z_AXIS,true);
          if (!cmd_seen('Z') && !cmd_seen('X') && !cmd_seen('Y')) {
            setAxisKnown(X_AXIS,true);
            setAxisKnown(Y_AXIS,true);
            setAxisKnown(Z_AXIS,true);
          }

          if (infoMachineSettings.leveling != BL_DISABLED)
            storeCmd("M420\n");  // check bed leveling state
          break;

        #if BED_LEVELING_TYPE > 0  // if bed leveling is enabled
          case 29:  // G29
            if (infoMachineSettings.firmwareType == FW_REPRAPFW)
            {
              if (cmd_seen('S'))
              {
                uint8_t v = cmd_value();

                if (v == 1 || v == 2)
                  setParameter(P_ABL_STATE, 0, v & 1U);  // value will be 1 if v == 1, 0 if v == 2
              }
            }
            #if BED_LEVELING_TYPE == 4  // if UBL
              else if (infoMachineSettings.firmwareType == FW_MARLIN)
              {
                // if (cmd_seen('A'))
                // {
                //   setParameter(P_ABL_STATE, 0, 1);
                //   storeCmd("M117 UBL active\n");
                // }
                // else if (cmd_seen('D'))
                // {
                //   setParameter(P_ABL_STATE, 0, 0);
                //   storeCmd("M117 UBL inactive\n");
                // }
                /*
                   Bed leveling state will be set through "parsACK.c" after receiving confirmation
                   message from the printer to prevent wrong state and/or value in case of error
                */

                if (cmd_seen('A') || cmd_seen('D')) storeCmd("M420\n");  // check bed leveling state
              }
            #endif
          break;
        #endif

        case 90:  // G90, set absolute position mode, in Marlin this includes the extruder position unless overridden by M83
          coorSetRelative(false);

          if (infoMachineSettings.firmwareType == FW_MARLIN)
            eSetRelative(false);
          break;

        case 91:  // G91, set relative position mode, in Marlin this includes the extruder position unless overridden by M82
          coorSetRelative(true);

          if (infoMachineSettings.firmwareType == FW_MARLIN)
            eSetRelative(true);
          break;

        case 92:  // G92
        {
          bool coorRelative = coorGetRelative();
          bool eRelative = eGetRelative();

          // set to absolute mode
          coorSetRelative(false);
          eSetRelative(false);

          for (AXIS i = X_AXIS; i < TOTAL_AXIS; i++)
          {
            if (cmd_seen(axis_id[i]))
            {
              coordinateSetAxisTarget(i, cmd_float());

              #ifdef FIL_RUNOUT_PIN
                if (i == E_AXIS)  // reset SFS status. Avoid false Filament runout caused by G92 resetting E-axis position
                  FIL_SFS_SetAlive(true);
              #endif
            }
          }

          // restore mode
          coorSetRelative(coorRelative);
          eSetRelative(eRelative);
          break;
        }
      }
      break;  // end parsing G-codes
    case 'C':
      switch (cmd_value())
      {
        case 100:  // C100
          if (cmd_seen('X')) infoParameters.MachineMin[X_AXIS] = cmd_float();
          if (cmd_seen('Y')) infoParameters.MachineMin[Y_AXIS] = cmd_float();
          if (cmd_seen('Z')) infoParameters.MachineMin[Z_AXIS] = cmd_float();
          break;
        case 101:  // C101
          if (cmd_seen('X')) infoParameters.MachineMax[X_AXIS] = cmd_float();
          if (cmd_seen('Y')) infoParameters.MachineMax[Y_AXIS] = cmd_float();
          if (cmd_seen('Z')) infoParameters.MachineMax[Z_AXIS] = cmd_float();
          break;
        case 102:  // C102
          if (cmd_seen('X')) infoParameters.BedSize[X_AXIS] = cmd_float();
          if (cmd_seen('Y')) infoParameters.BedSize[Y_AXIS] = cmd_float();
          if (cmd_seen('Z')) infoParameters.BedSize[Z_AXIS] = cmd_float();
          break;

      }
      break;
    case 'T':
      heatSetToolIndex(cmd_value());
      break;
  }  // end parsing cmd

send_cmd:
  // if command was sent:
  //   - if TFT is connected, update tx slots and tx count
  //   - if TFT is not connected, consider the command as an out of band message
  //
  if (sendCmd(false, avoid_terminal) == true && infoHost.connected == true)
  {
    // decrease the number of available tx slots and increase the pending commands tx count
    //
    // NOTE: no need to check infoHost.tx_slots > 0 before decreasing infoHost.tx_slots because the check
    //       infoHost.tx_slots == 0 was already performed at the beginning of sendQueueCmd() function
    //
    infoHost.tx_slots--;
    infoHost.tx_count++;
  }
}  // sendQueueCmd
