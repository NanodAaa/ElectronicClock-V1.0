/*
*		EletricClock.c
*/


#include "reg52.h"
#include "ElectricClock.h"


void ScreenDisplay();


int main()
{
	// 
	LcdInital(); // 初始化LCD1602
	
	while(1)
	{
		
		ScreenDisplay(); // 屏幕默认显示数据
		
		
		
		DelayMs(270);
	}
	
}


// 显示屏默认显示
void ScreenDisplay()
{
	LcdWriteString(FIRST_LINE, 0, "DATE");
	LcdWriteString(SECOND_LINE, 0, "TIME");
	LcdWriteChar(FIRST_LINE, 10, 0x3a); //':'
	LcdWriteChar(FIRST_LINE, 13, 0x3a);
	LcdWriteChar(SECOND_LINE, 10, 0x2d); //'-'
	LcdWriteChar(SECOND_LINE, 13, 0x2d);
	
	LcdWriteChar(FIRST_LINE, 6, '2');
	LcdWriteChar(FIRST_LINE, 7, '0');
}



