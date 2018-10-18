#include "NativeRender.h"
using namespace room;
NativeRender::NativeRender() {
	one = 1.0f;
	dataState = 0;
	bitmapWidth = KROOM_VIDEO_WIDTH;
	bitmapHeight = KROOM_VIDEO_HEIGHT;
	data = (unsigned char*) malloc(bitmapWidth * bitmapHeight * 4);
	float pos[12] = { one, one, 0, // 右上
			one, -one, 0, // 右下
			-one, one, 0, // 左上
			-one, -one, 0 // 左下
			};
	//不做转向,针对横屏
//	float ins[8] = { 0, one, 0, 0, one, one, one, 0 };

	//转向逆时针90度,针对竖屏
//	float ins[8] = { 0, 0, one, 0, 0, one, one, one };

	//转向逆时针180度,针对竖屏
	float ins[8] = { one, 0, one, one, 0, 0, 0, one };

	memcpy(vertices, pos, 12 * sizeof(float));
	memcpy(texCoords, ins, 8 * sizeof(float));

	pthread_mutex_init(&rgbMutex, NULL);
}
void NativeRender::onSurfaceCreated() {
	LOGD("NativeRender::onSurfaceCreated");
	// 告诉系统对透视进行修正
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	// 黑色背景
	glClearColor(0, 0, 0, 0);
	// 启用阴影平滑
	glShadeModel(GL_SMOOTH);

	// 清除深度缓存
	glClearDepthf(one);
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	// 所做深度测试的类型
	glDepthFunc(GL_LEQUAL);

	// 创建纹理
	glGenTextures(1, vbo);
	// 绑定要使用的纹理
	glBindTexture(GL_TEXTURE_2D, vbo[0]);
	// 生成纹理
	//GLUtils.texImage2D(GL_TEXTURE_2D, 0, bitmap, 0);
	// 线性滤波

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
void NativeRender::onSurfaceChanged(GLint width, GLint height) {
	LOGD("NativeRender::onSurfaceChanged.width=%d,height=%d", width, height);
	//图形最终显示到屏幕的区域的位置、长和宽
	glViewport(0, 0, width, height);
	//float ratio = (float) width / height;
	//glFrustumf(-ratio, ratio, -1, 1, 1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void NativeRender::onDrawFrame() {
	// 清除深度和颜色缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	pthread_mutex_lock(&rgbMutex);
	if (dataState == 1) {
		// 生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmapWidth, bitmapHeight, 0,
					 GL_RGBA, GL_UNSIGNED_BYTE, data);
		//LOGD("NativeRender::onDrawFrame.dataState=%d", dataState);
		dataState = 0;
	} else if (dataState == 2) {
		// 生成纹理

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapWidth, bitmapHeight, 0,
					 GL_RGB, GL_UNSIGNED_BYTE, data);
		//LOGD("NativeRender::onDrawFrame.dataState=%d", dataState);
		dataState = 0;
	}
	pthread_mutex_unlock(&rgbMutex);
	// 开启顶点和纹理缓冲
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//开启纹理
	glEnable(GL_TEXTURE_2D);
	// 设置点点和纹理
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

	// 向z轴里移入
	//glTranslatef(0.0f, 0.0f, -2.0f);
	// 设置3个方向的旋转
//	glRotatef(0, one, 0.0f, 0.0f);
//	glRotatef(0, 0.0f, one, 0.0f);
//	glRotatef(0, 0.0f, 0.0f, one);

	// 绘制图形
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
}
void NativeRender::onDrawFrame1() {
	// 清除深度和颜色缓存
	//LOGE("----------------- linenum = %d  funname = %s",__LINE__,__FUNCTION__);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//LOGE("----------------- linenum = %d  funname = %s",__LINE__,__FUNCTION__);
	glLoadIdentity();
	//LOGE("----------------- linenum = %d  funname = %s",__LINE__,__FUNCTION__);
	pthread_mutex_lock(&rgbMutex);
	//LOGE("----------------- linenum = %d  funname = %s",__LINE__,__FUNCTION__);
	if (dataState == 1) {
		// 生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmapWidth, bitmapHeight, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, data);
		//LOGD("NativeRender::onDrawFrame.dataState=%d", dataState);
		dataState = 0;
	} else if (dataState == 2) {
		// 生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapWidth, bitmapHeight, 0,
				GL_RGB, GL_UNSIGNED_BYTE, data);
		//LOGD("NativeRender::onDrawFrame.dataState=%d", dataState);
		dataState = 0;
	}
	//LOGE("----------------- linenum = %d  funname = %s",__LINE__,__FUNCTION__);
	pthread_mutex_unlock(&rgbMutex);
	//LOGE("----------------- linenum = %d  funname = %s",__LINE__,__FUNCTION__);
	// 开启顶点和纹理缓冲
	glEnableClientState(GL_VERTEX_ARRAY);
	//LOGE("----------------- linenum = %d  funname = %s",__LINE__,__FUNCTION__);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//开启纹理
	//LOGE("----------------- linenum = %d  funname = %s",__LINE__,__FUNCTION__);
	glEnable(GL_TEXTURE_2D);
	// 设置点点和纹理
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	//LOGE("----------------- linenum = %d  funname = %s",__LINE__,__FUNCTION__);
	// 向z轴里移入
	//glTranslatef(0.0f, 0.0f, -2.0f);
	// 设置3个方向的旋转
//	glRotatef(0, one, 0.0f, 0.0f);
//	glRotatef(0, 0.0f, one, 0.0f);
//	glRotatef(0, 0.0f, 0.0f, one);

	// 绘制图形
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	//LOGE("----------------- linenum = %d  funname = %s",__LINE__,__FUNCTION__);
}
void NativeRender::setBitmap1(AndroidBitmapInfo info, void* bitmap) {
	pthread_mutex_lock(&rgbMutex);
	bitmapWidth = info.width;
	bitmapHeight = info.height;
	dataState = 1;
	memcpy(data, bitmap, bitmapWidth * bitmapHeight * 4);
	pthread_mutex_unlock(&rgbMutex);
}
void NativeRender::setBitmap(int w, int h, const char * rgba) {
	pthread_mutex_lock(&rgbMutex);
	memcpy(data, rgba, w * h * 4);
	pthread_mutex_unlock(&rgbMutex);
}
void NativeRender::setRGB(int w, int h, const char * rgb) {
	if (dataState != 0) { //正在播放,扔掉本帧
		LOGD("正在播放,扔掉本帧");
		return;
	}
#ifdef OPENGL_COMP
	int dstW = 512;
	int dstH = 256;
	int len = dstW * dstH * 3;
	char rgbTemp[len];
	scale(w, h, rgb, dstW, dstH, rgbTemp, 3);
	pthread_mutex_lock(&rgbMutex);
	bitmapWidth = dstW;
	bitmapHeight = dstH;
	dataState = 2;
	memcpy(data, rgbTemp, bitmapWidth * bitmapHeight * 3);
	pthread_mutex_unlock(&rgbMutex);
#else
	pthread_mutex_lock(&rgbMutex);
	bitmapWidth = w;
	bitmapHeight = h;
	dataState = 2;
	memcpy(data, rgb, bitmapWidth * bitmapHeight * 3);
	//memcpy(data, rgb, bitmapWidth * bitmapHeight );
	pthread_mutex_unlock(&rgbMutex);
#endif
}
void NativeRender::scale(int srcW, int srcH, const char *src, int dstW,
		int dstH, char *dst, int bytesPerPixel) {
	uint32_t scaleX = (srcW << 16) / dstW, scaleY = (srcH << 16) / dstH;
	uint16_t scaleX_i = (scaleX >> 16) & 0xffff, scaleX_f = scaleX & 0xffff;
	uint16_t scaleY_i = (scaleY >> 16) & 0xffff, scaleY_f = scaleY & 0xffff;
	uint16_t x0, y0;
	uint16_t wX, wY;
	size_t index00, index01, index10, index11;
	uint64_t c00, c01, c10, c11;
	uint64_t one = 1 << 16;

	int n = bytesPerPixel;

	for (int i = 0; i < dstH; ++i) {
		uint32_t t = i * (uint32_t) scaleY_f;
		y0 = i * scaleY_i + (t >> 16);
		wY = t & 0xffff;
		for (int j = 0; j < dstW; ++j) {
			uint32_t s = j * (uint32_t) scaleX_f;
			x0 = j * scaleX_i + (s >> 16);
			wX = s & 0xffff;
			index00 = (y0 * srcW + x0) * n;
			if (x0 + 1 < srcW && y0 + 1 < srcH) {
				index01 = index00 + n;
				index10 = index00 + n * srcW;
				index11 = index10 + n;
				c00 = (one - wX) * (one - wY);
				c01 = wX * (one - wY);
				c10 = (one - wX) * wY;
				c11 = (uint64_t) 1 * wX * wY;
				for (int k = 0; k < n; ++k) {
					dst[(i * dstW + j) * n + k] = (c00 * src[index00 + k]
							+ c01 * src[index01 + k] + c10 * src[index10 + k]
							+ c11 * src[index11 + k]) >> 32;
				}
			} else if (x0 + 1 < srcW) {
				index01 = index00 + n;
				c00 = (one - wX);
				c01 = wX;
				for (int k = 0; k < n; ++k) {
					dst[(i * dstW + j) * n + k] = (c00 * src[index00 + k]
							+ c01 * src[index01 + k]) >> 16;
				}
			} else if (y0 + 1 < srcH) {
				index10 = index00 + n * srcW;
				c00 = (one - wY);
				c10 = wY;
				for (int k = 0; k < n; ++k) {
					dst[(i * dstW + j) * n + k] = (c00 * src[index00 + k]
							+ c10 * src[index10 + k]) >> 16;
				}
			} else {
				for (int k = 0; k < n; ++k) {
					dst[(i * dstW + j) * n + k] = src[index00 + k];
				}
			}
		}
	}
}
void NativeRender::scaleF(int srcW, int srcH, const char *src, int dstW,
		int dstH, char *dst) {
	float scaleX = (srcW * 1.0) / (dstW * 1.0);
	float scaleY = (srcH * 1.0) / (dstH * 1.0);
	for (int i = 0; i < dstH; ++i) {
		for (int j = 0; j < dstW; ++j) {
			float x = j * scaleX, y = i * scaleY;
			uint16_t x0 = x, y0 = y;
			float wX = x - x0, wY = y - y0;
			size_t index00 = (y0 * srcW + x0) * 3;
			size_t index10, index01, index11;
			if (x0 + 1 < srcW && y0 + 1 < srcH) {
				index01 = index00 + 3;
				index10 = index00 + 3 * srcW;
				index11 = index10 + 3;
				float c00 = (1 - wX) * (1 - wY), c01 = wX * (1 - wY), c10 = (1
						- wX) * wY, c11 = wX * wY;
				for (int k = 0; k < 3; ++k) {
					dst[(i * dstW + j) * 3 + k] = c00 * src[index00 + k]
							+ c01 * src[index01 + k] + c10 * src[index10 + k]
							+ c11 * src[index11 + k];
				}
			} else if (x0 + 1 < srcW) {
				index01 = index00 + 3;
				float c00 = 1 - wX, c01 = wX;
				for (int k = 0; k < 3; ++k) {
					dst[(i * dstW + j) * 3 + k] = c00 * src[index00 + k]
							+ c01 * src[index01 + k];
				}
			} else if (y0 + 1 < srcH) {
				index10 = index00 + 3 * srcW;
				float c00 = 1 - wY, c10 = wY;
				for (int k = 0; k < 3; ++k) {
					dst[(i * dstW + j) * 3 + k] = c00 * src[index00 + k]
							+ c10 * src[index10 + k];
				}
			} else {
				for (int k = 0; k < 3; ++k) {
					dst[(i * dstW + j) * 3 + k] = src[index00 + k];
				}
			}
		}
	}
}
NativeRender::~NativeRender() {
	free(data);
	pthread_mutex_destroy(&rgbMutex);
}
