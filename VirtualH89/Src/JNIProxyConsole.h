/// \file JNIProxyConsole.h
///
/// A console replacement that supports running inside a Java program.
///
/// \date Mar 6, 2016
/// \author Douglas Miller
///

#ifndef JNIPROXYCONSOLE_H_
#define JNIPROXYCONSOLE_H_

#include <jni.h>
#include <assert.h>

#include "Console.h"

/// \brief JNIProxyConsole
///
///
class JNIProxyConsole: public Console
{
  public:
    JNIProxyConsole(int    argc,
                    char** argv,
                    JNIEnv *env,
                    jobject thisObj);
    virtual ~JNIProxyConsole();

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
    jobject javaObj;
    jmethodID putChar;
    JNIEnv *javaEnv;
    JavaVM *jvm;
    jclass javaClass;
};

extern "C" {
JNIEXPORT void JNICALL Java_V89JNI_startV89(JNIEnv *, jobject, jobjectArray);
JNIEXPORT jstring JNICALL Java_V89JNI_doCmd(JNIEnv *, jobject, jstring);
JNIEXPORT void JNICALL Java_V89JNI_doChar(JNIEnv *, jobject, jint);
};

#endif // JNIPROXYCONSOLE_H_
