/*
		LCD1602.c
*/

#include "reg52.h"
#include "intrins.h"
#include "ElectricClock.h"



// lcd初始化
void LcdInital()
{
	LcdDefaultMode(); 						// 默认功能设置
	DelayMs(5);
	LcdWriteCommand(0x08);
	LcdClear(); 							// 清屏
	LcdSetInputMode(CURSOR_MOVE_RIGHT); 	// 输入时光标右移，AC + 1
	LcdDisplayControl();					// 显示开关控制
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
	while(LcdCheckBusy());	// 检测繁忙
	RS = 0;
	RW = 0;
	EN = 1;	
	LCD = byCommand;
	_nop_();
	EN = 0;
	
	DelayMs(2);
	
}

// lcd写数据
void LcdWriteData(BYTE byData)
{
	while(LcdCheckBusy());
	
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
	
}

// lcd光标归位
void LcdCursorReturn()
{
	LcdWriteCommand(0x02);
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
			break;
		
		// 输入时 光标右移，AC + 1
		case CURSOR_MOVE_RIGHT:
			LcdWriteCommand(0x06);
			break;
	}
}

// 显示开关控制
void LcdDisplayControl()
{
	// 默认显示屏开显示 D=1
	// 默认不显示光标 C=0
	// 默认光标不闪烁 B=0 
	// 0000 1100
	
	LcdWriteCommand(0x0c);
}


// 光标显示移动
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
	
}

// 设置DDRAM地址
// 范围 40H-67H
void LcdSetDDRAM(BYTE address)
{
	LcdWriteCommand(address);
	
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






















