/*
*		EletricClock.c
*/


//#include "reg52.h"
#include "ElectricClock.h"


void ScreenDisplay();
void Ds1302TimeInit();
void LcdReadDisplay();

int main()
{
	// 
	LcdInital(); // 初始化LCD1602
	Ds1302TimeInit(); // 设置Ds1302默认时间为 2003/2/24 11:45:14
	
	while(1)
	{
		
		ScreenDisplay(); // 屏幕默认显示的数据
		LcdReadDisplay(); // 读取Ds1302时间并显示
		
		
		DelayMs(270);
	}
	
}


// 显示屏默认显示
void ScreenDisplay()
{
	LcdWriteString (FIRST_LINE,   0, "DATE");
	LcdWriteString (SECOND_LINE,  0, "TIME");
	LcdWriteChar   (FIRST_LINE,   10, 0x3a);   	//':'
	LcdWriteChar   (FIRST_LINE,   13, 0x3a);
	LcdWriteChar   (SECOND_LINE,  10, 0x2d);  	//'-'
	LcdWriteChar   (SECOND_LINE,  13, 0x2d);
	
	LcdWriteChar   (FIRST_LINE,   6, '2');
	LcdWriteChar   (FIRST_LINE,   7, '0');
}

// 时钟的默认时间
void Ds1302TimeInit()
{
	Ds1302WriteProtect(FALSE); // 关闭写保护
	
	// 2003/02/24
	Ds1302WriteByte(WRITE_YEAR,   DatToBcd(3));
	Ds1302WriteByte(WRITE_MONTH,  DatToBcd(2));
	Ds1302WriteByte(WRITE_DAY,    DatToBcd(24));

	// 11:45:14
	Ds1302WriteByte(WRITE_HOUR,   DatToBcd(11));
	Ds1302WriteByte(WRITE_MINUTE, DatToBcd(45));
	Ds1302WriteByte(WRITE_SECOND, DatToBcd(14));

	Ds1302WriteProtect(TRUE);  // 打开写保护
	
}

// 从DS1302中读取时间并显示
void LcdReadDisplay()
{
	// 获取DS1302中的时间并储存到结构体SReadTime中
	// 读取的是BCD码，需要转换为DAT

	Ds1302WriteProtect(FALSE); // 关闭写保护
	
	SReadTime.byYear   = BcdToDat(Ds1302ReadByte(READ_YEAR));
	SReadTime.byMonth  = BcdToDat(Ds1302ReadByte(READ_MONTH));
	SReadTime.byDay    = BcdToDat(Ds1302ReadByte(READ_DAY));
	SReadTime.byHour   = BcdToDat(Ds1302ReadByte(READ_HOUR));
	SReadTime.byMinute = BcdToDat(Ds1302ReadByte(READ_MINUTE));
	SReadTime.bySecond = BcdToDat(Ds1302ReadByte(READ_SECOND));

	Ds1302WriteProtect(TRUE);  // 打开写保护

	// 分解位个位和十位
	
	SReadTime.byDay1    = 0x30 + (SReadTime.byDay    % 10);
	SReadTime.byDay2    = 0x30 + (SReadTime.byDay    / 10);

	SReadTime.byMonth1  = 0x30 + (SReadTime.byMonth  % 10);
	SReadTime.byMonth2  = 0x30 + (SReadTime.byMonth  / 10);

	SReadTime.byYear1   = 0x30 + (SReadTime.byYear   % 10);
	SReadTime.byYear2   = 0x30 + (SReadTime.byYear   / 10);

	SReadTime.bySecond1 = 0x30 + (SReadTime.bySecond % 10);
	SReadTime.bySecond2 = 0x30 + (SReadTime.bySecond / 10);

	SReadTime.byMinute1 = 0x30 + (SReadTime.byMinute % 10);
	SReadTime.byMinute2 = 0x30 + (SReadTime.byMinute / 10);

	SReadTime.byHour1   = 0x30 + (SReadTime.byHour   % 10);
	SReadTime.byHour2   = 0x30 + (SReadTime.byHour   / 10);

	// lcd1602显示数据

	// 第一行日期
	LcdWriteChar(FIRST_LINE,  14, SReadTime.byDay1);
	LcdWriteChar(FIRST_LINE,  15, SReadTime.byDay2);

	LcdWriteChar(FIRST_LINE,  11, SReadTime.byMonth1);
	LcdWriteChar(FIRST_LINE,  12, SReadTime.byMonth2);

	LcdWriteChar(FIRST_LINE,  9,  SReadTime.byYear1);
	LcdWriteChar(FIRST_LINE,  8,  SReadTime.byYear2);
	
	// 第二行时间
	LcdWriteChar(SECOND_LINE, 14, SReadTime.bySecond2);

	LcdWriteChar(SECOND_LINE, 12, SReadTime.byMinute1);
	LcdWriteChar(SECOND_LINE, 11, SReadTime.byMinute2);

	LcdWriteChar(SECOND_LINE, 9,  SReadTime.byHour1);
	LcdWriteChar(SECOND_LINE, 8,  SReadTime.byHour2);
	
	LcdWriteChar(SECOND_LINE, 15, SReadTime.bySecond1); // 为使光标处于第二行最后一列，勿调整

}



