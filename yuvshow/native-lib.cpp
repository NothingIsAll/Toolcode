#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <android/log.h>
#include "NativeRender.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/bitmap.h>

#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"YUVSHOW",FORMAT,##__VA_ARGS__);
using namespace room;
NativeRender glrender;

#define JAVA_CLASS_NAME "com/audiocn/libs/CamShowGlRender"

extern "C"{


	 jstring stringFromJNI(JNIEnv *env, jclass type){
		return  env->NewStringUTF("hello mrfan iam come back!!");
	}


	 void onSurfaceCreated(JNIEnv *env, jclass type){
		glrender.onSurfaceCreated();
	}


	 void onSurfaceChanged(JNIEnv *env, jclass type,jint width,jint heigh){
		glrender.onSurfaceChanged(width,heigh);
	}


	 void onDrawFrame(JNIEnv *env, jclass type){
		glrender.onDrawFrame();
	}


	 jint PutYuvData(JNIEnv *env, jclass type,jbyteArray data,jint size,jint width,jint heigh){
		char *buf = (char*) env->GetByteArrayElements(data, JNI_FALSE);
		int res = glrender.setYUV420N21(width,heigh,buf,size);
		env->ReleaseByteArrayElements(data, (jbyte*)buf, JNI_FALSE);
		return res;
	}


	static JNINativeMethod getMethods[] = {
	        {"stringFromJNI","()Ljava/lang/String;",(void*)stringFromJNI},
			{"onSurfaceCreated","()V",(void*)onSurfaceCreated},
			{"onSurfaceChanged","(II)V",(void*)onSurfaceChanged},
			{"onDrawFrame","()V",(void*)onDrawFrame},
			{"PutYuvData","([BIII)I",(void*)PutYuvData}
	};

	//此函数通过调用JNI中 RegisterNatives 方法来注册我们的函数
	static int registerNativeMethods(JNIEnv* env, const char* className,JNINativeMethod* getMethods,int methodsNum){
	    jclass clazz;
	    //找到声明native方法的类
	    clazz = env->FindClass(className);
	    if(clazz == NULL){
	        return JNI_FALSE;
	    }
	   //注册函数 参数：java类 所要注册的函数数组 注册函数的个数
	    if(env->RegisterNatives(clazz,getMethods,methodsNum) < 0){
	        return JNI_FALSE;
	    }
	    return JNI_TRUE;
	}

	static int registerNatives(JNIEnv* env){
	    //指定类的路径，通过FindClass 方法来找到对应的类
	    const char* className  = JAVA_CLASS_NAME;
	    return registerNativeMethods(env,className,getMethods, sizeof(getMethods)/ sizeof(getMethods[0]));
	}
	//回调函数 在这里面注册函数
	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved){
	    JNIEnv* env = NULL;
	   //判断虚拟机状态是否有问题
	    if(vm->GetEnv((void**)&env,JNI_VERSION_1_6)!= JNI_OK){
	        return -1;
	    }
	    //开始注册函数 registerNatives -》registerNativeMethods -》env->RegisterNatives
	    if(!registerNatives(env)){
	        return -1;
	    }
	    //返回jni 的版本
	    return JNI_VERSION_1_6;
	}

}
