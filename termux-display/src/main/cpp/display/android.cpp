
#include "client.h"
#include "server.h"
#include "termuxdc_server.h"
#include "LogUtil.h"

void inputCallback(termuxdc_event ev) {
//    LOG_I("%d", ev.type);
}
void initClient() {
    display_client_init(800, 600);
    event_socket_init(inputCallback);
    pthread_t t;
    pthread_create(&t, NULL, reinterpret_cast<void *(*)(void *)>(display_client_start), nullptr);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_initDisplayWindow(JNIEnv *env, jclass clazz,
                                                  jstring name) {
    setNativeWindow(env);
    displayServerInit();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_startClient(JNIEnv *env, jclass clazz) {
    initClient();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_connect(JNIEnv *env, jclass) {
    // TODO: implement connect()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_setClipboardSyncEnabled(JNIEnv *env, jclass clazz, jboolean enabled,
                                                        jboolean ignored) {
    // TODO: implement setClipboardSyncEnabled()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_sendClipboardAnnounce(JNIEnv *env, jobject thiz) {
    // TODO: implement sendClipboardAnnounce()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_sendClipboardEvent(JNIEnv *env, jobject thiz, jbyteArray text) {
    // TODO: implement sendClipboardEvent()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_sendWindowChange(JNIEnv *env, jclass clazz, jobject surface) {
    jobject sf = surface ? env->NewGlobalRef(surface) : NULL;
    setSurface(sf);
    setNativeWindow(env);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_sendMouseEvent(JNIEnv *env, jobject thiz, jfloat x, jfloat y,
                                               jint which_button, jboolean button_down,
                                               jboolean relative) {
    // TODO: implement sendMouseEvent()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_sendTouchEvent(JNIEnv *env, jobject thiz, jint action, jint id,
                                               jint x, jint y) {
    // TODO: implement sendTouchEvent()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_sendStylusEvent(JNIEnv *env, jobject thiz, jfloat x, jfloat y,
                                                jint pressure, jint tilt_x, jint tilt_y,
                                                jint orientation, jint buttons, jboolean eraser,
                                                jboolean mouse_mode) {
    // TODO: implement sendStylusEvent()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_requestStylusEnabled(JNIEnv *env, jclass clazz, jboolean enabled) {
    // TODO: implement requestStylusEnabled()
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_termux_display_Display_sendKeyEvent(JNIEnv *env, jobject thiz, jint scan_code,
                                             jint key_code, jboolean key_down) {
    // TODO: implement sendKeyEvent()
    return true;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_sendTextEvent(JNIEnv *env, jobject thiz, jbyteArray text) {
    // TODO: implement sendTextEvent()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_sendUnicodeEvent(JNIEnv *env, jobject thiz, jint code) {
    // TODO: implement sendUnicodeEvent()
}
