#ifndef _COMMON_STRUCT_NET_
#define _COMMON_STRUCT_NET_
#include "Common.h"
typedef enum _OP_RESULT
{
	OP_OK,                 // 操作成功
	CAM_INIT_ERROR,        // 相机初始化错误
	TIMERQUEUE_INIT_ERROR, // 线程队列初始化失败
	OPEN_SQL_ERROR,
	EXCUTE_SQL_ERROR,
	CAM_EXIST,
	CAM_NOEXIST,
	TASK_EXIST,
	TASK_NOEXIST,
	TRANS_ADD_ERROR,
	REC_READ_ERROR,
}OP_RESULT;
typedef enum _CONNECT_CMD  //CMD
{
	// 回复操作结果
	NET_OP_RESULT,
	// Cam操作
	CAM_ADD=1,
	CAM_ADD_REPLY,
	CAM_EDIT,
	CAM_SELECT,
	CAM_SELECT_REPLY,
	CAM_DELETE,
	// 任务操作
	TASK_ADD,
	TASK_START,
	TASK_STOP,
	TASK_SELECT,
	TASK_SELECT_REPLY,
	TASK_DELETE,
	// 视频数据操作
	VIDEO_START,
	VIDEO_DATA,
	// 录像回放操作
	REC_PLAY,
	REC_DOWN,
	// RoSeek接收数据
	CAM_DATA,
}CONNECT_CMD;
#pragma pack (1) /*指定按1字节对齐*/
// 包头
typedef struct _CMD_HEAD_DATA
{
	long Cmd;
	long Len;
	char Data[1];
}CMD_HEAD_DATA;

// Cam Op
typedef struct _CAM_OP_DATA
{
	long  CamId;
	long  Pid;
	char  CamName[32];
	short CamType;
	char  CamIp[16];
	unsigned short Port;
	char  User[32];
	char  Pass[16];
	long   CamRot;
}CAM_OP_DATA;

// Task OP
typedef struct _TASK_SEL_DATA
{
	long  CamId;
	short Status;
}TASK_SEL_DATA;
// REC Start
typedef struct _VIDEO_START_DATA
{
	long  CamId;
	short CamType;
	char  CamIp[16];
	unsigned short Port;
	char  User[32];
	char  Pass[16];
}VIDEO_START_DATA;

typedef struct _REC_PLAY_DATA
{
	long  CamId;
	LONGLONG sTime;
	LONGLONG eTime;
}REC_PLAY_DATA;
typedef union _LARGE_DATA
{
	struct {
		LONG Cmd;
		LONG nType;// 0头数据,1 帧数据
	};
	LONGLONG Time; // 时间
	struct {
		DWORD LowPart;
		LONG HighPart;
	};
	struct {
		DWORD LowPart;
		LONG HighPart;
	} u;
	LONGLONG QuadPart;
}LARGE_DATA;

typedef struct _INDEX_DATA // 索引信息
{
	LARGE_DATA offPos;  // 位置偏移
	LARGE_DATA eTime;   // 结束时间
	short      tLong;   // 播放时长
	LONG       dLong;   // 数据长度
}INDEX_DATA;

#pragma pack () /*取消指定对齐，恢复缺省对齐*/
#endif