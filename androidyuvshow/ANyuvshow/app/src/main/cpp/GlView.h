//
// Created by huizai on 2017/11/24.
//
#ifndef FFMPEG_DEMO_GLVIEW_H
#define FFMPEG_DEMO_GLVIEW_H

#include "JniDefine.h"


void SurfaceCreated();
void SurfaceChanged(int width,int heigh);
void DrawFrame();
int setYUV420N21(int width,int heigh, char* buf,int size);
//==============
void setVideoSize(int width,int height);
void initGL();
/**
 清除画面
 */
void clearFrame();
void Render();
void SetupYUVTextures();


#endif