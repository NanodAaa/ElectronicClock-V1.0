unsigned char Busy_Check(); //���LCD1602��æ

void write_lcd1602_command(unsigned char com); //��LCD1602д������

void lcd1602_write_data(unsigned char dat); //��LCD1602д������

void lcd1602_write_char(unsigned char Hang, unsigned char Lie, unsigned char n); //��LCD1602д���ַ�

void lcd1602_write_string(unsigned char Hang1, unsigned char Lie1, unsigned char *s); //��LCD1602д���ַ���

void lcd1602_clear(); //����

void lcd1602_init();  //��ʾ��ʼ��

void lcd1602_alarm_init(); //�������ó�ʼ������

void lcd1602_course_move(); //����ƶ�ָ��

void lcd1602_change_number(); //�ı�lcd1602��Ļ����

unsigned char lcd1602_read_data(); //��lcd1602�ж�ȡ����

unsigned char read_lcd1602_char(unsigned char hang, unsigned char lie); //��ȡ�ַ�