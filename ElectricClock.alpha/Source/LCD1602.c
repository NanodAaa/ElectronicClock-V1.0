/*
				LCD1602相关函数
				!!!Encode in ANSI!!!
*/


#include "intrins.h"
#include "LCD1602.h"
#include "main.h"
#include "reg51.h"

#define DATA P0 //LCD1602数据输入/输出

bit Moveflag;

//LCD1602
sbit W_R = P2^6; 
sbit RS = P2^5;
sbit E = P2^7;

//独立按键
sbit K1 = P3^2;
sbit K2 = P3^3;
sbit K3 = P3^4; //INT0
sbit K4 = P3^5; //INT1

unsigned char i = 0x30; //
unsigned char j;

extern bit Keyflag0;

//检测繁忙
unsigned char lcd1602_check_busy()
{
	unsigned char ucData;
	RS = 0;
	W_R = 1;
	DelayUs(10);
	E = 0;
	DelayUs(10);
	E = 1;
	DelayUs(10);		//模式设为读取忙标志
	ucData = DATA;
	//返回“1”意味繁忙，返回“0”意味空闲
	
	return (DATA&0x80); // 1表示忙，0表示空闲
}

//写入指令
void write_lcd1602_command(unsigned char com)
{
	while(lcd1602_check_busy()); //空闲时执行写入命令
	
	RS = 0;
	W_R = 0; //模式设为写入命令
	E = 1;   //允许写入指令
	DelayUs(10);
	
	DATA = com; //向lcd1602写入命令
	
	DelayUs(10);
	E = 0;
	DelayUs(5);
}

//从lcd1602中读取数据
unsigned char read_lcd1602_data()
{
	unsigned char dat;
	while(lcd1602_check_busy());
	
	RS = 1;  //选中数据寄存器
	W_R = 1; //设为读信息
	E = 1;   //允许读信息
	DelayMs(5);
	
	dat = DATA; //从lcd1602读取数据
	
	return dat; //返回读取的数据 dat
}

//读取字符
unsigned char read_lcd1602_char(unsigned char hang, unsigned char lie)
{
	unsigned char dat;
	
	//设定显示的位置：第1行，及列数
	if(hang == 0)
	{
		write_lcd1602_command(0x80 + lie);
	}
	//设定显示的位置：第2行，及列数
	else if(hang == 1)
	{
		write_lcd1602_command(0xc0 + lie);
	}
	
	//从lcd1602读取数据
	dat = read_lcd1602_data();
	
	return dat; //返回读取的字符dat
}

//写入数据
void write_lcd1602_data(unsigned char dat)
{
	while(lcd1602_check_busy()); //空闲时执行写入命令
	
	RS = 1;
	W_R = 0;
	E = 1;
	DelayUs(10);
	
	DATA = dat; //输入数据
	
	DelayUs(10);
	E = 0;
	DelayUs(10);
}

//写入字符     					行					列				字符
void lcd1602_write_char(unsigned char Hang, unsigned char Lie, unsigned char n)
{
	//设定显示的位置：第1行，及列数
	if(Hang == 0)
	{
		write_lcd1602_command(0x80 + Lie); //设定显示的位置
	}
	//设定显示的位置：第2行，及列数
	else
	{
		write_lcd1602_command(0xC0 + Lie);
	}
	
	//向lcd1602写入字符
	write_lcd1602_data(n); //写入字符'n'的ASCII
	
}

//写入字符串
void lcd1602_write_string(unsigned char Hang1, unsigned char Lie1, unsigned char *s)
{
	//设定显示的位置：第1行，及列数
	if(Hang1 == 0)
	{
		write_lcd1602_command(0x80 + Lie1); 
	}
	//设定显示的位置：第2行，及列数
	else
	{
		write_lcd1602_command(0xC0 + Lie1);
	}
	
	//向lcd1602输入字符串
	while(*s)
	{
		write_lcd1602_data(*s);
		s++;
	}
	
}

//清屏
void lcd1602_clear()
{
	write_lcd1602_command(0x01);
	DelayMs(5);
}

//按要求初始化
void lcd1602_init()
{
	write_lcd1602_command(0x38); //功能：8位总线、2行显示、5*7点阵
	DelayMs(5);
	write_lcd1602_command(0x08); //功能：显示屏关显示、不显示光标、光标闪烁
	write_lcd1602_command(0x01); //清屏
	write_lcd1602_command(0x06); //功能：输入数据时光标右移、输入字符时显示屏全部显示不移动
	DelayMs(5);
	write_lcd1602_command(0x0c); //显示屏开显示、不显示光标、光标闪烁
}

//闹钟设置初始化命令
void lcd1602_alarm_init()
{
	write_lcd1602_command(0x0f); //显示屏开显示、显示光标、光标闪烁
}	

//光标移动指令
void lcd1602_course_move()
{
	//光标左移
	if(K1 == 0)
	{
		DelayMs(10);
		if(K1 == 0)
		{
			while(K1 == 0);
									
			j++;
			
			//跳过':'
			if(j == 2)
			{
				write_lcd1602_command(0x10);
			}			
			else if(j == 4)
			{
				write_lcd1602_command(0x10);
			}
			
			//将光标跳到第1行最右
			else if(j == 6)
			{
				write_lcd1602_command(0x02); //光标归位
				DelayMs(5);
				
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				write_lcd1602_command(0x14);
				DelayUs(5);
				//write_lcd1602_command(0x14);
				//DelayUs(5);
				//write_lcd1602_command(0x14);
				//DelayUs(5);
				//write_lcd1602_command(0x14);
				
			}
			
			else if(j == 8)
			{
				write_lcd1602_command(0x10);
			}
			else if(j == 10)
			{
				write_lcd1602_command(0x10);
			}
			
			else if(j == 14)
			{
				j = 0;
				
				lcd1602_write_char(1, 15, '0');
			}
			
			if(K3 == 0)
			{
				DelayMs(5);
				if(K3 == 0)
				{
					while(K3 == 0);
					
					j = 0;
				}
			}
			
			if(Keyflag0 == 0)
			{
				j = 0;
			}
			
			Moveflag = 1; //向改变lcd1602中发送flag，清零i
			write_lcd1602_command(0x10); //左移
		}	
	}
}

//改变lcd1602屏幕数字
void lcd1602_change_number()
{	
	//按下K2当前位置数字 +1
	if(K2 == 0)
	{
		DelayMs(10);
		if(K2 == 0)
		{
			while(K2 == 0);
					
			write_lcd1602_data(i); //向lcd1602中写入 i 的值  
			write_lcd1602_command(0x10); //写入后左移1位
			
			i++;
		}
	}
		
	//按下K1 光标左移时i重置
	if(Moveflag == 1)
	{
		i = 0x30;
		Moveflag = 0;
	}
	
	//i=10时i重置
	else if(i == 0x3a)
	{
		i = 0x30;
	}
	
	//按下K4 退出屏幕三时重置i
	else if(K4 == 0)
	{
		i = 0x30;
	}
}	