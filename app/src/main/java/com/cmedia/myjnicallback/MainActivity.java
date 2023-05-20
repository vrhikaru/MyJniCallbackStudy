package com.cmedia.myjnicallback;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Debug;
import android.util.Log;

import com.cmedia.nativefunction.JNIListener;
import com.cmedia.nativefunction.NativeLib;

public class MainActivity extends AppCompatActivity {
    final static String TAG = "MainActivity";
    NativeLib nativeLib;
    JNIListener jniListener;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


    }

    @Override
    protected void onStart(){
        super.onStart();
        Thread testLoop = new Thread(new Runnable() {
            @Override
            public void run() {
                callbackFromNativeTest();
            }
        });
        testLoop.start();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        nativeLib.ndismissListener();
    }

    void callbackFromNativeTest(){
        nativeLib = new NativeLib();
        jniListener = new JNIListener() {
            @Override
            public void onAcceptMessage(String string) {
                Log.d(TAG, "onAcceptMessage = " + string);
            }

            @Override
            public void onAcceptMessageVal(int messVal) {
                Log.d(TAG, "onAcceptMessageVal = " + messVal);
            }
        };

        nativeLib.nsubscribeListener(jniListener);
    }
}