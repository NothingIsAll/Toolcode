package com.example.tlkg_014.anyuvshow;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class yuvglsurfaceView extends GLSurfaceView {
    public yuvglsurfaceView(Context context) {
        super(context);
        setEGLContextClientVersion(2);
        yuvglRender render = new yuvglRender();
        setRenderer(render);
    }
    private yuvglRender renderer;
    public yuvglsurfaceView(Context context,AttributeSet attrs){
        super(context,attrs);
        setEGLContextClientVersion(2);
        renderer = new yuvglRender();
        setRenderer(renderer);
    }
}
