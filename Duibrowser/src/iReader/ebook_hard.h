//
// Copyright (c) HanWang cyl Co., Ltd. 2008.
//
//
// 2008-12-25 v0.1
//
//
// do not modify !!!
//
#ifndef __EBOOK_HARD_H__
#define __EBOOK_HARD_H__
// Windows Header Files:
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HARD_517        100
#define HARD_518        120
#define HARD_520        130

#define	TIME_CLR_EVENT	        _T("TIMEER_CLR")

#define VK_Ebook_Control        VK_CONTROL
#define VK_Ebook_Down           VK_DOWN
#define VK_Ebook_Up             VK_UP
#define VK_Ebook_Right          VK_RIGHT
#define VK_Ebook_Left           VK_LEFT
#define VK_Ebook_Next           VK_NEXT
#define VK_Ebook_Prior          VK_PRIOR
#define VK_Ebook_Ok             VK_RETURN
#define VK_Ebook_Cancel         VK_ESCAPE
#define VK_Ebook_Space          VK_SPACE
#define VK_Ebook_Dict           VK_F6
#define VK_Ebook_VolumeUp       VK_F7
#define VK_Ebook_VolumeDown     VK_F8
#define VK_Ebook_Symbol         VK_F9
#define VK_Ebook_Shift			VK_SHIFT
#define VK_Ebook_Capital		VK_CAPITAL
#define VK_Ebook_Del            VK_BACK

#ifdef MIPS
#define VK_Ebook_Refurbish      VK_F5
#define VK_Ebook_Mark           VK_F2
#define VK_Ebook_Rotate         VK_F3
#define VK_Ebook_Zoom           VK_ZOOM
#define VK_Ebook_Menu           VK_F4
#define VK_Ebook_Play           VK_PLAY
#define VK_Ebook_Power          VK_F7
#else
#define VK_Ebook_Refurbish      'W'
#define VK_Ebook_Mark           'A'
#define VK_Ebook_Rotate         'R'
#define VK_Ebook_Zoom           'Z'
#define VK_Ebook_Menu           'M'
#define VK_Ebook_Play           'P'
#define VK_Ebook_Power          'Q'
#define VK_Ebook_Lock           'L'
#define VK_Ebook_Unlock         'U'
#define VK_Ebook_SDIn           'S'
#define VK_Ebook_SDOut          'D'
#define VK_Ebook_USBCIn         'F'
#define VK_Ebook_USBCOut        'G'
#define VK_Ebook_USBHIn         'H'
#define VK_Ebook_USBHOut        'J'
#define VK_Ebook_DcIn           'V'
#define VK_Ebook_DcOut          'B'
#endif

#define VK_Ebook_Num_0        0x30
#define VK_Ebook_Num_1        0x31
#define VK_Ebook_Num_2        0x32
#define VK_Ebook_Num_3        0x33
#define VK_Ebook_Num_4        0x34
#define VK_Ebook_Num_5        0x35
#define VK_Ebook_Num_6        0x36
#define VK_Ebook_Num_7        0x37
#define VK_Ebook_Num_8        0x38
#define VK_Ebook_Num_9        0x39

//--------------------------------------
#define WM_EBOOKHARD            (WM_USER + 0x1001)
//参数高位 //参数低位
    #define WP_TIMEOUT  1
        #define LP_TIMEOUT      0
        #define LP_ALARM        1

    #define WP_SDCARD   2
        #define LP_SDOUT        0
        #define LP_SDIN         1
        #define LP_SDINSERT     3

    #define WP_USBC     3
        #define LP_USBCOUT      0
        #define LP_USBCIN       1

    #define WP_USBH     4
        #define LP_USBHOUT      0
        #define LP_USBHIN       1

    #define WP_DCPOWER  5
        #define LP_DCOUT        0
        #define LP_DCIN         1

    #define WP_BATLEVEL 6
        #define LP_POWER0       0               //auto off   需要关机
        #define LP_POWER1       (LP_POWER0+1)   // low bat   需要提示低电
        #define LP_POWER2       (LP_POWER0+2)
        #define LP_POWER3       (LP_POWER0+3)
        #define LP_POWER4       (LP_POWER0+4)
        #define LP_POWER5       (LP_POWER0+5)
        #define LP_POWER6       (LP_POWER0+6)

    #define WP_KEYLOCK  7
        #define LP_KEYUNLOCK    0
        #define LP_KEYLOCK      1

    #define WP_POWERKEY 8
        #define LP_POWERKEY     0
        #define LP_WIFION       2
        #define LP_WIFIOFF      3

    #define WP_PENPOWER 9
        #define LP_PENOFF       0
        #define LP_PENON        1

    #define WP_DISEND   10
        #define LP_DISPART      0
        #define LP_DISFULL      1
        #define LP_DISFORC      2

    #define WP_WIFI     11
        #define LP_ERROR        0

    #define WP_Angle    12
        #define LP_DMDO_0       0

//Added by BYZ for MultiAlarm++

	#define WP_ALARM    13
        #define LP_ALARMINDEX1  1

//Added by BYZ for MultiAlarm--

//--------------------------------------
typedef struct __EBOOK_STATE__
{
    DWORD   Haveinit;
    DWORD   Sdcard; 		// app use
    DWORD   Usbclient;	    // app use
    DWORD   Usbhost;
    DWORD   Dcpower;        // app use
    DWORD   Batlevel;       // app use
    DWORD   Keylock;        // app use
    DWORD   Timeout;
    DWORD   PenState;       // app use
    DWORD   Angle;          // deriver use
    DWORD   rev6;
    DWORD   rev5;
    DWORD   rev4;
    DWORD   rev3;
    DWORD   rev2;
    DWORD   rev1;
}EBOOK_STATE, *P_EBOOK_STATE;
//--------------------------------------

//Added by BYZ for MultiAlarm++
//--------------------------------------
typedef struct __ALARM__
{
    DWORD   	 Index;
    SYSTEMTIME * pAlarmTime;
}ALARM, *PALARM;
//--------------------------------------

typedef struct __DRV_ALARM__
{
    DWORD   	 Index;    
	SYSTEMTIME   AlarmTime;
}DRV_ALARM, *PDRV_ALARM;

//Added by BYZ for MultiAlarm--

#define LCDMODE_PART    2
#define LCDMODE_FULL    3
#define LCDMODE_AUTO    4

#define LCD_ANG0        0
#define LCD_ANG90       1
#define LCD_ANG180      2
#define LCD_ANG270      4

//----------------------------------------
// hardinfo index define
typedef enum {
    INF_HARD = 0,    //dword    //硬件版本
    INF_SOFT,        //dword    //软件版本
    INF_MAJOR,     //dword  //主产品号
    INF_MINOR,    //dword   //次产品号
    INF_LCD,        //byte
    INF_AUDIO,          //byte
    INF_RTC,            //byte
    INF_WIFI,           //byte
    INF_TOUCH,       //byte
    INF_KEY,            //byte

} HARDINFO_INDEX;


#ifdef MIPS

//得到设备ID号 pOutBuf 字符串缓冲区 , len 缓冲取长度
//返回值 id 长度
DWORD _Ebook_Get_Id(WCHAR *pOutBuf, DWORD len);

// 设置设备ID号 len 必须等于16, 返回false 设置失败
BOOL _Ebook_Set_Id(WCHAR *pInBuf, DWORD len);

// 设置设备WIFI MAC ,  len 必须等于12, 返回false 设置失败
BOOL _Ebook_Set_WIFIMAC(WCHAR *pInBuf, DWORD len);

//升级程序， pinbuf 指定文件名
BOOL _Ebook_Set_Updata(CHAR *pInBuf, DWORD len);

//得到当前版本号，1Dword 硬件版本，1Dword 软件版本
DWORD _Ebook_Get_HardVersion(DWORD *pOutBuf);

//得到当前硬件信息; index 查询的序号
DWORD _Ebook_Get_HardInfo(DWORD Index);

// 得到当前 lcd 显示刷新模式
DWORD _Ebook_Get_LcdMode(void);

//设置lcd 显示模式 LCDMODE_PART 局部， LCDMODE_FULL 全局， LCDMODE_AUTO；自动（由驱动决定刷新模式)
DWORD _Ebook_Set_LcdMode(DWORD iMode);

//设置lcd 灰阶模式
DWORD _Ebook_Set_Lcd16Mode(BOOL b16Mode);

// 立即 用当前模式刷新 LCDMODE_PART 局部，LCDMODE_FULL 全局 ;LCDMODE_AUTO  不改变愿模式
DWORD _Ebook_Force_Updata(DWORD iMode);

// 下一次 用当前模式刷新 LCDMODE_PART 局部，LCDMODE_FULL 全局 ;LCDMODE_AUTO  不改变愿模式
DWORD _Ebook_Updata_Next(DWORD iMode);

// 得到当前 显示角度 LCD_ANG0 =0 LCD_ANG90=90
DWORD _Ebook_Get_Rotate(void);

DWORD _Ebook_rotate_Lcd(DWORD iAngler);

// 是否进入手写模式 ture
DWORD _Ebook_Enter_Handwrite(BOOL bWrite);

// 是否开始数据传输 true 开始, 在收到USB_IN 消息后才允许调用
VOID _Ebook_Start_Transfer(void);

// 重起系统
VOID _Ebook_Reset_System(void);

// 关闭 系统电源  blank 指示是否将显示刷新成白屏
DWORD _Ebook_Power_Off(BOOL Blank);

// 进入休眠模式
DWORD _Ebook_Enter_Sleep(void);

// 开关wifi 
DWORD _Ebook_WiFi_On(BOOL bOn);

// 得到当前硬件状态
DWORD _Ebook_Get_HardState(EBOOK_STATE *default_state);

// 休眠时是否允许 touch 唤醒，默认允许唤醒
DWORD _Ebook_Touch_EnableWake(BOOL bWake);

// 是否关闭touch 电源， 如关闭，则没有touch消息切touch不能唤醒
DWORD _Ebook_Touch_Off(BOOL bOff);

// 设置休眠后自动唤醒的时间，单位秒
DWORD _Ebook_Set_WakeTime(DWORD iSec);

// 设置 超时时间，单位秒， 0 关闭超时消息
DWORD _Ebook_Set_TimeOut(DWORD iSec);

DWORD _Ebook_Get_Bmp(unsigned char *buf);

DWORD _Ebook_Set_Alarm(PALARM pAlarm);

DWORD _Ebook_Disable_KeyWake(BOOL Disable);

#pragma comment (lib, "ebook_hard.lib")
#endif // MIPS

#ifdef __cplusplus
}
#endif

#endif // __EBOOK_HARD_H__