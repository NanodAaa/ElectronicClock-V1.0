/*
*		EletricClock.c
*/


#include "reg52.h"
#include "ElectricClock.h"


void ScreenDisplay();


int main()
{
	// 
	
	while(1)
	{
		LcdInital(); // 初始化LCD1602
		ScreenDisplay();
		
		
		
		DelayMs(500);
	}
	
}


// 显示屏默认显示
void ScreenDisplay()
{
	LcdWriteString(FIRST_LINE, 0, "DATE");
	LcdWriteString(SECOND_LINE, 0, "TIME");
}



