//
//  RoomCommand.h
//  AndroidRoom
//
//  Created by jiabin on 14-3-5.
//  Copyright (c) 2014年 jiabin. All rights reserved.
//

#ifndef AndroidRoom_RoomCommand_h
#define AndroidRoom_RoomCommand_h

#include <pthread.h>
#include <stdio.h>
#include <jni.h>
#include "NativeRender.h"


typedef unsigned char GUID[32];
#define TEXTBUFSIZE  2000
#define MEDIABUFSIZE  1000 * 5      //大概5秒钟的尺寸。
#define NICK_BUFFER_SIZE 200
#define MAX_AFRAME_NUM     512      //最大缓存音频帧数量
#define MAX_TXT_NUM 200             //最大缓存文本纪录数量
#define MAX_SEND_BUF_SIZE  20000*20 //意义：2万个用户的房间，当下载所有用户信息数据到量。20：指ST_AddUser的大小。
#define KROOM_SAMPLES_RATE 44100 //采样率
#define KROOM_DECODE_CHANNELS 2  //声道数,faad2暂时只能解码成双声道
#define KROOM_ENCODE_CHANNELS 1  //声道数
#define KROOM_VIDEO_WIDTH 1280;//1280;//720;//480  //视频宽度
#define KROOM_VIDEO_HEIGHT 720;//720;//480;//360 //视频高度
#define PREVIEW_WIDTH  640  //预览视频宽度
#define PREVIEW_HEIGHT 480	//预览视频高度
#define VIDEO_FRAME_RATE 10 //视频帧率
#define VIDEO_BIT_RATE 250 //视频码率
#define AUDIO_BIT_RATE 40 //音频码率
//#define OPENGL_COMP //是否启用2的n次方兼容
#define KROOM_VEDIO_FRAME_LEN (KROOM_VIDEO_WIDTH * KROOM_VIDEO_HEIGHT * 3) //视频容量
#define MONITOR_READSIZE 512 
#define MONITOR_WRITESIZE (MONITOR_READSIZE * 2)
/**
 * mv 制作视频编码参数
 */
#define MV_BIT_RATE 		500 		//MV视频码率
#define MV_FRAME_RATE 	10 		//MV视频帧率

#define IS_DEBUG 		1
#define Debug_Mode    	1 /*是否打印log*/

typedef enum {
	SEND = 1,
	RECV = 2
} SEND_OR_RECV;

//交互命令集合(目前命令可能不全，先完成骨干命令，实现一对多通话，以后要考虑每个服务器预留指令空间为以后好扩充）

typedef enum {

    /********************************** 客户端与代理服务器交互集合**********************************************/
    //C2P
    C2P_LOGIN                      =   0,                //登录代理服务器命令
    C2P_ASK_FOR_SING   =   1,           //申请排麦
    C2P_GIVE_UP_SING  =   2,           //切麦命令
    C2P_DELETE_SING   =   3,            //删麦命令
    C2P_PRI_SING	= 	4,			//优先麦序
    C2P_GET_MAI_LIST	=	5,			//获取麦序列表
    C2P_TCP_HEART_BEAT   = 10,
    C2P_KICK_OUT,
    C2P_SEND_GIFT,
    C2P_UDP_TEXT,
    C2P_PRIVATE_TEXT,
    C2P_GET_USER_LIST,
    C2P_LOGOUT,
    C2P_GET_SERVICE,                //获取服务器
    C2P_SET_USER_RIGHT,

    //P2C
    P2C_LOGIN_RESP          =   50,             //登录房间相应命令
    P2C_ASK_FOR_SING_RESP  =  C2P_ASK_FOR_SING + 50,  //排麦响应命令
    P2C_GIVE_UP_SING_RESP  =  C2P_GIVE_UP_SING + 50,  //切麦命令响应
    P2C_DELETE_SING_RESP  =  C2P_DELETE_SING + 50,  //删除麦响应命令
    P2C_PRI_SING_RESP   = 	C2P_PRI_SING + 50,	//更改麦序响应
    P2C_GET_MAI_LIST_RESP	=   C2P_GET_MAI_LIST + 50,	//麦序列表
    P2C_START_SING          = 60,             //上麦通知给歌手
    P2C_START_SING_VIEWER,      //上麦通知给观众
    P2C_TCP_HEART_BEAT_RESP,    //tcp心跳响应
    P2C_KICK_OUT_RESP,          //踢人响应
    P2C_KICK_OUT_BY_OWNER,      //被踢用户提示
    P2C_SEND_GIFT_RESP,         //送礼物响应
    P2C_SEND_GIFT,          //送礼物指令
    P2C_PRIVATE_TEXT,       //悄悄说
    P2C_GET_USER_LIST_RESP,     //观众列表
    P2C_LOGOUT,             //退出
    P2C_TCP_SYS_MSG     = 70,    //系统公告
    P2C_MAI_NOTICE,
    P2C_USER_RELOGIN,  //重复登录
    P2C_USER_LOGOUT,
    P2C_ROOM_NOTICE,   //房间公告
    P2C_GET_SERVICE_RESP,               // 获取服务器响应
    P2C_GIVE_UP_NOTICE,         //下麦通知
    P2C_TXT_ERR_RESP,       //悄悄说响应
    P2C_USERNUMBER,
    P2C_SET_USER_RIGHT_RESP = 79,  // 给设置的人发的命令
    P2C_SET_USER_RIGHT_TOUSER = 80,  // 给被设置的人发的命令


    /**********************************客户端与流媒体上传服务器交互集合********************************************/
    //C2TMS
    C2TMS_LOGIN   =   100,                //登录流媒体上传服务器,此命令已废弃
    C2TMS_LOGOUT,
    C2TMS_AUDIO_DATA,           //音频数据上传命令
    C2TMS_VIDEO_DATA,           //视频数据上传命令
    C2TMS_START_SING,           //上麦请求命令
    C2TMS_TCP_HEART_BEAT,
    //TMS2C
    TMS2C_START_SING_RESP   =   150,          //上麦请求响应命令
    TMS2C_TCP_HEART_BEAT_RESP,

    /*********************************客户端与流媒体下行服务器交互集合*********************************************/
    //C2UMS
    C2UMS_LOGIN   =   200,         //登录流媒体下行服务器
    C2UMS_UDP_HEART_BEAT,
    C2UMS_RECV_MODE,
    //UMS2C
    UMS2C_LOGIN_RESP     =   250,
    UMS2C_AUDIO_DATA,         //数据下行命令
    UMS2C_VIDEO_DATA,
    UMS2C_UDP_HEART_BEAT_RESP,
    UMS2C_RECV_MODE_RESP,
    UMS2C_OFF_LINE,

    /*********************************客户端与补包服务器交互集合*************************************************/
    //C2RMS
    C2RMS_LOGIN    =   300,         //登录流媒体补包服务器
    C2RMS_RESEND_AUDIO_PK,         //补包
    C2RMS_RESEND_VIDEO_PK,
    C2RMS_UDP_HEART_BEAT,


    RMS2C_LOGIN_RESP     =   350,
    RMS2C_UDP_HEART_BEAT_RESP,
    /*********************************客户端与文本上传服务器交互集合*************************************************/
    //C2TTS
    C2TTS_LOGIN    =   400,         //登录文本服务器
    C2TTS_TEXT,         //文本服务器上行命令
    C2TTS_TCP_HEART_BEAT,

    TTS2C_LOGIN_RESP     =   450,
    TTS2C_TCP_HEART_BEAT_RESP,
    TTS2C_LOGIN_NOTIFY,     //用户进入房间通知

    /*********************************客户端与文本下行服务器交互集合*************************************************/
    //C2UTS
    C2UTS_LOGIN     =   500,         //登录文本下行服务器
    C2UTS_UDP_HEART_BEAT,

    UTS2C_LOGIN_RESP     =   550,
    UTS2C_TEXT,
    UTS2C_UDP_HEART_BEAT_RESP,

    /********************************代理与业务服务器交互指令集合***************************************************/
    //P2S
    P2S_LOGIN       =   600,  //用户登录验证命令
    P2S_ASK_FOR_SING,        //验证用户排麦命令
    P2S_LOGOUT,
    P2S_KICK_OUT,

    //S2P
    S2P_LOGIN_RESP  =   650,     //业务返回代理服务器登录验证结果命令
    S2P_ASK_FOR_SING_RESP,       //业务返回代理服务器排麦验证结果命令
    S2P_KICK_USER,               //业务返回踢人指令
    S2P_SEND_GIFT,               //业务向代理 送礼物内容。2014-3-11

    /********************************流媒体上传服务器与代理交互*****************************************************/
    //P2TMS
    P2TMS_START_SING_RESP     =   700,        //上麦验证返回命令
    P2TMS_NEW_MAI_START, // 麦手变化 通知tcp上传服务器maiid
    //TMS2P
    TMS2P_START_SING      =   750,             //上麦验证命令

    /***************************流媒体上传服务器和流媒体下行服务器交互**************************************************/
    U2T_TCP_LOGIN      =   800,

    T2U_START_SING   =   850,

}E_ROOM_CMD;

#pragma pack(1)
/**-----------------------------结构体重新整理-------------------------------**/
//房间类型（技术层面）
enum E_ROOM_TPYE {
	AUDIO = 1, //音频包房
	VIDEO, //视频包房
	SYS_ADUIO, //系统预置
	SYS_VIDEO, //音频预置
};

//房间职能（业务层面）
enum E_ROOM_FUNC {
	SONG = 1, //唱歌，KTV等等
	EDUCATION, //远程教育
	MEETING, //企业或组织会议
	LIVE, //托口秀，产品介绍，新闻发布，直播节目采访等等
};
//房间人数

enum E_ROOM_SIZE {
	SIZE100 = 1, //容纳100人
	SIZE500, //容纳500人
};

//用户权限设置

enum E_USER_RIGHT {
	TOURIST = 0, //游客
	NORMAL, //普通用户
	OWNER, //房主
	ADMIN, //管理员
	LINSTEN_ONLY, //听众用户
	BANNED, //禁言用户
	KICKED, //被踢用户
	Other, //其他
	SU_ADMIN = 10, //超级管理员
};

//服务器功能类型
enum E_SERVICE {
	MEDIA_UP = 1, //流媒体上传
	MEDIA_DOWN = 2, //流媒体下发
	RESEND_PKG = 3, //补包服务器
	TXT_UP = 4, //文本服务器上传
	TXT_DOWN = 5, //文本服务器下发
};




/*
 * 房主信息体
 * */
struct ST_OWER_INFO {
		int m_OwerID; // 房主ID
		char m_OwerName[50]; // 房主名字
		char m_OwerPicURL[100]; // 房主头像
};

/**
 * 存放解码后的pcm,最大4096字节
 */
typedef struct pcm_frame {
	int userid;
	int time;
	int pcmLen;
	char pcmData[4096];
} PFrame;


typedef struct ST_BaseHeader {
		int16_t m_nCmdTpye;
		uint32_t m_nPacketLengh;
		int32_t m_nUserID;
		int32_t m_nRoomID;
		uint8_t m_nVersion_H;
		uint8_t m_nVersion_L;
		int32_t m_nRetain;
} ST_BaseHeader;
/**
 * 服务器登录结构体
 * type         服务器类型
 * serviceid    服务器id
 */
struct ST_SvrLogin {
		ST_BaseHeader baseHeader;
		uint8_t type;
		uint16_t serviceid;
		char ip[16];
		uint16_t ctlPort;
		uint16_t dataPort;
};
/**
 * 请求那一台服务器
 * type = 1 流媒体上传
 * type = 2 流媒体下发
 * type = 3 补包服务器
 * type = 4 文本服务器上传
 * type = 5 文本服务器下发
 */
struct ST_SERVICE {
		ST_BaseHeader baseHeader;
		uint8_t type;							//文档不一致
};

struct ST_SERVICE_RESP {
		ST_BaseHeader baseHeader;
		uint8_t is_ok;
		uint8_t type;
		char serverip[16];
		uint16_t ctlport;				//文档不一致
		uint16_t dataport;
};

/**
 * 用户所拥有得服务器列表
 */
struct ST_userServer {
		uint16_t mediaUpSvr_id;
		uint16_t mediaDownSvr_id;
		uint16_t reSendPkgSvr_id;
		uint16_t txtUpSvr_id;
		uint16_t txtDownSvr_id;
};
/*
 * 麦手信息
 * */

struct ST_ASK_SING_INFO {
		int m_nMaiID;
		int m_nSingId; //歌曲ID
		uint16_t m_upType; //流媒体类型 音频直播或视频直播
		uint16_t m_zhiboType; //直播类型 清唱 伴奏 无限唱
		uint16_t m_songType; //伴奏类型 音频伴奏或视频伴奏
		char songName[100];
		int userId;
		char userName[50];
		char headImg[150];
		uint16_t userRight;
		uint8_t sex;
		char birthday[11];
		uint16_t userLevel;
		uint8_t constellation;
		uint8_t m_qkType;        //0：曲库 1：合唱
};

/*
 *麦序列表
 **/
struct ST_MAI_LIST_INFO {				//不完整
		ST_BaseHeader m_Header;
		int maiCount;
		uint16_t maiInfoLen;
		char data[100];
		//ST_ASK_SING_INFO;
};

/*
 * 用户基本信息
 */
struct ST_USER_BASE_INFO {
		int m_nUserID; //用户ID
		char m_name[50]; //用户名字
		char m_PicURL[150]; //用户头像URL
		short m_nLevel; //权限级别。    1.房主2管理员3普通用户4.听众用户5.禁言用户 10.超级用户
		uint8_t m_sex; //性别
		char m_birthday[11]; //生日
		uint16_t m_nUserlevel; //用户级别，
		uint8_t m_Constellation; //星座
		uint8_t isOnline;	//是否在线
};

/*
 * 观众列表
 */
struct ST_USER_LIST {
		ST_BaseHeader 		baseHeader;
		uint32_t            _audienceNum;
		uint32_t            _touristNum;
		uint16_t            _audienceInfoLen;
		char 				data[100];
		//ST_USER_BASE_INFO
};

struct ST_LogIn {
		ST_BaseHeader m_Header;
		char m_SessionId[32];
};

struct ST_CommonLogInResp {																//不定长！！！
		ST_BaseHeader m_Header;
		char m_logInOK; //1 成功，0 失败。
		uint16_t reasonLen;
		char m_reason[200]; //原#define MAX_SRV_EVENT	100000因
		uint16_t m_heart_invterval; //tcp心跳
};
/*
 * 登录响应命令
 * */
struct ST_LogInResp {																	//不定长！！！  1
		ST_BaseHeader m_Header;
		char m_logInOK; //0 成功，1 失败。
		uint16_t reasonLen;
		char m_reason[200]; //原#define MAX_SRV_EVENT	100000因
		uint16_t m_OwerInfoLen; //房主结构体长度
		ST_OWER_INFO m_OwerInfo; //房主信息
		uint16_t m_userInfoLen; //用户基本信息长度；
		ST_USER_BASE_INFO m_userInfo; //用户基本信息;
		uint16_t m_heart_invterval; //tcp心跳
		char m_sUdpIp[16]; //流媒体udp下行服务器
		uint16_t m_nUdpPort; //udp服务器下发端口（udp）
		uint16_t m_nUdpCtrlSrvPort; //udp服务器控制端口
		char m_sResendSrvIp[16]; //补包服务器ip
		uint16_t m_nResendPort; //补包服务器端口
		char m_sChatTcpSrvIp[16]; //群聊服务器上传ip
		uint16_t m_nChatTcpSrPort; //群聊服务器上传端口
		char m_sChatUdpSrvIp[16]; //群聊服务器下行ip
		uint16_t m_nChatUdpSrvPort; //群聊服务器下行端口

		uint16_t singsLen;
		ST_ASK_SING_INFO singInfo;
};

/*
 * 通知客户端用户上线结构体
 */
struct ST_NOTIFY_USER_BASE_INFO {
		ST_BaseHeader m_Header;
		ST_USER_BASE_INFO m_Info; //用户基本信息
};

struct C2P_ST_ASK_SING_INFO {							//1
		ST_BaseHeader m_Header;
		int m_nSingId; //歌曲ID
		uint16_t 	m_upType; //流媒体类型
		uint16_t 	m_zhiboType; //直播类型
		uint16_t 	m_songType; //伴奏类型
		uint8_t  	type;//表示伴奏是曲库伴奏还是合唱伴奏,手机导入伴奏 （0，1，2）			//文档内没有
		uint16_t		m_nameLen;
		char 		m_name[50];
};

/*
 * 代理返回给上传服务器验证上麦歌手的结果信息体
 * */

struct ST_ASK_SING_INFO_RESP {													//不定长！！！
		ST_BaseHeader m_Header;
		uint8_t is_ok;            //1：成功， 0：失败
		uint16_t resonLen;		//错误原因长度
		char sReason[200];       //如果失败给出原因
};

/*
 * 代理返回用户切麦响应
 * */
struct ST_GIVE_UP_RESP {											//文档内不一致
		ST_BaseHeader m_Header;
		char nResult; //1：#define MAX_SRV_EVENT	100000成功 0：失败
		int maiid;
};

/*
 * 排麦歌手信息体
 **/
struct ST_SingerInfo {
		ST_BaseHeader m_Header;
		char m_data; //json串。
};

/*
 *	系统公告
 **/

struct ST_SysMsg {															//不定长！！！
		ST_BaseHeader m_Header;
		uint16_t m_msgLen;
		char m_data[200];
};

struct ST_GiveUpMaiNotice {
		ST_BaseHeader m_Header;
		uint32_t m_maiId;
};

/*
 *上麦通知在麦歌手的消息体
 **/
struct ST_START_SING_SINGER {

		ST_BaseHeader m_Header;
		char m_sTcpIp[16];
		uint16_t m_nPort;
		int m_nMaiID; //麦ID
		int m_nSingID; //歌曲ID
		uint16_t m_upType; //流媒体类型
		uint16_t m_zhiboType; //直播类型
		uint16_t m_songType; //伴奏类型
};

/*
 * 上麦通知观众消息体
 */
struct ST_START_SING_VIEWER {
		ST_BaseHeader 		m_Header;
//		uint32_t            	m_maiID;
//		uint32_t            	m_songID;
//
//		uint16_t           	m_mediaType;     //1：音频 0：视频
//		uint16_t           	m_liveType;      //1：清唱 2：无限唱
//		uint16_t           	m_accomType;     //1：音频伴奏 2：视频伴奏
//		char					m_songName[100];

		ST_ASK_SING_INFO		m_singerInfo;
};


/**
 * 麦通知消息
 */
struct ST_MAI_NOTIFY {
		int roomId;
		int userId;
		int maiId;
};

/*struct ST_START_SING_VIEWER{
 ST_BaseHeader       m_Header;
 int                 m_nMaiID;//麦ID
 int                 m_nSingID;//歌曲ID
 uint16_t            m_upType;//流媒体类型
 uint16_t            m_zhiboType;//直播类型
 uint16_t            m_songType;//伴奏类型
 uint16_t			m_userInfoLen;//后面用户信息长度
 };*/

/*
 *上麦通知观众的消息体
 **/

/*
 *上麦通知给TCP上传服务器
 **/
struct ST_START_SING {
		ST_BaseHeader m_Header;
		int m_nMaiID; //麦ID

};

/*
 *切麦或删麦
 **/
struct ST_MAI_OPERATE {
		ST_BaseHeader m_Header;
		int m_nMaiID; //麦ID
};

struct ST_SingerRlt {
		ST_BaseHeader m_Header;
		char data[1000]; //业务返回的json数据保存

};

struct ST_pthread {
		void* (*pthreadfun)(void*);
		void * objMgr;
};

struct ST_service {
		ST_BaseHeader m_Header;
		int serviceid;
};

/*
 房间类型 roomType 1：音频包房
 房间类别 roomCate 1：K歌
 房间名称 roomName
 房间大小 roomSize 1:100人
 房间简介 roomDesc
 房间id	roomId
 */
struct ST_CreateRoom {
		E_ROOM_TPYE roomType;
		E_ROOM_FUNC roomCate;
		E_ROOM_SIZE roomSize;
		char roomName[500];
		char roomDesc[1600];
		int roomId;
		int roomOwerID; //房主ID。

};

/*
 *
 * */
struct ST_AddUser {
		int nResult; //1：成功 0：失败
		char sReason[200]; //如果失败给出原因
		int nUserID; //用户ID
		int nRoomID; //房间ID
		char sNickName[200]; //昵称
		char sPortraitUrl[256]; //头像URL
		short nUserRight; //用户权限
		int nTcpSocket; //用户会话到tcp socket

		uint8_t m_sex; //性别
		char m_birthday[10]; //生日
		uint16_t m_nUserlevel; //用户级别，
		uint8_t m_Constellation; //星座

};

struct ST_CreateRoomResp {
		ST_BaseHeader m_baseHeader;
		int m_nCreateOK; //创建成功与否 1 成功  0失败
		char m_sReason[100]; //如果失败给出原因
		int m_nTcpPort; //包房服务端口
		int m_nUdpPort;

};

struct ST_Port {
		int nUdpPort;
		int nTcpPort;
};

struct ST_UserDelState {
		int nWillDel; //0，不需要删除。1 需要删除。
		int nTagTcp; //tcp 数据引擎删除关联 标志位。
		int nTagGetUserList; //获取userlist
};

/*
 *
 * 用户登录退出通知命令结构体
 */
struct ST_USER_LOG_Txt {														//不定长！！！
		ST_BaseHeader baseHeader;
		uint8_t userType;
		uint16_t txtLen;
		char text[200];
};

struct ST_Kick_User {
		ST_BaseHeader baseHeader;
		int kickUserid;
};
//代理发给管理员踢人失败指令。

struct ST_kick_to_user {															//不定长！！！
		ST_BaseHeader baseHeader;
		uint8_t isok;
		uint16_t resonLen;
		char reason[200];
};
//被踢用户客户端

struct ST_kick {																		//不定长！！！
		ST_BaseHeader baseHeader;
		uint16_t txtlen;
		char reason[200];
};

/**
 * 送礼物结构体
 */
struct ST_SEND_GIFT {
		ST_BaseHeader 	baseHeader;
		uint32_t 		toUserId;
		uint32_t 		giftNum;
		uint8_t 			in_pub;
		uint16_t 		gift_id;
};

//送礼物发送失败
struct ST_SEND_GIFT_RESP {												//不定长！！！ 文档严重不一致
		ST_BaseHeader baseHeader;
		uint8_t is_ok;
		uint16_t len;
		char text[200];
};
//房间缓存得当前在迈开始及结束得时间
struct ST_GETEXP {
		int userId;
		int roomId;
		long startTime;
		long endTime;
};

typedef struct ST_PRIVATE_TXT {																		//不定长！！！
		ST_BaseHeader baseHeader;
		uint32_t 		toUserID;
		uint32_t 		nTime;
		uint32_t 		nDataLen;
		char 			data[TEXTBUFSIZE];
		uint16_t 		level;												//后加的
		uint16_t 		len_sndName;
		char 			m_sndName[50];       //用户名字
		uint16_t 		len_sndPicURL;
		char 			m_sndPicURL[150];    //用户头像URL
		uint16_t 		len_rcvName;
		char 			m_rcvName[50];
		uint16_t 		len_rcvPicURL;
		char 			m_rcvPicURL[150];    //用户头像URL

} PRIVATE_TXT;

/**
 * 重连
 */
struct ST_RequestServersResp {														//1
		ST_BaseHeader m_Header;
		char m_IsOK;
		char serverId;
		char serverIp[16];
		uint16_t port;
};

/**
 * 发送文本
 */
typedef struct {																	//不定长！！！
    ST_BaseHeader baseHeader;
    int nTime;
    uint32_t nDataLen;
    char data[TEXTBUFSIZE];
    int16_t level;
    uint16_t len_name;
    char m_name[50]; //用户名字
    uint16_t len_picURL;
    char m_PicURL[150]; //用户头像URL
    uint16_t len_name2; //@谁的昵称长度
    char m_name2[50]; //@谁的昵称
} TxtFrame;

/*
 *  房间人数更新  P2C_USERNUMBER
 */
typedef struct {
	ST_BaseHeader 		baseHeader;
	int 					m_userNumber;
}RoomNumber;

/*
 * 	用户进入房间通知	TTS2C_LOGIN_NOTIFY
 */
typedef struct {
	ST_BaseHeader 		baseHeader;
	int 					nTime;
	uint32_t 			nDataLen;
    char 				data[TEXTBUFSIZE];
    int16_t 				level;
    uint16_t 			len_name;
    char 				m_name[50]; //用户名字
    uint16_t 			len_picURL;
    char 				m_PicURL[150]; //用户头像URL
    uint16_t 			len_name2; //@谁的昵称长度
    char 				m_name2[50]; //@谁的昵称
}LoginNotify;

struct ST_P2C_SEND_GIFT												//后填的
{
	ST_BaseHeader   	baseHeader;
	char            	senderName[50];
	char				senderImgUrl[150];
	uint32_t			receiverUserId;
	char				recevierName[50];
	char				recevierImgUrl[150];
	uint16_t			giftNum;
	uint16_t			giftId;
	char				giftName[50];
	uint8_t			playType;
	uint16_t 		txtLen;
	char 			txt[500];	//变长
	uint16_t		verbTxtLen;
	char			verbTxt[100];	//变长
};

//数据(音频)
typedef struct ST_AFrame {
	ST_BaseHeader baseHeader;
	int32_t nSeqNO;
	int32_t nTime;
	int8_t tag;
	int32_t nMaiId;
	int32_t nDatLen;
	char data[MEDIABUFSIZE];

} AFrame;

//数据(视频)
//typedef struct ST_VFrame {
//	ST_BaseHeader baseHeader;
//	int32_t nSeqNO;
//	int32_t nTime;
//	int8_t subFrameSum;
//	int8_t subSeqNO;
//	int8_t tag;
//	int32_t nMaiId;
//	int16_t offset;
//	char data[KROOM_VEDIO_FRAME_LEN];
//} VFrame;

struct ST_SET_USER_RIGHT{				//C2P_SET_USER_RIGHT
	ST_BaseHeader baseHeader;
	uint32_t toUserId;
	uint16_t userRight;
};

struct	ST_RECV_MODE				//C2UMS_RECV_MODE
{
	ST_BaseHeader baseHeader;
	char recv_mode;
};

/*
 *  补包请求命令
 */
struct ST_ResendCmd {
	ST_BaseHeader baseHeader;
	int num;
};

/*
 * 音频补包说明信息体
 */
struct ST_LostPkInfo {
	int nSourceID; //数据源是哪个用户的
	short nMediaType; //0：音频，1视频。
	int nSeq;
};
/*
 * 视频补包说明信息体
 */
struct ST_LostPkInfo_Vedio {
	int nSourceID; //数据源是哪个用户的
	short nMediaType; //0：音频，1视频。
	int nSeq;
	int8_t nSubSeq; //子序号。
};

/**
 * 管理员设置权限通知	 代理->被设置用户
 */
struct ST_SetUserRightTouser {			//P2C_SET_USER_RIGHT_TOUSER
	ST_BaseHeader baseHeader;
	char flag;
	int userid; //被设置的用户ID
	int user_right; //2管理员3普通用户4.听众用户5.禁言用户6踢出

};
/**
 * 管理员设置权限通知	代理->设置用户
 */
struct ST_SetUserRightResp {				//P2C_SET_USER_RIGHT_RESP
	ST_BaseHeader baseHeader;
	char flag;
	char txt[50];

};

/*
 * 登陆udp服务器响应
 */
struct ST_LoginUdpResp {
	ST_BaseHeader 	baseHeader;
	char 			success;
	char 			txt[200];
	uint16_t        	udpHeartInterval;
};

#pragma pack()

#if IS_DEBUG
#include <android/log.h>
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "NativeRoom", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "NativeRoom", __VA_ARGS__)
#else
#define LOGD(...)
#define LOGE(...)
//#define LOGD(...) do{\
//printf(__VA_ARGS__);\
//	printf("\n");\
//}while(0)
//#define LOGE(...) printf(__VA_ARGS__)
#endif
#endif
