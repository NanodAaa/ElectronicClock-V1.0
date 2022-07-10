/*
			驱动DS1302相关函数
*/

#include "reg51.h"
#include "main.h"
#include "DS1302.h"

sbit TIO = P3^4;   //输入、输出引脚
sbit TRST = P3^5;  //使能端
sbit TSCLK = P3^6; //输入时钟脉冲

//写入1字节数据
void ds1302_write_data(unsigned char cmd, unsigned char dat)
{
	unsigned char i;
	
	TRST = 0;
	TSCLK = 0;
	TRST = 1; //上升沿允许写数据
	
	//写入命令
	for(i = 0; i < 8; i++)
	{
		TSCLK = 0;
		TIO = cmd&0x01; //从最低位开始传送
		TSCLK = 1;
		cmd >>= 1;
	}
	
	//写入数据
	for(i = 0; i < 8; i++)
	{
		TSCLK = 0;
		TIO = dat&0x01;
		TSCLK = 1;
		dat >>= 1; 
	}
	
	TRST = 0; //拉低禁止写数据
}

//读1字节数据
unsigned char ds1302_read(unsigned char cmd)
{
	unsigned char dat, i;
	
	TRST = 0;
	TSCLK = 0;
	TRST = 1; //上升沿允许写数据
	
	//写入命令
	for(i = 0; i < 8; i++)
	{
		TSCLK = 0;
		TIO = cmd&0x01;
		TSCLK = 1;
		cmd >>= 1;
	}
	
	//读出数据
	for(i = 0; i < 8; i++)
	{
		TSCLK = 0;
		dat >>= 1;
		if(TIO)
		{
			dat |= 0x80; //按位异或
		}
		TSCLK = 1;
	}
	
	TRST = 0; //拉低禁止写数据
	
	return dat;
}

//2位数据转BCD
unsigned char dat_to_BCD(unsigned char dat)
{
	unsigned char dat1, dat2;
	
	dat1 = dat / 10;
	dat2 = dat % 10;
	dat2 = dat2 + dat1 * 16;
	
	return dat2;
}

//BCD转2位数据
unsigned char BCD_to_dat(unsigned char dat)
{
	unsigned char dat1, dat2;
	
	dat1 = dat / 16;
	dat2 = dat % 16;
	dat2 = dat2 + dat1 * 10;
	
	return dat2;
}