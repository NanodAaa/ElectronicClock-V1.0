/*
*		ElectricClock.h
*/

#ifndef _ELECRICCLOCK_H_
#define _ELECRICCLOCK_H_


#define _TIME_ADJUST_  // 时间调整开关
#define _SET_ALARM_    // 设置闹钟开关


#include "reg52.h"

#define BYTE unsigned char 

#define BOOL char
#define TRUE  1
#define FALSE 0

// 定义引脚

#define LCD P0		// Lcd1602输入输出


//***************LCD1602***************

// 光标移动模式
typedef enum _tagCursorMoveMode
{
	CURSOR_MOVE_LEFT = 0,	// 
	CURSOR_MOVE_RIGHT
}CursorMoveMode;


// 向LCD写入数据的位置
typedef enum _tagLcdScreenLine
{
	FIRST_LINE,			 
	SECOND_LINE			
}LcdScreenLine;


// 储存从DS1302中读取的时间
typedef struct _tagReadTime
{
	BYTE byYear;
	BYTE byYear1;  		
	BYTE byYear2;  		

	BYTE byMonth;
	BYTE byMonth1;
	BYTE byMonth2;

	BYTE byDay;
	BYTE byDay1;
	BYTE byDay2;

	BYTE byHour;
	BYTE byHour1;
	BYTE byHour2;

	BYTE byMinute;
	BYTE byMinute1;
	BYTE byMinute2;

	BYTE bySecond;
	BYTE bySecond1;
	BYTE bySecond2;
}ReadTime;


void LcdInital();
bit LcdCheckBusy();
void LcdWriteCommand(BYTE byCommand);
void LcdWriteData(BYTE byData);
BYTE LcdReadData();
BYTE LcdReadChar(LcdScreenLine line, int row);
void LcdClear();
void LcdCursorReturn();
void LcdSetInputMode(CursorMoveMode MODE);
void LcdCursorMove(CursorMoveMode DIRECTION);
void LcdDefaultMode();
void LcdSetDDRAM(BYTE address);
void DelayUs(BYTE tu);
void DelayMs(BYTE tm);
void LcdWriteString(LcdScreenLine line, int row, BYTE * byString);
void LcdWriteChar(LcdScreenLine line, int row, BYTE byChar);
void LcdDisplayControl();


//***************DS1302***************

// 读写指令宏定义
#define WRITE_YEAR   	0x8c
#define WRITE_MONTH  	0x88
#define WRITE_DAY 		0x86
#define WRITE_HOUR 	 	0x84
#define WRITE_MINUTE 	0x82
#define WRITE_SECOND 	0x80

#define READ_YEAR   	0x8d
#define READ_MONTH  	0x89
#define READ_DAY 		0x87
#define READ_HOUR   	0x85
#define READ_MINUTE 	0x83
#define READ_SECOND 	0x81


BYTE Ds1302ReadByte(BYTE byComByte);
void Ds1302WriteByte(BYTE byComByte, BYTE byData);
BYTE BcdToDat(BYTE byDat);
BYTE DatToBcd(BYTE byDat);
void Ds1302WriteProtect(BOOL bMode);


// ***************ElectricClock***************

void Screen1Display();
void Screen2Display();
void Screen3Display();
void Ds1302TimeInit();
void TimeAdjust();
void LcdReadDisplay();
void INT0_SET();
void INT0_FUNC();
void INT1_SET();
void INT1_FUNC();
void TimeAdjust();
void Screen2Init();
void Screen3Init();
void LcdToDs1302(ReadTime* STime);
void SetAlarm();
void AlarmBeep();


//***************其他***************

void DelayMs(BYTE tm);
void DelayUs(BYTE tu);


#endif
