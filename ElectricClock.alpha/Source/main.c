/*
			DS1302 x LCD1602 时钟，附带闹钟功能
*/


#include "reg51.h"
#include "LCD1602.h"
#include "DS1302.h"

bit Keyflag0; //INT0标志
bit Keyflag1; //INT1标志

//独立按键
sbit K1 = P3^2;
sbit K2 = P3^3;
sbit K3 = P3^4; //INT0
sbit K4 = P3^5; //INT1

//全局时间变量
unsigned char g_sec;
unsigned char g_sec1; //秒1位
unsigned char g_sec2; //秒10位
unsigned char g_min;
unsigned char g_min1; //分1位
unsigned char g_min2; //分10位
unsigned char g_hou;  
unsigned char g_hou1; //时1位
unsigned char g_hou2; //时10位
unsigned char g_day;
unsigned char g_day1; //日1位
unsigned char g_day2; //日10位
unsigned char g_mon; 
unsigned char g_mon1; //月1位
unsigned char g_mon2; //月10位
unsigned char g_yea;
unsigned char g_yea1; //年1位
unsigned char g_yea2; //年10位

//屏幕2储存时间变量
unsigned char s_sec1; //秒1位
unsigned char s_sec2; //秒10位
unsigned char s_sec;
unsigned char s_min;
unsigned char s_min1; //分1位
unsigned char s_min2; //分10位
unsigned char s_hou;
unsigned char s_hou1; //时1位
unsigned char s_hou2; //时10位
unsigned char s_day;
unsigned char s_day1; //日1位
unsigned char s_day2; //日10位
unsigned char s_mon;
unsigned char s_mon1; //月1位
unsigned char s_mon2; //月10位
unsigned char s_yea;
unsigned char s_yea1; //年1位
unsigned char s_yea2; //年10位

//闹钟时间变量
unsigned char a_sec1;
unsigned char a_sec2;
unsigned char a_min1;
unsigned char a_min2;
unsigned char a_hou1;
unsigned char a_hou2;

void DelayUs(unsigned char tu);
void DelayMs(unsigned char tm);
void Screen_interface();
void Screen2_interface();
void Screen2_digit_save();
void Screen2_digit_interface();
void Screen3_interface();
void Time_display();
void Alarm_set();
void INT0_S();
void INT0_Z();
void INT1_S();
void INT1_Z();
void Clock_display();
void Clock_initial_value();
void Date_display();
void Time_Adjust();
void Clock_value_adjust();


void main()
{
	//初始化
	//T0_S();
	Clock_initial_value(); //设置时钟初值
	INT0_S();
	INT1_S();
	lcd1602_init();
	lcd1602_clear();
	
	//主循环
	while(1)
	{
		//时间、日期显示
		Clock_display();
		
	
		//时间调节
		Time_Adjust();
		
		//闹钟设置
		Alarm_set();
		
		DelayMs(50); //屏幕刷新率 50ms
	}
}

//屏幕1、2界面
void Screen_interface()
{
	lcd1602_write_string(0, 0, "Date");
	lcd1602_write_string(1, 0, "Time");
	lcd1602_write_char(1, 10, 0x3a); //':'
	lcd1602_write_char(1, 13, 0x3a);
	lcd1602_write_char(0, 10, 0x2d); //'-'
	lcd1602_write_char(0, 13, 0x2d);
	
	lcd1602_write_char(0, 6, '2');
	lcd1602_write_char(0, 7, '0');
}

void Screen2_interface()
{
	lcd1602_write_string(1, 0, "Adjust");
	lcd1602_write_string(0, 0, "Time");
	lcd1602_write_char(1, 10, 0x3a); //':'
	lcd1602_write_char(1, 13, 0x3a);
	lcd1602_write_char(0, 10, 0x2d); //'-'
	lcd1602_write_char(0, 13, 0x2d);
	
	lcd1602_write_char(0, 6, '2');
	lcd1602_write_char(0, 7, '0');
}

//屏幕2数字储存
void Screen2_digit_save()
{
	s_sec1 = g_sec1; //秒1位
	s_sec2 = g_sec2; //秒10位

	s_min1 = g_min1; //分1位
	s_min2 = g_min2; //分10位

	s_hou1 = g_hou1; //时1位
	s_hou2 = g_hou2; //时10位

	s_day1 = g_day1; //日1位
	s_day2 = g_day2; //日10位

	s_mon1 = g_mon1; //月1位
	s_mon2 = g_mon2; //月10位

	s_yea1 = g_yea1; //年1位
	s_yea2 = g_yea2; //年10位
}

//屏幕2数字显示
void Screen2_digit_interface()
{
	Screen2_digit_save();
	
	lcd1602_write_char(1, 14, s_sec2);
	
	lcd1602_write_char(1, 12, s_min1);
	lcd1602_write_char(1, 11, s_min2);
	lcd1602_write_char(1, 9, s_hou1);
	lcd1602_write_char(1, 8, s_hou2);
	lcd1602_write_char(0, 14, s_day2);
	lcd1602_write_char(0, 15, s_day1);
	lcd1602_write_char(0, 11, s_mon2);
	lcd1602_write_char(0, 12, s_mon1);
	lcd1602_write_char(0, 9, s_yea2);
	lcd1602_write_char(0, 8, s_yea1);
	
	lcd1602_write_char(1, 15, s_sec1); //固定位置，别动！
}

//屏幕3界面
void Screen3_interface()
{
	lcd1602_clear(); //清屏
	lcd1602_write_string(1, 0, "ALARM");
	lcd1602_write_string(0, 0, "SET     NanodAaa");
	lcd1602_write_char(1, 8, '0');
	lcd1602_write_char(1, 9, '0');
	lcd1602_write_char(1, 11, '0');
	lcd1602_write_char(1, 12, '0');
	lcd1602_write_char(1, 14, '0');

	lcd1602_write_char(1, 10, 0x3a); //':'
	lcd1602_write_char(1, 13, 0x3a);
	
	lcd1602_write_char(1, 15, '0'); //不要动
	write_lcd1602_command(0x10);
}

//屏幕1时钟总显示
void Clock_display()
{
	Screen_interface();
	Time_display();
	Date_display();
}

//调用ds1302显示时间
void Time_display()
{
	//读取秒数
	ds1302_write_data(0x8e, 0);
	g_sec = BCD_to_dat(ds1302_read(0x81)); 
	ds1302_write_data(0x8e, 0x80);
	//秒位显示
	g_sec1 = 0x30 + (g_sec % 10);
	g_sec2 = 0x30 + (g_sec / 10);
	lcd1602_write_char(1, 15, g_sec1);
	lcd1602_write_char(1, 14, g_sec2);
	
	//读取分数
	ds1302_write_data(0x8e, 0);
	g_min = BCD_to_dat(ds1302_read(0x83)); 
	ds1302_write_data(0x8e, 0x80);
	//分位显示
	g_min1 = 0x30 + (g_min % 10);
	g_min2 = 0x30 + (g_min / 10);
	lcd1602_write_char(1, 12, g_min1);
	lcd1602_write_char(1, 11, g_min2);
	
	//读取时数
	ds1302_write_data(0x8e, 0);
	g_hou = BCD_to_dat(ds1302_read(0x85)); 
	ds1302_write_data(0x8e, 0x80);
	//时位显示
	g_hou1 = 0x30 + (g_hou % 10);
	g_hou2 = 0x30 + (g_hou / 10);
	lcd1602_write_char(1, 9, g_hou1);
	lcd1602_write_char(1, 8, g_hou2);
}

//调用DS1302显示日期
void Date_display()
{
	//读取日期
	ds1302_write_data(0x8e, 0);
	g_day = BCD_to_dat(ds1302_read(0x87)); 
	ds1302_write_data(0x8e, 0x80);
	
	//向lcd1602写入日期
	g_day1 = 0x30 + (g_day % 10);
	g_day2 = 0x30 + (g_day / 10);
	lcd1602_write_char(0, 14, g_day2);
	lcd1602_write_char(0, 15, g_day1);
	
	//读取月
	ds1302_write_data(0x8e, 0);
	g_mon = BCD_to_dat(ds1302_read(0x89)); 
	ds1302_write_data(0x8e, 0x80);
	
	//向lcd1602写入月份
	g_mon1 = 0x30 + (g_mon % 10);
	g_mon2 = 0x30 + (g_mon / 10);
	lcd1602_write_char(0, 11, g_mon2);
	lcd1602_write_char(0, 12, g_mon1);
	
	//读取年份
	ds1302_write_data(0x8e, 0);
	g_yea = BCD_to_dat(ds1302_read(0x8d)); 
	ds1302_write_data(0x8e, 0x80);
	
	//向lcd1602写入年份
	g_yea1 = 0x30 + (g_yea % 10);
	g_yea2 = 0x30 + (g_yea / 10);
	lcd1602_write_char(0, 9, g_yea2);
	lcd1602_write_char(0, 8, g_yea1);
}

//闹钟设置
void Alarm_set()
{
	if(Keyflag1 == 1)
	{
		//屏幕三
		lcd1602_alarm_init(); //闹钟设置初始化命令
		Screen3_interface(); //屏幕三界面
		
		//按下K4进入屏幕三
		while(Keyflag1 == 1)
		{
			lcd1602_course_move(); //光标移动指令
			lcd1602_change_number(); //改变lcd1602屏幕数字
			
			DelayMs(200); //设置屏幕三刷新频率
					
			//再次按下K4退出屏幕3
			if(K4 == 0)
			{
				DelayMs(10); //消抖
				if(K4 == 0)
				{
					while(K4 == 0);
					
					//读取数据,作为闹钟响铃时间
					a_hou1 = read_lcd1602_char(1, 8);
					a_hou2 = read_lcd1602_char(1, 9);
					a_min1 = read_lcd1602_char(1, 11);
					a_min2 = read_lcd1602_char(1, 12);
					a_hou1 = read_lcd1602_char(1, 14);
					a_hou2 = read_lcd1602_char(1, 15);
					
					lcd1602_clear(); //清屏
					write_lcd1602_command(0x0c); //显示屏开显示、不显示光标、光标闪烁
					Keyflag1 = 0; //重置Keyflag1
				}
			}
		}		
	}
}

//调整时钟时间
void Time_Adjust()
{
	if(Keyflag0 == 1)
	{
		lcd1602_clear();            //清屏
		lcd1602_alarm_init();       //光标闪烁
		Screen2_interface();         //屏幕2界面
		Screen2_digit_interface();  //屏幕2数字显示
		write_lcd1602_command(0x10);
		
		while(Keyflag0 == 1)
		{
			lcd1602_course_move(); //光标移动指令
			lcd1602_change_number(); //改变lcd1602屏幕数字
			
			DelayMs(200); //设置屏幕2刷新率
			
			//按下K3退出屏幕2
			if(K3 == 0)
			{
				DelayMs(10);
				if(K3 == 0)
				{
					while(K3 == 0);
					
					Clock_value_adjust();
					
					lcd1602_clear(); //清屏
					write_lcd1602_command(0x0c); //显示屏开显示、不显示光标、光标闪烁
					Keyflag0 = 0;
				}
			}
		}
	}
}

//给时钟写入调整后的初值
void Clock_value_adjust()
{
	s_sec1 = read_lcd1602_char(1, 15);
	s_sec2 = read_lcd1602_char(1, 14);
	s_sec = s_sec1 + s_sec2 * 10;
	
	s_min1 = read_lcd1602_char(1, 11);
	s_min2 = read_lcd1602_char(1, 12);
	s_min = s_min1 + s_min2 * 10;
	
	s_hou1 = read_lcd1602_char(1, 14);
	s_hou2 = read_lcd1602_char(1, 15);
	s_hou = s_hou1 + s_hou2 * 10;
	
	ds1302_write_data(0x8e, 0); //关闭写保护
	ds1302_write_data(0x80, s_sec); 
	ds1302_write_data(0x8e, 0x80); //开启写保护
}

//时钟初值设置
void Clock_initial_value()
{
	ds1302_write_data(0x8e, 0); //关闭写保护
	ds1302_write_data(0x80, 0x1e); 			 //秒初值
	ds1302_write_data(0x82, dat_to_BCD(30)); //分初值
	ds1302_write_data(0x84, dat_to_BCD(10)); //时初值
	ds1302_write_data(0x86, dat_to_BCD(18)); //日
	ds1302_write_data(0x88, dat_to_BCD(3)); //月
	ds1302_write_data(0x8c, dat_to_BCD(22)); //年 
	ds1302_write_data(0x8e, 0x80); //开启写保护
}

/*
//闹钟响铃
void Alarm_start()
{
	unsigned char i;
	if(s_hou1 == a_hou1 && s_hou2 == a_hou2 && s_min1 == a_min1 && s_min2 == a_min2 && s_sec1 == a_sec1 && s_sec2 == a_sec2)
	{
		for(i = 0; i < 3000; i++)
		{
			BEEP();
		}
	}
}
*/





//外部中断INT0设置
void INT0_S()
{
	EA = 1;
	EX0 = 1;
	IT0 = 1;
}

//外部中断INT0功能
void INT0_Z() interrupt 1
{
	if(Keyflag0 == 0)
	{
		Keyflag0 = 1;
	}	
}	

//外部中断INT1设置
void INT1_S()
{
	EA = 1;
	EX1 = 1;
	IT1 = 1;
}

//外部中断INT1功能
void INT1_Z() interrupt 2
{
	if(Keyflag1 == 0)
	{
		Keyflag1 = 1;
	}
}

//延时函数
void DelayUs(unsigned char tu)
{
	while(--tu);
}

void DelayMs(unsigned char tm)
{
	while(tm--)
	{
		DelayUs(248);
		DelayUs(248);
	}
}