void ds1302_write_data(unsigned char cmd, unsigned char dat); //��DS1302д������

unsigned char ds1302_read(unsigned char cmd); //��DS1302��������

unsigned char dat_to_BCD(unsigned char dat); //ת��

unsigned char BCD_to_dat(unsigned char dat); //ת��