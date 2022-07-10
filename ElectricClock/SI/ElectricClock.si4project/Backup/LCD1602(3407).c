/*
		LCD1602.c
*/

#include "reg52.h"
#include "intrins.h"
#include "ElectricClock.h"



// lcd初始化
void LcdInital()
{
	LcdClear(); // 清屏
	LcdCursorReturn(); // 光标归位
	LcdSetInputMode(CURSOR_MOVE_RIGHT); // 输入时光标右移，AC + 1
	LcdDefaultMode(); // 默认功能设置
}


// 检测繁忙 
// 返回 “1” 标识1602内部忙
bit LcdCheckBusy()
{
	RS = 0;
	RW = 1;
	EN = 0;
	_nop_();
	EN = 1;
	
	return (LCD & 0x80); // 保留DB7
}


// lcd写指令
void LcdWriteCommand(BYTE byCommand)
{
	//while(LcdCheckBusy());	// 检测繁忙
	RS = 0;
	RW = 0;
	EN = 1;	
	LCD = byCommand;
	_nop_();
	EN = 0;
	
}

// lcd写数据
void LcdWriteData(BYTE byData)
{
	//while(LcdCheckBusy());
	
	RS = 1;
	RW = 0;
	EN = 1;
	LCD = byData;
	_nop_();
	EN = 0;
	
}

// lcd写字符
void LcdWriteChar(LcdScreenLine line, int row, BYTE byChar)
{
	if(line == FIRST_LINE)
	{
		
LcdWriteCommand(0x80 + row);
	}
	else if(line == SECOND_LINE)
	{
		LcdWriteCommand(0xc0 + row);
	}

	
	LcdWriteData(byChar);
	
}


// lcd写字符串
void LcdWriteString(LcdScreenLine line, int row, BYTE* byString)
{
	if(line == FIRST_LINE)
	{
		
LcdWriteCommand(0x80 + row);
	}
	else if(line == SECOND_LINE)
	{
		LcdWriteCommand(0xc0 + row);
	}

	while(*byString)
	{
		LcdWriteData(*byString);
		byString++;
	}
}



// lcd清屏
void LcdClear()
{
	LcdWriteCommand(0x01);
	DelayMs(5);
	
}

// lcd光标归位
void LcdCursorReturn()
{
	LcdWriteCommand(0x02);
	DelayMs(5);
}


// 设置输入模式
void LcdSetInputMode(CursorMoveMode mode)
{
	// 默认输入字符时显示屏全部显示不移动， S = 0
	
	switch(mode)
	{
		// 输入时 光标左移 AC - 1
		case CURSOR_MOVE_LEFT:
			LcdWriteCommand(0x04);
			DelayMs(5);
			break;
		
		// 输入时 光标右移，AC + 1
		case CURSOR_MOVE_RIGHT:
			LcdWriteCommand(0x06);
			DelayMs(5);
			break;
	}
}

// 设置光标和显示移动指令
void LcdSetMoveMode(CursorMoveMode mode)
{
	switch(mode)
	{
		// 光标左移1格，AC - 1
		case CURSOR_MOVE_LEFT:
			LcdWriteCommand(0x18);
			DelayMs(5);
			break;
		
		// 光标右移1格，AC + 1
		case CURSOR_MOVE_RIGHT:
			LcdWriteCommand(0x14);
			DelayMs(5);
			break;
	}
}

// 默认功能设置
void LcdDefaultMode()
{
	// 默认8位数据总线
	// 默认2行显示模式
	// 默认5x7点阵/字符

	LcdWriteCommand(0x38);
	DelayMs(5);
	
}

// 设置DDRAM地址
// 范围 40H-67H
void LcdSetDDRAM(BYTE address)
{
	LcdWriteCommand(address);
	DelayMs(5);
	
}

// 延时函数
void DelayUs(BYTE tu)
{
	while(--tu);
}

void DelayMs(BYTE tm)
{
	while(tm--)
	{
		DelayUs(248);
		DelayUs(248);
	}
}






















