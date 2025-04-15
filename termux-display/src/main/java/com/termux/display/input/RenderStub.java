package com.termux.display.input;



/**
 * Interface with a set of functions to control the behavior of the remote host renderer.
 */
public interface RenderStub {
    /**
     * Informs the stub that swipe was performed.
     */
    void swipeUp();

    /**
     * Informs the stub that swipe was performed.
     */
    void swipeDown();

    public static class NullStub implements RenderStub {
        @Override
        public void swipeUp() {
        }

        @Override
        public void swipeDown() {
        }
    }
}
