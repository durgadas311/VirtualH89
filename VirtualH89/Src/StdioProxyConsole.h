/// \file StdioProxyConsole.h
///
/// A console replacement that supports an external process as the H19.
/// Also supports a out-of-band channel for command/control messages.
/// Typically, this is selected by commandline options for main(),
/// with the parent process as the H19 emulation.
///
/// \date Feb 6, 2016
/// \author Douglas Miller
///

#ifndef STDIOPROXYCONSOLE_H_
#define STDIOPROXYCONSOLE_H_

#include <assert.h>

#include "config.h"
#include "Console.h"
#include "H89Operator.h"

/// \brief StdioProxyConsole
///
///
class StdioProxyConsole : public Console
{
  public:
    StdioProxyConsole(int argc, char **argv);
    virtual ~StdioProxyConsole();

    virtual void init();
    virtual void reset();
    virtual void display();
    virtual void processCharacter(char ch);
    virtual void keypress(char ch);
    virtual void receiveData(BYTE);
    virtual bool checkUpdated();
    virtual unsigned int getBaudRate();
    virtual void run();

  private:
    H89Operator *op_m;
};

#endif // STDIOPROXYCONSOLE_H_
