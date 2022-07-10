/*
				LCD1602��غ���
				!!!Encode in ANSI!!!
*/


#include "intrins.h"
#include "LCD1602.h"
#include "main.h"
#include "reg51.h"

#define DATA P0 //LCD1602��������/���

bit Moveflag;

//LCD1602
sbit W_R = P2^6; 
sbit RS = P2^5;
sbit E = P2^7;

//��������
sbit K1 = P3^2;
sbit K2 = P3^3;
sbit K3 = P3^4; //INT0
sbit K4 = P3^5; //INT1

unsigned char i = 0x30; //
unsigned char j;

extern bit Keyflag0;

//��ⷱæ
unsigned char lcd1602_check_busy()
{
	unsigned char ucData;
	RS = 0;
	W_R = 1;
	DelayUs(10);
	E = 0;
	DelayUs(10);
	E = 1;
	DelayUs(10);		//ģʽ��Ϊ��ȡæ��־
	ucData = DATA;
	//���ء�1����ζ��æ�����ء�0����ζ����
	
	return (DATA&0x80); // 1��ʾæ��0��ʾ����
}

//д��ָ��
void write_lcd1602_command(unsigned char com)
{
	while(lcd1602_check_busy()); //����ʱִ��д������
	
	RS = 0;
	W_R = 0; //ģʽ��Ϊд������
	E = 1;   //����д��ָ��
	DelayUs(10);
	
	DATA = com; //��lcd1602д������
	
	DelayUs(10);
	E = 0;
	DelayUs(5);
}

//��lcd1602�ж�ȡ����
unsigned char read_lcd1602_data()
{
	unsigned char dat;
	while(lcd1602_check_busy());
	
	RS = 1;  //ѡ�����ݼĴ���
	W_R = 1; //��Ϊ����Ϣ
	E = 1;   //�������Ϣ
	DelayMs(5);
	
	dat = DATA; //��lcd1602��ȡ����
	
	return dat; //���ض�ȡ������ dat
}

//��ȡ�ַ�
unsigned char read_lcd1602_char(unsigned char hang, unsigned char lie)
{
	unsigned char dat;
	
	//�趨��ʾ��λ�ã���1�У�������
	if(hang == 0)
	{
		write_lcd1602_command(0x80 + lie);
	}
	//�趨��ʾ��λ�ã���2�У�������
	else if(hang == 1)
	{
		write_lcd1602_command(0xc0 + lie);
	}
	
	//��lcd1602��ȡ����
	dat = read_lcd1602_data();
	
	return dat; //���ض�ȡ���ַ�dat
}

//д������
void write_lcd1602_data(unsigned char dat)
{
	while(lcd1602_check_busy()); //����ʱִ��д������
	
	RS = 1;
	W_R = 0;
	E = 1;
	DelayUs(10);
	
	DATA = dat; //��������
	
	DelayUs(10);
	E = 0;
	DelayUs(10);
}

//д���ַ�     					��					��				�ַ�
void lcd1602_write_char(unsigned char Hang, unsigned char Lie, unsigned char n)
{
	//�趨��ʾ��λ�ã���1�У�������
	if(Hang == 0)
	{
		write_lcd1602_command(0x80 + Lie); //�趨��ʾ��λ��
	}
	//�趨��ʾ��λ�ã���2�У�������
	else
	{
		write_lcd1602_command(0xC0 + Lie);
	}
	
	//��lcd1602д���ַ�
	write_lcd1602_data(n); //д���ַ�'n'��ASCII
	
}

//д���ַ���
void lcd1602_write_string(unsigned char Hang1, unsigned char Lie1, unsigned char *s)
{
	//�趨��ʾ��λ�ã���1�У�������
	if(Hang1 == 0)
	{
		write_lcd1602_command(0x80 + Lie1); 
	}
	//�趨��ʾ��λ�ã���2�У�������
	else
	{
		write_lcd1602_command(0xC0 + Lie1);
	}
	
	//��lcd1602�����ַ���
	while(*s)
	{
		write_lcd1602_data(*s);
		s++;
	}
	
}

//����
void lcd1602_clear()
{
	write_lcd1602_command(0x01);
	DelayMs(5);
}

//��Ҫ���ʼ��
void lcd1602_init()
{
	write_lcd1602_command(0x38); //���ܣ�8λ���ߡ�2����ʾ��5*7����
	DelayMs(5);
	write_lcd1602_command(0x08); //���ܣ���ʾ������ʾ������ʾ��ꡢ�����˸
	write_lcd1602_command(0x01); //����
	write_lcd1602_command(0x06); //���ܣ���������ʱ������ơ������ַ�ʱ��ʾ��ȫ����ʾ���ƶ�
	DelayMs(5);
	write_lcd1602_command(0x0c); //��ʾ������ʾ������ʾ��ꡢ�����˸
}

//�������ó�ʼ������
void lcd1602_alarm_init()
{
	write_lcd1602_command(0x0f); //��ʾ������ʾ����ʾ��ꡢ�����˸
}	

//����ƶ�ָ��
void lcd1602_course_move()
{
	//�������
	if(K1 == 0)
	{
		DelayMs(10);
		if(K1 == 0)
		{
			while(K1 == 0);
									
			j++;
			
			//����':'
			if(j == 2)
			{
				write_lcd1602_command(0x10);
			}			
			else if(j == 4)
			{
				write_lcd1602_command(0x10);
			}
			
			//�����������1������
			else if(j == 6)
			{
				write_lcd1602_command(0x02); //����λ
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
			
			Moveflag = 1; //��ı�lcd1602�з���flag������i
			write_lcd1602_command(0x10); //����
		}	
	}
}

//�ı�lcd1602��Ļ����
void lcd1602_change_number()
{	
	//����K2��ǰλ������ +1
	if(K2 == 0)
	{
		DelayMs(10);
		if(K2 == 0)
		{
			while(K2 == 0);
					
			write_lcd1602_data(i); //��lcd1602��д�� i ��ֵ  
			write_lcd1602_command(0x10); //д�������1λ
			
			i++;
		}
	}
		
	//����K1 �������ʱi����
	if(Moveflag == 1)
	{
		i = 0x30;
		Moveflag = 0;
	}
	
	//i=10ʱi����
	else if(i == 0x3a)
	{
		i = 0x30;
	}
	
	//����K4 �˳���Ļ��ʱ����i
	else if(K4 == 0)
	{
		i = 0x30;
	}
}	