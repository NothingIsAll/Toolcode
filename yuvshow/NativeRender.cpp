#include "NativeRender.h"
using namespace room;
NativeRender::NativeRender() {
	one = 1.0f;
	dataState = 0;
	bitmapWidth = KROOM_VIDEO_WIDTH;
	bitmapHeight = KROOM_VIDEO_HEIGHT;
	data = (unsigned char*) malloc(bitmapWidth * bitmapHeight * 4);
	float pos[12] = { one, one, 0, // 鍙充笂
			one, -one, 0, // 鍙充笅
			-one, one, 0, // 宸︿笂
			-one, -one, 0 // 宸︿笅
			};
	//涓嶅仛杞悜,閽堝妯睆
//	float ins[8] = { 0, one, 0, 0, one, one, one, 0 };

	//杞悜閫嗘椂閽�90搴�,閽堝绔栧睆
//	float ins[8] = { 0, 0, one, 0, 0, one, one, one };

	//杞悜閫嗘椂閽�180搴�,閽堝绔栧睆
	float ins[8] = { one, 0, one, one, 0, 0, 0, one };

	memcpy(vertices, pos, 12 * sizeof(float));
	memcpy(texCoords, ins, 8 * sizeof(float));

	pthread_mutex_init(&rgbMutex, NULL);
}
void NativeRender::onSurfaceCreated() {
	LOGD("NativeRender::onSurfaceCreated");
	// 鍛婅瘔绯荤粺瀵归�忚杩涜淇
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	// 榛戣壊鑳屾櫙
	glClearColor(0, 0, 0, 0);
	// 鍚敤闃村奖骞虫粦
	glShadeModel(GL_SMOOTH);

	// 娓呴櫎娣卞害缂撳瓨
	glClearDepthf(one);
	// 鍚敤娣卞害娴嬭瘯
	glEnable(GL_DEPTH_TEST);
	// 鎵�鍋氭繁搴︽祴璇曠殑绫诲瀷
	glDepthFunc(GL_LEQUAL);

	// 鍒涘缓绾圭悊
	glGenTextures(1, vbo);
	// 缁戝畾瑕佷娇鐢ㄧ殑绾圭悊
	glBindTexture(GL_TEXTURE_2D, vbo[0]);
	// 鐢熸垚绾圭悊
	//GLUtils.texImage2D(GL_TEXTURE_2D, 0, bitmap, 0);
	// 绾挎�ф护娉�

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
void NativeRender::onSurfaceChanged(GLint width, GLint height) {
	LOGD("NativeRender::onSurfaceChanged.width=%d,height=%d", width, height);
	//鍥惧舰鏈�缁堟樉绀哄埌灞忓箷鐨勫尯鍩熺殑浣嶇疆銆侀暱鍜屽
	glViewport(0, 0, width, height);
	//float ratio = (float) width / height;
	//glFrustumf(-ratio, ratio, -1, 1, 1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void NativeRender::onDrawFrame() {
	// 娓呴櫎娣卞害鍜岄鑹茬紦瀛�
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	pthread_mutex_lock(&rgbMutex);
	if (dataState == 1) {
		// 鐢熸垚绾圭悊
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmapWidth, bitmapHeight, 0,
					 GL_RGBA, GL_UNSIGNED_BYTE, data);
		//LOGD("NativeRender::onDrawFrame.dataState=%d", dataState);
		dataState = 0;
	} else if (dataState == 2) {
		// 鐢熸垚绾圭悊

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapWidth, bitmapHeight, 0,
					 GL_RGB, GL_UNSIGNED_BYTE, data);
		//LOGD("NativeRender::onDrawFrame.dataState=%d", dataState);
		dataState = 0;
	}
	pthread_mutex_unlock(&rgbMutex);
	// 寮�鍚《鐐瑰拰绾圭悊缂撳啿
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//寮�鍚汗鐞�
	glEnable(GL_TEXTURE_2D);
	// 璁剧疆鐐圭偣鍜岀汗鐞�
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

	// 鍚憐杞撮噷绉诲叆
	//glTranslatef(0.0f, 0.0f, -2.0f);
	// 璁剧疆3涓柟鍚戠殑鏃嬭浆
//	glRotatef(0, one, 0.0f, 0.0f);
//	glRotatef(0, 0.0f, one, 0.0f);
//	glRotatef(0, 0.0f, 0.0f, one);

	// 缁樺埗鍥惧舰
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	//LOGE("----------------- linenum = %d  funname = %s",__LINE__,__FUNCTION__);
}

void NativeRender::setBitmap(AndroidBitmapInfo info, void* bitmap) {
	pthread_mutex_lock(&rgbMutex);
	bitmapWidth = info.width;
	bitmapHeight = info.height;
	dataState = 1;
	memcpy(data, bitmap, bitmapWidth * bitmapHeight * 4);
	pthread_mutex_unlock(&rgbMutex);
}

int NativeRender::setRGB(int w, int h, const char * rgb) {
	if (dataState != 0) { //姝ｅ湪鎾斁,鎵旀帀鏈抚
		LOGD("姝ｅ湪鎾斁,鎵旀帀鏈抚");
		return -1;
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
	return 0;
#else
	pthread_mutex_lock(&rgbMutex);
	bitmapWidth = w;
	bitmapHeight = h;
	dataState = 2;
	memcpy(data, rgb, bitmapWidth * bitmapHeight * 3);
	//memcpy(data, rgb, bitmapWidth * bitmapHeight );
	pthread_mutex_unlock(&rgbMutex);
	return 0;
#endif
}

int NativeRender::setYUV420N21(int width,int heigh,char* buf,int size){

	int bufsize_n21y = width*heigh*2;
	int bufsize_n21uv = bufsize_n21y;
	int realsize_n21y = width*heigh;
	int realsize_n21uv = realsize_n21y/2;
	int realwidth_y = width + width%32;
	int realwidth_uv = realwidth_y;
	int realwidth_u = realwidth_y/2;
	int realwidth_v = realwidth_u;
	char* bufn21y = (char*) malloc(bufsize_n21y);
	char* bufn21uv = (char*)malloc(bufsize_n21uv);
	int bufsize_420 =  bufsize_n21y;
	char* buf_y = (char*)malloc(bufsize_420);
	char* buf_u = (char*)malloc(bufsize_420);
	char* buf_v = (char*)malloc(bufsize_420);
	char* buf_rgb = (char*)malloc(bufsize_420*2);
	memcpy(bufn21y,buf,realsize_n21y);
	memcpy(bufn21uv,buf+realsize_n21y,realsize_n21uv);

    libyuv::NV21ToI420((uint8*)bufn21y,realwidth_y,
                       (uint8*)bufn21uv,realwidth_y,
                       (uint8*)buf_y,realwidth_y,
                       (uint8*)buf_v,realwidth_u,
                       (uint8*)buf_u,realwidth_v,
                       width,heigh);
    libyuv::I420ToRGB24((uint8*)buf_y, realwidth_y,
                        (uint8*)buf_u, realwidth_u,
                        (uint8*)buf_v, realwidth_v,
                        (uint8*)buf_rgb, width*3,
                        width, heigh);

    int res = setRGB(width,heigh,buf_rgb);

    free(bufn21y);
    free(bufn21uv);
    free(buf_y);
    free(buf_u);
    free(buf_v);
    free(buf_rgb);
    return res;
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
