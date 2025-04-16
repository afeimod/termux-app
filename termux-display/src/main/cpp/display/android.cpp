
#include "client.h"
#include "server.h"
#include "termuxdc_server.h"
#include "LogUtil.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Render_initRenderWindow(JNIEnv *env, jclass clazz,
                                                jstring name) {
    setNativeWindow(env);
    displayServerInit();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Render_setClipboardSyncEnabled(JNIEnv *env, jclass clazz, jboolean enabled,
                                                       jboolean ignored) {
    // TODO: implement setClipboardSyncEnabled()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Render_sendClipboardAnnounce(JNIEnv *env, jobject thiz) {
    // TODO: implement sendClipboardAnnounce()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Render_sendClipboardEvent(JNIEnv *env, jobject thiz, jbyteArray text) {
    // TODO: implement sendClipboardEvent()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Render_sendWindowChange(JNIEnv *env, jclass clazz, jobject surface) {
    jobject sf = surface ? env->NewGlobalRef(surface) : NULL;
    setSurface(sf);
    setNativeWindow(env);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Render_sendMouseEvent(JNIEnv *env, jobject thiz, jfloat x, jfloat y,
                                              jint which_button, jboolean button_down,
                                              jboolean relative) {
    // TODO: implement sendMouseEvent()
    server_termux_event ev = {
            .type=EVENT_MOUSE,
    };
    ev.mouse={
            .t=EVENT_MOUSE,
            .x=x,
            .y=y,
            .detail=(uint8_t)which_button,
            .down=button_down,
            .relative=relative
    };
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Render_sendTouchEvent(JNIEnv *env, jobject thiz, jint action, jint id,
                                              jint x, jint y) {
    // TODO: implement sendTouchEvent()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Render_sendStylusEvent(JNIEnv *env, jobject thiz, jfloat x, jfloat y,
                                               jint pressure, jint tilt_x, jint tilt_y,
                                               jint orientation, jint buttons, jboolean eraser,
                                               jboolean mouse_mode) {
    // TODO: implement sendStylusEvent()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Render_requestStylusEnabled(JNIEnv *env, jclass clazz, jboolean enabled) {
    // TODO: implement requestStylusEnabled()
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_termux_display_Render_sendKeyEvent(JNIEnv *env, jobject thiz, jint scan_code,
                                            jint key_code, jboolean key_down) {
    // TODO: implement sendKeyEvent()
    return true;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Render_sendTextEvent(JNIEnv *env, jobject thiz, jbyteArray text) {
    // TODO: implement sendTextEvent()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Render_sendUnicodeEvent(JNIEnv *env, jobject thiz, jint code) {
    // TODO: implement sendUnicodeEvent()
}
