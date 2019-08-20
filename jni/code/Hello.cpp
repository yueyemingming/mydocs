#include "Hello.h"

JNIEXPORT void JNICALL Java_Hello_SayHello (JNIEnv *e, jclass c)
{
	printf("Hello, Jni ~~~~~\n") ;
}

