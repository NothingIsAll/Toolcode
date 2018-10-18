#ifndef NATIVERENDER_H_
#define NATIVERENDER_H_
#include "Common.h"
#include <android/bitmap.h>
#include <GLES/gl.h>


namespace room {
class NativeRender {
private:
	/**
	 * 是否有新的数据
	 * 0无数据,1图片rgba数据,2摄像头rgb数据
	 */
	int dataState;
	/**
	 * 定义顶点时使用
	 */
	float one;
	/**
	 * 只有一张图,也就是一个纹理
	 */
	unsigned int vbo[1];
	/**
	 * 正方形顶点
	 */
	float vertices[12];
	/**
	 * 纹理点
	 */
	float texCoords[8];

	/**
	 * rgb图片宽度
	 */
	int bitmapWidth;
	/**
	 * rgb图片高度
	 */
	int bitmapHeight;

	/**
	 * rgb数据
	 */
	unsigned char* data;

	/**
	 * rgb锁
	 */
	pthread_mutex_t rgbMutex;

	void scale(int w, int h, const char* rgb, int tow, int toh, char* torgb, int pixPerChar);
	void scaleF(int w, int h, const char* rgb, int tow, int toh, char* torgb);

public:
	void onSurfaceCreated();
	void onSurfaceChanged(int width, int height);
	void onDrawFrame();
	void onDrawFrame1();
	void setBitmap(int w, int h, const char * rgba);
	void setBitmap1(AndroidBitmapInfo info, void* bitmap);
	void setRGB(int w, int h, const char * rgb);
	NativeRender();
	~NativeRender();
};
}
extern room::NativeRender glrender;
#endif /* NATIVERENDER_H_ */
