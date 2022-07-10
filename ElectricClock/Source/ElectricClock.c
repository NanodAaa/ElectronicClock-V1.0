/*
*		EletricClock.c
*/


//#include "reg52.h"
#include "ElectricClock.h"
#include "stdio.h"

BOOL TimeAdjustFlag;
BOOL SetAlarmFlag;

ReadTime SAlarmTime;
ReadTime SSavedTime;
ReadTime SReadTime;


// 独立按键
sbit K1 = P3^2;
sbit K2 = P3^3; 
sbit K3 = P3^4; // 外部中断INT0
sbit K4 = P3^5; // 外部中断INT1

// 蜂鸣器?
sbit BEEP = P1^0;

int main()
{
	LcdInital(); // 初始化LCD1602
	Ds1302TimeInit(); // 设置Ds1302默认时间为 2003/2/24 11:45:14
	
	while(1)
	{
		
		Screen1Display(); // 屏幕默认显示的数据
		LcdReadDisplay(); // 读取Ds1302时间并显示


		// 调整时间	
		#ifdef _TIME_ADJUST_

		TimeAdjust();

		#endif


		// 设置闹钟
		#ifdef _SET_ALARM_

		SetAlarm();
		AlarmBeep();


		#endif

		
		DelayMs(50);		
	}	
}


// 显示屏默认显示
void Screen1Display()
{
	LcdWriteString (FIRST_LINE,   0, "DATE");
	LcdWriteString (SECOND_LINE,  0, "TIME");
	LcdWriteChar   (SECOND_LINE,  10, 0x3a);   	//':'
	LcdWriteChar   (SECOND_LINE,  13, 0x3a);
	LcdWriteChar   (FIRST_LINE,   10, 0x2d);  	//'-'
	LcdWriteChar   (FIRST_LINE,   13, 0x2d);
	
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
	
	SReadTime.byDay1    = 0x30 + (SReadTime.byDay    & 0x0f);
	SReadTime.byDay2    = 0x30 + (SReadTime.byDay    & 0xf0);

	SReadTime.byMonth1  = 0x30 + (SReadTime.byMonth  & 0x0f);
	SReadTime.byMonth2  = 0x30 + (SReadTime.byMonth  & 0xf0);

	SReadTime.byYear1   = 0x30 + (SReadTime.byYear   & 0x0f);
	SReadTime.byYear2   = 0x30 + (SReadTime.byYear   & 0xf0);

	SReadTime.bySecond1 = 0x30 + (SReadTime.bySecond & 0x0f);
	SReadTime.bySecond2 = 0x30 + (SReadTime.bySecond & 0xf0);

	SReadTime.byMinute1 = 0x30 + (SReadTime.byMinute & 0x0f);
	SReadTime.byMinute2 = 0x30 + (SReadTime.byMinute & 0xf0);

	SReadTime.byHour1   = 0x30 + (SReadTime.byHour   & 0x0f);
	SReadTime.byHour2   = 0x30 + (SReadTime.byHour   & 0xf0);

	// lcd1602显示数据
	// 第一行日期
	LcdWriteChar(FIRST_LINE,  15, SReadTime.byDay1);
	LcdWriteChar(FIRST_LINE,  14, SReadTime.byDay2);

	LcdWriteChar(FIRST_LINE,  12, SReadTime.byMonth1);
	LcdWriteChar(FIRST_LINE,  11, SReadTime.byMonth2);

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


// ***************调整时间功能***************

#ifdef _TIME_ADJUST_

// 时间调整
void TimeAdjust()
{

	// 触发中断开始
	if(TimeAdjustFlag)
	{	
		// 界面初始化
		Screen2Init();
		Screen2Display();
		LcdReadDisplay();

		while(TimeAdjustFlag)
		{
			// 再次按下 K3 退出中断
			if(K3 == 0)
			{	
				DelayMs(10);
				if(K3 == 0);
				{
					TimeAdjustFlag = FALSE;
				}
			}	

			// 改变LCD1602数字
			LcdChangeDigit();

			// 读取lcd1602写入Ds1302
			LcdToDs1302(SSavedTime);
		}
	}	
}


// “更改时间”界面初始化
void Screen2Init()
{
	LcdClear(); // 清屏
	// 显示屏开显示
	// 显示光标
	// 光标闪烁
	LcdWriteCommand(0x0f);
}


// “调整时间”界面
void Screen2Display()
{
	LcdClear(); // 清屏

	LcdWriteString(FIRST_LINE,    0, "Time");
	LcdWriteString(SECOND_LINE,   0, "Adjust");
	LcdWriteChar  (SECOND_LINE,   10, 0x3a);     //':'
	LcdWriteChar  (SECOND_LINE,   13, 0x3a);
	LcdWriteChar  (FIRST_LINE,    10, 0x2d);     //'-'
	LcdWriteChar  (FIRST_LINE,    13, 0x2d);
	
	LcdWriteChar  (FIRST_LINE,    6, '2');
	LcdWriteChar  (FIRST_LINE,    7, '0');
}


// 更改LCD1602屏幕上的数字
// K1 左移
// K2 +1
void LcdChangeDigit()
{
	BYTE PlusNum = 0; // 加数
	int PlusTime = 0; // 增加的次数
	BOOL LineFeedFlag = FALSE; // 换行标志
	
	// 按下K1光标左移1位
	if(K1 == 0)
	{
		DelayMs(10);
		if(K1 == 0)
		{
			// 左移15位后，下一位换行到第1行
			if(PlusTime == 16)
			{
				// 光标归位
				LcdCursorReturn();
				
				
				// 光标右移15位
				for(int i = 0; i < 16; i++)
				{
					LcdCursorMove(CURSOR_MOVE_RIGHT); // 光标右移
				}
	
			}
		
			LcdCursorMove(CURSOR_MOVE_LEFT); // 光标左移1位
			PlusTime++;			
			LineFeedFlag = TRUE; // 启用换行标志	
		}	
	}

	// 按下K2更改光标位置数字
	// 数字从0开始到9
	if(K2 == 0)
	{
		DelayMs(10);
		if(K2 == 0)
		{
			// 如果换行，加数重置为0
			// 如果加数大于9，加数重置为0
			if(LineFeedFlag == TURE || PlusNum > 9)
			{
				PlusNum = 0;
			}
			
			LcdWriteData(DatToBcd(PlusNum)); // 显示更改后的数字
			PlusNum++;						 //  加数+1		
		}
	}
}


// 读取lcd1602写入Ds1302，调整时间
void LcdToDs1302(ReadTime* STime)
{
	
	// 储存LCD1602显示的数据
	SReadTime.bySecond1 = LcdReadChar(SECOND_LINE, 15);
	SReadTime.bySecond2 = LcdReadChar(SECOND_LINE, 14);

	SReadTime.byMinute1 = LcdReadChar(SECOND_LINE, 12);
	SReadTime.byMinute2 = LcdReadChar(SECOND_LINE, 11);

	SReadTime.byHour1   = LcdReadChar(SECOND_LINE,  9);
	SReadTime.byHour2   = LcdReadChar(SECOND_LINE,  8);

	SReadTime.byDay1    = LcdReadChar(FIRST_LINE,  14);
	SReadTime.byDay2    = LcdReadChar(FIRST_LINE,  15);

	SReadTime.byMonth1  = LcdReadChar(FIRST_LINE,  11);
	SReadTime.byMonth2  = LcdReadChar(FIRST_LINE,  12);

	SReadTime.byYear1   = LcdReadChar(FIRST_LINE,   9);
	SReadTime.byYear2   = LcdReadChar(FIRST_LINE,   8);

	// 合并十位和个位
	SReadTime.bySecond = SReadTime.bySecond1 + SReadTime.bySecond2 * 10;
	SReadTime.byMinute = SReadTime.byMinute1 + SReadTime.byMinute2 * 10;
	SReadTime.byHour   = SReadTime.byHour1   + SReadTime.byHour2   * 10;
	SReadTime.byDay    = SReadTime.byDay1    + SReadTime.byDay2    * 10;
	SReadTime.byMonth  = SReadTime.byMonth1  + SReadTime.byMonth2  * 10;
	SReadTime.byYear   = SReadTime.byYear1   + SReadTime.byYear2   * 10;

	// 写入DS1302
	Ds1302WriteProtect(FALSE); // 关闭写保护
	
	Ds1302WriteByte(WRITE_SECOND, DatToBcd(SReadTime.bySecond));
	Ds1302WriteByte(WRITE_MINUTE, DatToBcd(SReadTime.byMinute));
	Ds1302WriteByte(WRITE_HOUR,   DatToBcd(SReadTime.byHour));
	Ds1302WriteByte(WRITE_DAY,    DatToBcd(SReadTime.byDay));
	Ds1302WriteByte(WRITE_MONTH,  DatToBcd(SReadTime.byMonth));
	Ds1302WriteByte(WRITE_YEAR,   DatToBcd(SReadTime.byYear));

	Ds1302WriteProtect(TRUE); // 打开写保护
}


#endif


// ***************设置闹钟***************
// 有些函数在#ifdef _TIME_ADJUST_里，如屏蔽则部分功能无法实现

#ifdef _SET_ALARM_

// 设置闹钟
void SetAlarm()
{
	if(SetAlarmFlag == TRUE)
	{
		
		// 初始化屏幕3
		Screen3Init();
		Screen3Display();

		while(SetAlarmFlag == TRUE)
		{
			// 再次按下K4退出屏幕3
			if(K4 == 0)
			{
				DelayMs(10);
				if(K4 == 0)
				{
					SetAlarmFlag == FALSE;
				}
			}
			
			// 设置闹钟时间
			LcdChangeDigit();
			
		}
		
		// 将Lcd1602时间储存到DS1302
		LcdToDs1302(SAlarmTime); 
	}
}


// 到达设定时间，蜂鸣器发声
void AlarmBeep()
{
	if(memcmp(&SReadTime, &SAlarmTime, sizeof(ReadTime)) == 0)
	{
		for(int i = 0; i < 2000; i++)
		{
			BEEP = 1;
			DelayMs(20);
			BEEP = 0;
		}
	}
}


// 屏幕3初始化
void Screen3Init()
{
	LcdClear(); // 清屏
	// 显示屏开显示
	// 显示光标
	// 光标闪烁
	LcdWriteCommand(0x0f);
}


// “闹钟设置”界面
void Screen3Display()
{
	LcdClear(); //清屏
	LcdWriteString(SECOND_LINE, 0,  "ALARM");
	LcdWriteString(FIRST_LINE,  0,  "SET     NanodAaa");
	LcdWriteChar  (SECOND_LINE, 8,  '0');
	LcdWriteChar  (SECOND_LINE, 9,  '0');
	LcdWriteChar  (SECOND_LINE, 11, '0');
	LcdWriteChar  (SECOND_LINE, 12, '0');
	LcdWriteChar  (SECOND_LINE, 14, '0');

	LcdWriteChar  (SECOND_LINE, 10, 0x3a); //':'
	LcdWriteChar  (SECOND_LINE, 13, 0x3a);
	
	LcdWriteChar  (SECOND_LINE, 15, '0'); //不要动
	LcdWriteCommand(0x10);
}

#endif



// ***************其他函数***************

// 外部中断0 K3控制
void INT0_SET()
{
	EA = 1;
	EX0 = 1;
	IT0 = 1;
}


// 外部中断0 功能
void INT0_FUNC() interrupt 1
{
	if(TimeAdjustFlag == FALSE)
	{
		TimeAdjustFlag = TRUE;
	}
}


// 外部中断1 K4控制
void INT1_SET()
{
	EA = 1;
	EX1 = 1;
	IT1 = 1;
}


void INT1_FUNC() interrupt 2
{
	if(SetAlarmFlag == FALSE)
	{
		SetAlarmFlag = TRUE;
	}
}


