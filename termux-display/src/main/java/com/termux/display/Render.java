package com.termux.display;

import android.content.res.AssetManager;
import android.view.Surface;

import com.termux.display.input.RenderInputStub;

public class Render implements RenderInputStub {
    public interface RenderCallback {
        void onFinishInitialize();

        void onClientExit();

        void onNewClientCreate();
    }

    private RenderCallback renderCallback;

    public void setRenderCallback(RenderCallback callback) {
        renderCallback = callback;
    }

    public void notifyWindowChanged(int state) {
        if (renderCallback != null) {
            switch (state) {
                case 0: {
                    renderCallback.onFinishInitialize();
                    break;
                }
                case 1: {
                    renderCallback.onClientExit();
                    break;
                }
                case 2:{
                    renderCallback.onNewClientCreate();
                    break;
                }
            }
        }
    }

    @Override
    public void sendMouseWheelEvent(float deltaX, float deltaY) {
        sendMouseEvent(deltaX, deltaY, BUTTON_SCROLL, false, true);
    }
    native public void initRenderJNIEnv();

    static native public void initRenderWindow(String name);

    static native public void setServerNativeAssetManager(AssetManager assetManager);


    static native void setClipboardSyncEnabled(boolean enabled, boolean ignored);

    static native public void sendClipboardAnnounce();

    static public native void sendClipboardEvent(byte[] text);

    static public native void sendWindowChange(Surface surface);

    native public void sendMouseEvent(float x, float y, int whichButton, boolean buttonDown, boolean relative);

    native public void sendTouchEvent(int action, int id, int x, int y);

    static native public void sendStylusEvent(float x, float y, int pressure, int tiltX, int tiltY, int orientation, int buttons, boolean eraser, boolean mouseMode);

    static native public void requestStylusEnabled(boolean enabled);

    native public boolean sendKeyEvent(int scanCode, int keyCode, boolean keyDown);

    native public void sendTextEvent(byte[] text);

    native public void sendUnicodeEvent(int code);

    static native public void onFrameComplete(long frameTimeNanos);

    static {
        System.loadLibrary("Render");
    }
}
