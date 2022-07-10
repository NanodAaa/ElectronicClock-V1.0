/*
			DS1302 x LCD1602 ʱ�ӣ��������ӹ���
*/


#include "reg51.h"
#include "LCD1602.h"
#include "DS1302.h"

bit Keyflag0; //INT0��־
bit Keyflag1; //INT1��־

//��������
sbit K1 = P3^2;
sbit K2 = P3^3;
sbit K3 = P3^4; //INT0
sbit K4 = P3^5; //INT1

//ȫ��ʱ�����
unsigned char g_sec;
unsigned char g_sec1; //��1λ
unsigned char g_sec2; //��10λ
unsigned char g_min;
unsigned char g_min1; //��1λ
unsigned char g_min2; //��10λ
unsigned char g_hou;  
unsigned char g_hou1; //ʱ1λ
unsigned char g_hou2; //ʱ10λ
unsigned char g_day;
unsigned char g_day1; //��1λ
unsigned char g_day2; //��10λ
unsigned char g_mon; 
unsigned char g_mon1; //��1λ
unsigned char g_mon2; //��10λ
unsigned char g_yea;
unsigned char g_yea1; //��1λ
unsigned char g_yea2; //��10λ

//��Ļ2����ʱ�����
unsigned char s_sec1; //��1λ
unsigned char s_sec2; //��10λ
unsigned char s_sec;
unsigned char s_min;
unsigned char s_min1; //��1λ
unsigned char s_min2; //��10λ
unsigned char s_hou;
unsigned char s_hou1; //ʱ1λ
unsigned char s_hou2; //ʱ10λ
unsigned char s_day;
unsigned char s_day1; //��1λ
unsigned char s_day2; //��10λ
unsigned char s_mon;
unsigned char s_mon1; //��1λ
unsigned char s_mon2; //��10λ
unsigned char s_yea;
unsigned char s_yea1; //��1λ
unsigned char s_yea2; //��10λ

//����ʱ�����
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
	//��ʼ��
	//T0_S();
	Clock_initial_value(); //����ʱ�ӳ�ֵ
	INT0_S();
	INT1_S();
	lcd1602_init();
	lcd1602_clear();
	
	//��ѭ��
	while(1)
	{
		//ʱ�䡢������ʾ
		Clock_display();
		
	
		//ʱ�����
		Time_Adjust();
		
		//��������
		Alarm_set();
		
		DelayMs(50); //��Ļˢ���� 50ms
	}
}

//��Ļ1��2����
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

//��Ļ2���ִ���
void Screen2_digit_save()
{
	s_sec1 = g_sec1; //��1λ
	s_sec2 = g_sec2; //��10λ

	s_min1 = g_min1; //��1λ
	s_min2 = g_min2; //��10λ

	s_hou1 = g_hou1; //ʱ1λ
	s_hou2 = g_hou2; //ʱ10λ

	s_day1 = g_day1; //��1λ
	s_day2 = g_day2; //��10λ

	s_mon1 = g_mon1; //��1λ
	s_mon2 = g_mon2; //��10λ

	s_yea1 = g_yea1; //��1λ
	s_yea2 = g_yea2; //��10λ
}

//��Ļ2������ʾ
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
	
	lcd1602_write_char(1, 15, s_sec1); //�̶�λ�ã��𶯣�
}

//��Ļ3����
void Screen3_interface()
{
	lcd1602_clear(); //����
	lcd1602_write_string(1, 0, "ALARM");
	lcd1602_write_string(0, 0, "SET     NanodAaa");
	lcd1602_write_char(1, 8, '0');
	lcd1602_write_char(1, 9, '0');
	lcd1602_write_char(1, 11, '0');
	lcd1602_write_char(1, 12, '0');
	lcd1602_write_char(1, 14, '0');

	lcd1602_write_char(1, 10, 0x3a); //':'
	lcd1602_write_char(1, 13, 0x3a);
	
	lcd1602_write_char(1, 15, '0'); //��Ҫ��
	write_lcd1602_command(0x10);
}

//��Ļ1ʱ������ʾ
void Clock_display()
{
	Screen_interface();
	Time_display();
	Date_display();
}

//����ds1302��ʾʱ��
void Time_display()
{
	//��ȡ����
	ds1302_write_data(0x8e, 0);
	g_sec = BCD_to_dat(ds1302_read(0x81)); 
	ds1302_write_data(0x8e, 0x80);
	//��λ��ʾ
	g_sec1 = 0x30 + (g_sec % 10);
	g_sec2 = 0x30 + (g_sec / 10);
	lcd1602_write_char(1, 15, g_sec1);
	lcd1602_write_char(1, 14, g_sec2);
	
	//��ȡ����
	ds1302_write_data(0x8e, 0);
	g_min = BCD_to_dat(ds1302_read(0x83)); 
	ds1302_write_data(0x8e, 0x80);
	//��λ��ʾ
	g_min1 = 0x30 + (g_min % 10);
	g_min2 = 0x30 + (g_min / 10);
	lcd1602_write_char(1, 12, g_min1);
	lcd1602_write_char(1, 11, g_min2);
	
	//��ȡʱ��
	ds1302_write_data(0x8e, 0);
	g_hou = BCD_to_dat(ds1302_read(0x85)); 
	ds1302_write_data(0x8e, 0x80);
	//ʱλ��ʾ
	g_hou1 = 0x30 + (g_hou % 10);
	g_hou2 = 0x30 + (g_hou / 10);
	lcd1602_write_char(1, 9, g_hou1);
	lcd1602_write_char(1, 8, g_hou2);
}

//����DS1302��ʾ����
void Date_display()
{
	//��ȡ����
	ds1302_write_data(0x8e, 0);
	g_day = BCD_to_dat(ds1302_read(0x87)); 
	ds1302_write_data(0x8e, 0x80);
	
	//��lcd1602д������
	g_day1 = 0x30 + (g_day % 10);
	g_day2 = 0x30 + (g_day / 10);
	lcd1602_write_char(0, 14, g_day2);
	lcd1602_write_char(0, 15, g_day1);
	
	//��ȡ��
	ds1302_write_data(0x8e, 0);
	g_mon = BCD_to_dat(ds1302_read(0x89)); 
	ds1302_write_data(0x8e, 0x80);
	
	//��lcd1602д���·�
	g_mon1 = 0x30 + (g_mon % 10);
	g_mon2 = 0x30 + (g_mon / 10);
	lcd1602_write_char(0, 11, g_mon2);
	lcd1602_write_char(0, 12, g_mon1);
	
	//��ȡ���
	ds1302_write_data(0x8e, 0);
	g_yea = BCD_to_dat(ds1302_read(0x8d)); 
	ds1302_write_data(0x8e, 0x80);
	
	//��lcd1602д�����
	g_yea1 = 0x30 + (g_yea % 10);
	g_yea2 = 0x30 + (g_yea / 10);
	lcd1602_write_char(0, 9, g_yea2);
	lcd1602_write_char(0, 8, g_yea1);
}

//��������
void Alarm_set()
{
	if(Keyflag1 == 1)
	{
		//��Ļ��
		lcd1602_alarm_init(); //�������ó�ʼ������
		Screen3_interface(); //��Ļ������
		
		//����K4������Ļ��
		while(Keyflag1 == 1)
		{
			lcd1602_course_move(); //����ƶ�ָ��
			lcd1602_change_number(); //�ı�lcd1602��Ļ����
			
			DelayMs(200); //������Ļ��ˢ��Ƶ��
					
			//�ٴΰ���K4�˳���Ļ3
			if(K4 == 0)
			{
				DelayMs(10); //����
				if(K4 == 0)
				{
					while(K4 == 0);
					
					//��ȡ����,��Ϊ��������ʱ��
					a_hou1 = read_lcd1602_char(1, 8);
					a_hou2 = read_lcd1602_char(1, 9);
					a_min1 = read_lcd1602_char(1, 11);
					a_min2 = read_lcd1602_char(1, 12);
					a_hou1 = read_lcd1602_char(1, 14);
					a_hou2 = read_lcd1602_char(1, 15);
					
					lcd1602_clear(); //����
					write_lcd1602_command(0x0c); //��ʾ������ʾ������ʾ��ꡢ�����˸
					Keyflag1 = 0; //����Keyflag1
				}
			}
		}		
	}
}

//����ʱ��ʱ��
void Time_Adjust()
{
	if(Keyflag0 == 1)
	{
		lcd1602_clear();            //����
		lcd1602_alarm_init();       //�����˸
		Screen2_interface();         //��Ļ2����
		Screen2_digit_interface();  //��Ļ2������ʾ
		write_lcd1602_command(0x10);
		
		while(Keyflag0 == 1)
		{
			lcd1602_course_move(); //����ƶ�ָ��
			lcd1602_change_number(); //�ı�lcd1602��Ļ����
			
			DelayMs(200); //������Ļ2ˢ����
			
			//����K3�˳���Ļ2
			if(K3 == 0)
			{
				DelayMs(10);
				if(K3 == 0)
				{
					while(K3 == 0);
					
					Clock_value_adjust();
					
					lcd1602_clear(); //����
					write_lcd1602_command(0x0c); //��ʾ������ʾ������ʾ��ꡢ�����˸
					Keyflag0 = 0;
				}
			}
		}
	}
}

//��ʱ��д�������ĳ�ֵ
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
	
	ds1302_write_data(0x8e, 0); //�ر�д����
	ds1302_write_data(0x80, s_sec); 
	ds1302_write_data(0x8e, 0x80); //����д����
}

//ʱ�ӳ�ֵ����
void Clock_initial_value()
{
	ds1302_write_data(0x8e, 0); //�ر�д����
	ds1302_write_data(0x80, 0x1e); 			 //���ֵ
	ds1302_write_data(0x82, dat_to_BCD(30)); //�ֳ�ֵ
	ds1302_write_data(0x84, dat_to_BCD(10)); //ʱ��ֵ
	ds1302_write_data(0x86, dat_to_BCD(18)); //��
	ds1302_write_data(0x88, dat_to_BCD(3)); //��
	ds1302_write_data(0x8c, dat_to_BCD(22)); //�� 
	ds1302_write_data(0x8e, 0x80); //����д����
}

/*
//��������
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





//�ⲿ�ж�INT0����
void INT0_S()
{
	EA = 1;
	EX0 = 1;
	IT0 = 1;
}

//�ⲿ�ж�INT0����
void INT0_Z() interrupt 1
{
	if(Keyflag0 == 0)
	{
		Keyflag0 = 1;
	}	
}	

//�ⲿ�ж�INT1����
void INT1_S()
{
	EA = 1;
	EX1 = 1;
	IT1 = 1;
}

//�ⲿ�ж�INT1����
void INT1_Z() interrupt 2
{
	if(Keyflag1 == 0)
	{
		Keyflag1 = 1;
	}
}

//��ʱ����
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