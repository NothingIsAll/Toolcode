#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <android/log.h>
#include "NativeRender.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/bitmap.h>

extern "C" {

#include "libyuv.h"
}

#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",FORMAT,##__VA_ARGS__);
//16bit 44100 PCM 数据大小
#define MAX_AUDIO_FRME_SIZE 44100 * 2

using namespace room;


extern "C" JNIEXPORT jstring Java_entry_com_audiocn_tlkg_opengltest_NativeFfmpegGL_stringFromJNI(JNIEnv *env, jclass type){
    return  env->NewStringUTF("hello mrfan iam come back!!");
}

NativeRender glrender;

#define BUFFERSIZE 1280*720

extern "C"
JNIEXPORT void JNICALL
Java_entry_com_audiocn_tlkg_opengltest_NativeFfmpegGL_video2RGBGL(JNIEnv *env, jclass type, jstring input_path_ ) {
    LOGE("----------------- linenum = %d  funname = %s",__LINE__,__FUNCTION__);
    int width = 1280;//720;//1280;
    int height = 720;//480;//720;

    FILE* file = fopen("/data/camera.yuv","rb+");
    if (!file){
        LOGE("==================fopen error!!!==============");
        return;
    }
    char* buf_nv21y = (char*)malloc(BUFFERSIZE*2);
    char* buf_nv21uv =(char*)malloc(BUFFERSIZE*2);//(char*)malloc(BUFFERSIZE*2);
    char* bufy = (char*)malloc(BUFFERSIZE*2);//[BUFFERSIZE]={};
    char* bufu = (char*)malloc(BUFFERSIZE*2);//[BUFFERSIZE]={};
    char* bufv = (char*)malloc(BUFFERSIZE*2);//[BUFFERSIZE] ={};
    char* buf_rgb = (char*)malloc(BUFFERSIZE*4);
    int resread = 1;
    int alignsize_y = width + width%32;
    int alignsize_u = alignsize_y/2;
    int alignsize_v = alignsize_u;
    while (resread >0) {
        resread = fread(buf_nv21y,1,BUFFERSIZE,file);
        resread = fread(buf_nv21uv,1,BUFFERSIZE/2,file);

        libyuv::NV21ToI420((uint8*)buf_nv21y,alignsize_y,
                           (uint8*)buf_nv21uv,alignsize_y,
                           (uint8*)bufy,alignsize_y,
                           (uint8*)bufv,alignsize_v,
                           (uint8*)bufu,alignsize_u,
                           width,height);
        libyuv::I420ToRGB24((uint8*)bufy, alignsize_y,
                            (uint8*)bufu, alignsize_u,
                            (uint8*)bufv, alignsize_v,
                            (uint8*)buf_rgb, width*3,
                            width, height);

        glrender.setRGB(width,height,(const char*)buf_rgb);

    }

}


extern "C"
JNIEXPORT void JNICALL
        Java_entry_com_audiocn_tlkg_opengltest_demoglRender_onSurfaceCreated(JNIEnv *env, jclass type){
    glrender.onSurfaceCreated();
}

extern "C"
JNIEXPORT void JNICALL
Java_entry_com_audiocn_tlkg_opengltest_demoglRender_onSurfaceChanged(JNIEnv *env, jclass type,jint width,jint heigh){
    glrender.onSurfaceChanged(width,heigh);
}

extern "C"
JNIEXPORT void JNICALL
Java_entry_com_audiocn_tlkg_opengltest_demoglRender_onDrawFrame(JNIEnv *env, jclass type){
    glrender.onDrawFrame();
}