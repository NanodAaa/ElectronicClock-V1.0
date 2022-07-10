/*
*		ElectricClock.h
*/

#ifndef _ELECRICCLOCK_H_

#include "reg52.h"

#define BYTE unsigned char 

#define LCD P0		//

sbit EN = P2^7;  	// 使能控制
sbit RS = P2^6;	 	// 寄存器选择
sbit RW = P2^5;  	// 读写选择

sbit IO = P3^4;		// 双向数据输入输出
sbit CE = P3^5;	// 使能端
sbit SCLK = P3^6;	// 串行时钟输入端，控制数据输入输出



//***************LCD1602***************

// 光标移动模式
typedef enum _tagCursorMoveMode
{
	CURSOR_MOVE_LEFT = 0,	// 
	CURSOR_MOVE_RIGHT
}CursorMoveMode;

// 像LCD写入数据的位置
typedef enum _tagLcdScreenLine
{
	FIRST_LINE,			 
	SECOND_LINE			
}LcdScreenLine;


void LcdInital();
bit LcdCheckBusy();
void LcdWriteCommand(BYTE byCommand);
void LcdWriteData(BYTE byData);
void LcdClear();
void LcdCursorReturn();
void LcdSetInputMode(CursorMoveMode MODE);
void LcdSetMoveMode(CursorMoveMode MODE);
void LcdDefaultMode();
void LcdSetDDRAM(BYTE address);
void DelayUs(BYTE tu);
void DelayMs(BYTE tm);
void LcdWriteString(LcdScreenLine line, int row, BYTE * byString);
void LcdWriteChar(LcdScreenLine line, int row, BYTE byChar);
void LcdDisplayControl();


//***************DS1302***************






//***************其他***************
void DelayMs(BYTE tm);
void DelayUs(BYTE tu);



#define _ELECRICCLOCK_H_
#endif _ELECRICCLOCK_H_
