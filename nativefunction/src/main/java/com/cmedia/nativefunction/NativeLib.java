package com.cmedia.nativefunction;

public class NativeLib {

    // Used to load the 'nativefunction' library on application startup.
    static {
        System.loadLibrary("nativefunction");
    }

    /**
     * A native method that is implemented by the 'nativefunction' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void nsubscribeListener(JNIListener jnilistener);

    public native void nonNextListener(String message);

    public native void ndismissListener();
}