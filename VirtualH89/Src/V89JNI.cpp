#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

#include "V89JNI.h"

// Implementation of native method startV89() of V89JNI class
JNIEXPORT void JNICALL Java_V89JNI_startV89(JNIEnv *env, jobject thisObj,
					jobjectArray args) {
	jsize argc = env->GetArrayLength(args);
	const char **argv = (const char **)malloc(sizeof(char *) * (argc + 1));
	jboolean foo;
	if (argv == NULL) return;
	int x;
	for (x = 0; x < argc; ++x) {
		jstring str = (jstring)env->GetObjectArrayElement(args, x);
		argv[x] = env->GetStringUTFChars(str, 0);
	}
	argv[x] = NULL;

	jclass cls = env->GetObjectClass(thisObj);
	jmethodID jputc = env->GetMethodID(cls, "putc", "(I)V");
	if (jputc == 0) {
		return;
	}
	jmethodID jputs = env->GetMethodID(cls, "puts", "(Ljava/lang/String;)V");
	if (jputs == 0) {
		return;
	}

	char buf[128];
	char *s;
	for (x = 0; x < argc; ++x) {
		sprintf(buf, "[%d] = \"%s\"\n", x, argv[x]);
		jstring js = env->NewStringUTF(buf);
		env->CallVoidMethod(thisObj, jputs, js);
	}
	jint ji = '\n';
	env->CallVoidMethod(thisObj, jputc, ji);
}
