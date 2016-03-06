/// \file startup.cpp
///
/// \date Mar 7, 2009
/// \author Mark Garlanger
///
/// Mar 6, 2016, Douglas Miller
/// Separated from main.cpp in order to share with things like JNIProxyConsole
///

#include "config.h"

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

#include "H89.h"
#include "Console.h"
#include "h19.h"
#include "StdioConsole.h"
#include "StdioProxyConsole.h"
#include "logger.h"
#include "startup.h"

/// \todo - make H89 into a singleton.
H89         h89;

FILE*       log_out     = 0;
FILE*       console_out = 0;

static void*
cpuThreadFunc(void* v)
{
    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    pthread_sigmask(SIG_BLOCK, &set, 0);
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    pthread_sigmask(SIG_UNBLOCK, &set, 0);
    H89* h89 = (H89*) v;

    // h89->clearMemory(0);
    // h89->reset();
    BYTE cpu_error;

    // Does not return until ready to exit.
    cpu_error = h89->run();

    return (0);
}

// This is the getopt string for ALL consumers of argc/argv.
// Right now, it must be manually kept up to date.
//
//	option		owner
//	-g <gui>	main.cpp
//	-l		StdioProxyConsole.cpp
//	-q		main.cpp
//
const char* getopts = "g:lq";

/// \todo - use argv[0] to determine configuration... ie H88 vs. H89 vs. Z90.
int
startup(int   argc,
        char* argv[],
        Console *console)
{

    h89.buildSystem(console);

    pthread_t thread;
    pthread_create(&thread, NULL, cpuThreadFunc, &h89);
    pthread_setname_np(thread, "cpuThreadFunc");
    h89.init();

    console->run();

    return (0);
}
