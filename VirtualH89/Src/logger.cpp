/// \file logger.cpp
///
/// \date May 6, 2009
/// \author Mark Garlanger
///

#include "logger.h"
#include "WallClock.h"

#define ANSI 0

unsigned debugLevel[ssMax];


logger::logger():  printToFile(false),
                   printToScreen(false),
                   logFile(NULL)
{

}

logger::~logger()
{

}

void
setDebugLevel()
{
    logLevel defaultLevel = ERROR;

    debugLevel[ssH89]                    = defaultLevel; // The overall H89
    debugLevel[ssMEM]                    = defaultLevel; // Memory class
    debugLevel[ssRAM]                    = defaultLevel; // RAM accesses
    debugLevel[ssROM]                    = defaultLevel; // ROM accesses
    debugLevel[ssZ80]                    = defaultLevel; // Z80 CPU
    debugLevel[ssH37InterruptController] = INFO;         // Interrupt Controller
    debugLevel[ssInterruptController]    = defaultLevel; // Interrupt Controller
    debugLevel[ssAddressBus]             = defaultLevel; // Address Bus
    debugLevel[ssIO]                     = defaultLevel; // I/O ports
    debugLevel[ssH17]                    = defaultLevel; // H17 controller
    debugLevel[ssH37]                    = VERBOSE;      // H37 controller
    debugLevel[ssH47]                    = defaultLevel; // H47 controller
    debugLevel[ssH67]                    = defaultLevel; // H67 controller
    debugLevel[ssDiskDrive]              = defaultLevel; // Floppy disks
    debugLevel[ssH17_1]                  = defaultLevel;
    debugLevel[ssH17_4]                  = defaultLevel;
    debugLevel[ssH47Drive]               = defaultLevel;
    debugLevel[ssConsole]                = defaultLevel; // Console
    debugLevel[ssH19]                    = defaultLevel; // H19 Terminal
    debugLevel[ss8250]                   = defaultLevel; // 8250 Serial Port
    debugLevel[ssTimer]                  = defaultLevel; // 2 mSec Timer
    debugLevel[ssWallClock]              = defaultLevel; // Wall Clock.
    debugLevel[ssFloppyDisk]             = INFO;         // Floppy Disk
    debugLevel[ssGpp]                    = defaultLevel; // General Purpose Port
    debugLevel[ssParallel]               = defaultLevel; // Parallel Port Interface (Z47)
    debugLevel[ssStdioConsole]           = defaultLevel;
    debugLevel[ssMMS77316]               = defaultLevel;
    debugLevel[ssWD1797]                 = VERBOSE;
    debugLevel[ssGenericFloppyDrive]     = defaultLevel;
    debugLevel[ssRawFloppyImage]         = defaultLevel;
    debugLevel[ssMMS77320]               = defaultLevel;
    debugLevel[ssGenericSASIDrive]       = defaultLevel;
    debugLevel[ssHostFileBdos]           = defaultLevel;
    debugLevel[ssCPNetDevice]            = defaultLevel;
    debugLevel[ssSectorFloppyImage]      = INFO;
}

void
setDebug(subSystems ss,
         logLevel   level)
{
    debugLevel[ss] = level;
}

void
__debugss(enum subSystems subsys, enum logLevel level, const char* functionName,
          const char* fmt, ...)
{
    va_list vl;
#if ANSI
    int     __val = 0;
    if (level < ERROR)
    {
        __val = 31; /* FATAL = Red */
    }
    else if (level < WARNING)
    {
        __val = 33; /* ERROR = Yellow */
    }
    else if (level < INFO)
    {
        __val = 35; /* WARNING = Magenta */
    }
    else if (level < VERBOSE)
    {
        __val = 34; /* INFO = Blue */
    }
    else
    {
        __val = 32; /* default = Green */
    }
    fprintf(log_out, "\x1b[37m");
#endif
    WallClock::instance()->printTime(log_out);
#if ANSI
    fprintf(log_out, "\x1b[36m%s: \x1b[%dm", functionName, __val);
#else
    fprintf(log_out, "%s: ", functionName);
#endif
    va_start(vl, fmt);
    vfprintf(log_out, fmt, vl);
    va_end(vl);
#if ANSI
    fprintf(log_out, "\x1b[0m");
#endif
    fflush(log_out);
}
