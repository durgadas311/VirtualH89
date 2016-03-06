/// \file startup.h
///
/// \date Mar 6, 2016 
/// \author Douglas Miller
///
/// Separated startup functionality from main.cpp to share with JNIProxyConsole
///

#ifndef _STARTUP_H_
#define _STARTUP_H_

#include <stdio.h>

#include "config.h"

using namespace std;

extern FILE* log_out;
extern FILE* console_out;
extern const char* getopts;

class Console;

extern int startup(int argc, char **argv, Console *console);

#endif // _STARTUP_H_
