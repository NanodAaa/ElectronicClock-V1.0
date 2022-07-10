/*
			����DS1302��غ���
*/

#include "reg51.h"
#include "main.h"
#include "DS1302.h"

sbit TIO = P3^4;   //���롢�������
sbit TRST = P3^5;  //ʹ�ܶ�
sbit TSCLK = P3^6; //����ʱ������

//д��1�ֽ�����
void ds1302_write_data(unsigned char cmd, unsigned char dat)
{
	unsigned char i;
	
	TRST = 0;
	TSCLK = 0;
	TRST = 1; //����������д����
	
	//д������
	for(i = 0; i < 8; i++)
	{
		TSCLK = 0;
		TIO = cmd&0x01; //�����λ��ʼ����
		TSCLK = 1;
		cmd >>= 1;
	}
	
	//д������
	for(i = 0; i < 8; i++)
	{
		TSCLK = 0;
		TIO = dat&0x01;
		TSCLK = 1;
		dat >>= 1; 
	}
	
	TRST = 0; //���ͽ�ֹд����
}

//��1�ֽ�����
unsigned char ds1302_read(unsigned char cmd)
{
	unsigned char dat, i;
	
	TRST = 0;
	TSCLK = 0;
	TRST = 1; //����������д����
	
	//д������
	for(i = 0; i < 8; i++)
	{
		TSCLK = 0;
		TIO = cmd&0x01;
		TSCLK = 1;
		cmd >>= 1;
	}
	
	//��������
	for(i = 0; i < 8; i++)
	{
		TSCLK = 0;
		dat >>= 1;
		if(TIO)
		{
			dat |= 0x80; //��λ���
		}
		TSCLK = 1;
	}
	
	TRST = 0; //���ͽ�ֹд����
	
	return dat;
}

//2λ����תBCD
unsigned char dat_to_BCD(unsigned char dat)
{
	unsigned char dat1, dat2;
	
	dat1 = dat / 10;
	dat2 = dat % 10;
	dat2 = dat2 + dat1 * 16;
	
	return dat2;
}

//BCDת2λ����
unsigned char BCD_to_dat(unsigned char dat)
{
	unsigned char dat1, dat2;
	
	dat1 = dat / 16;
	dat2 = dat % 16;
	dat2 = dat2 + dat1 * 10;
	
	return dat2;
}