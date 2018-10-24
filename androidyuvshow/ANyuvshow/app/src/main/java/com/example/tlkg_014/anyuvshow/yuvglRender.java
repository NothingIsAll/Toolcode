package com.example.tlkg_014.anyuvshow;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class yuvglRender implements GLSurfaceView.Renderer {
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        yuvNatives.getInstance().onSurfaceCreated();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        yuvNatives.getInstance().onSurfaceChanged(width,height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        yuvNatives.getInstance().onDrawFrame();
    }
}
