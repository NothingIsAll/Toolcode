#ifndef NATIVERENDER_H_
#define NATIVERENDER_H_
#include "Common.h"
#include <android/bitmap.h>
#include <GLES/gl.h>
extern "C" {
#include "libyuv.h"
}

namespace room {
class NativeRender {
private:
	/**
	 * 鏄惁鏈夋柊鐨勬暟鎹�
	 * 0鏃犳暟鎹�,1鍥剧墖rgba鏁版嵁,2鎽勫儚澶磖gb鏁版嵁
	 */
	int dataState;
	/**
	 * 瀹氫箟椤剁偣鏃朵娇鐢�
	 */
	float one;
	/**
	 * 鍙湁涓�寮犲浘,涔熷氨鏄竴涓汗鐞�
	 */
	unsigned int vbo[1];
	/**
	 * 姝ｆ柟褰㈤《鐐�
	 */
	float vertices[12];
	/**
	 * 绾圭悊鐐�
	 */
	float texCoords[8];

	/**
	 * rgb鍥剧墖瀹藉害
	 */
	int bitmapWidth;
	/**
	 * rgb鍥剧墖楂樺害
	 */
	int bitmapHeight;

	/**
	 * rgb鏁版嵁
	 */
	unsigned char* data;

	/**
	 * rgb閿�
	 */
	pthread_mutex_t rgbMutex;

	void scale(int w, int h, const char* rgb, int tow, int toh, char* torgb, int pixPerChar);
	void scaleF(int w, int h, const char* rgb, int tow, int toh, char* torgb);

public:
	void onSurfaceCreated();
	void onSurfaceChanged(int width, int height);
	void onDrawFrame();
	void setBitmap(AndroidBitmapInfo info, void* bitmap);
	int setRGB(int w, int h, const char * rgb);
	int setYUV420N21(int width,int heigh,char* buf,int size);
	NativeRender();
	~NativeRender();
};
}
extern room::NativeRender glrender;
#endif /* NATIVERENDER_H_ */
