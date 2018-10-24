package com.example.tlkg_014.anyuvshow;

public class yuvNatives {
    private static yuvNatives instance = null;

    static yuvNatives getInstance(){
        if (instance == null){
            instance = new yuvNatives();
        }
        return instance;
    }

    static {
        System.loadLibrary("native-lib");
    }
    public native void onSurfaceCreated();
    public native void onSurfaceChanged(int width,int heigh);
    public native void onDrawFrame();
    public native int PutYuvData(byte[] data,int size,int width,int heigh);
    public native String stringFromJNI();
}
