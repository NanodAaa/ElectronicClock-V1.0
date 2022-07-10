/*
*		DS1302.c
*/

//#include "reg52.h"
#include "ElectricClock.h"


// DS1302写入数据
void Ds1302WriteByte(BYTE byComByte, BYTE byData)
{
	int i;

	CE = 0;
	SCLK = 0;
	CE = 1;

	// 写命令字节
	for(i = 0; i < 8; i++)
	{
		SCLK = 0;
		IO = (byComByte & 0x01); // 最低位先传
		SCLK = 1;
		byComByte >>= 1;
		
	}

	// 周期写入数据
	for(i = 0; i < 8; i++)
	{
		SCLK = 0;
		IO = (byData & 0x01);
		SCLK = 1;
		byData >>= 1;
	}

	CE = 0; // 拉低使能端，关闭读写功能

	DelayMs(2);
	
}


// DS1302读取数据
BYTE Ds1302ReadByte(BYTE byComByte)
{
	BYTE byData;
	int i;

	CE = 0;
	SCLK = 0;
	CE = 1;

	// 写命令字节
	for(i = 0; i < 8; i++)
	{
		SCLK = 0;
		IO = (byComByte & 0x01); // 最低位先传
		SCLK = 1;
		byComByte >>= 1;
		
	}
	
	// 周期读出数据
	for(i = 0; i < 8; i++)
	{
		SCLK = 0;
		byData >>= 1;
		if(IO)
		{
			byData |= 0x80; // 对最高位置高
		}
		SCLK = 1;
		
	}

	CE = 0;

	DelayMs(1);
	
	return byData;

}


// 开关写保护
void Ds1302WriteProtect(BOOL bMode)
{
	// 打开写保护
	if(bMode == TRUE)
	{
		Ds1302WriteByte(0x8e, 0x80);
	}
	// 关闭写保护
	if(bMode == FALSE)
	{
		Ds1302WriteByte(0x8e, 0);
	}
}


// dat转BCD
BYTE DatToBcd(BYTE byDat)//数据转BCD码
{
	BYTE byDat1, byDat2;
	
	byDat1 = byDat / 10;
	byDat2 = byDat % 10;
	byDat2 = byDat2 + byDat1 * 16;
	
	return byDat2;
}


// BCD转dat
BYTE BcdToDat(BYTE byDat)
{
	BYTE byDat1, byDat2;
	
	byDat1 = byDat / 16;
	byDat2 = byDat % 16;
	byDat2 = byDat2 + byDat1 * 10;
	
	return byDat2;
}













