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



// LCD1602

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







#define _ELECRICCLOCK_H_
#endif _ELECRICCLOCK_H_
