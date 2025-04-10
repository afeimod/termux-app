#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <android/hardware_buffer.h>
#include <jni.h>
#include "OutputEvent.h"

void setNativeWindow(JNIEnv *env);

void setSurface(jobject sf);

void DisplayServerInit();

void ServerStart(void *);

void SendOutputEvent(termux_event);
