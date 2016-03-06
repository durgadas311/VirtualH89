/// \file JNIProxyConsole.cpp
///
/// A console replacement that supports running inside a Java program.
///
/// \date Mar 6, 2016
/// \author Douglas Miller
///

#include "JNIProxyConsole.h"

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sstream>

#include "H89.h"
#include "h89-io.h"
#include "logger.h"
#include "H89Operator.h"
#include "startup.h"

extern FILE* log_out; 
extern FILE* console_out;

static JNIProxyConsole *console;
static H89Operator *oper;

JNIEXPORT void JNICALL Java_V89JNI_startV89(JNIEnv *env, jobject thisObj,
					jobjectArray args) {
	setDebugLevel();

	jsize argc = env->GetArrayLength(args);
	char **argv = (char **)malloc(sizeof(char *) * (argc + 1));
	if (argv == NULL) return;
	int x;
	for (x = 0; x < argc; ++x) {
		jstring str = (jstring)env->GetObjectArrayElement(args, x);
		argv[x] = (char *)env->GetStringUTFChars(str, 0);
	}
	argv[x] = NULL;

	log_out = fopen("op.out", "w");

    	oper = new H89Operator();

	console = new JNIProxyConsole(argc, argv, env, thisObj);
	// Normally, this does not return. but in our case, we're done with init.
	startup(argc, argv, console);
}

JNIEXPORT void JNICALL Java_V89JNI_doChar(JNIEnv *env, jobject thisObj,
					jint ch) {
	console->keypress(ch);
}

JNIEXPORT jstring JNICALL Java_V89JNI_doCmd(JNIEnv *env, jobject thisObj,
					jstring cmd) {
	const char *ccmd = env->GetStringUTFChars(cmd, 0);
        std::string resp = oper->handleCommand(ccmd);
	jstring js = env->NewStringUTF(resp.c_str());
	return js;
}

/// \brief JNIProxyConsole
///
///
JNIProxyConsole::JNIProxyConsole(int    argc,
                                 char** argv,
                                 JNIEnv *env,
                                 jobject thisObj):
    Console(argc, argv),
    javaEnv(NULL),
    jvm(NULL),
    putChar(0)
{
    int          c;
    extern char* optarg;
    extern int   optind;

    bool logConsole = false;
    optind = 1;
    while ((c = getopt(argc, argv, getopts)) != EOF)
    {
        switch (c)
        {
            case 'l':
                logConsole = true;
                break;
        }
    }
    if (logConsole) {
	console_out = fopen("console.out", "w");
    } else {
	console_out = NULL;
    }

	javaObj = env->NewGlobalRef(thisObj);
	env->GetJavaVM(&jvm);
}

JNIProxyConsole::~JNIProxyConsole() {
	if (javaEnv != NULL) {
        	javaEnv->DeleteGlobalRef(javaObj);
	}
	jvm->DetachCurrentThread();
}

void
JNIProxyConsole::init() {
}

void
JNIProxyConsole::reset() {
}

void
JNIProxyConsole::display() {
}

void
JNIProxyConsole::processCharacter(char ch) {
}

void
JNIProxyConsole::keypress(char ch)
{
    // try not to overrun the UART, but do not wait too long.
    int timeout = 4000;
    int sleep   = 100;

    while (!sendReady() && timeout > 0)
    {
        usleep(sleep);
        timeout -= sleep;
    }


    sendData(ch);
}

void
JNIProxyConsole::receiveData(BYTE ch)
{
    if (javaEnv == NULL) {
    	jint rc = jvm->AttachCurrentThread((void **)&javaEnv, NULL);
	if (rc == JNI_OK) {
    		javaClass = javaEnv->GetObjectClass(javaObj);
		putChar = javaEnv->GetMethodID(javaClass, "putChar", "(I)V");
	}
    }
	if (putChar != 0) {
    		jint ji = ch;
    		javaEnv->CallVoidMethod(javaObj, putChar, ji);
	}
    if (console_out != NULL)
    {
        fputc(ch, console_out);
        if (ch == '\n')
        {
            fflush(console_out);
        }
    }
}

bool
JNIProxyConsole::checkUpdated()
{
    return false;
}

unsigned int
JNIProxyConsole::getBaudRate()
{
    return SerialPortDevice::DISABLE_BAUD_CHECK;
}

void
JNIProxyConsole::run()
{
	// Not called from "operating" thread... can't do anything for java.
}
