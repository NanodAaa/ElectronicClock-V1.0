void ds1302_write_data(unsigned char cmd, unsigned char dat); //向DS1302写入数据

unsigned char ds1302_read(unsigned char cmd); //从DS1302读出数据

unsigned char dat_to_BCD(unsigned char dat); //转码

unsigned char BCD_to_dat(unsigned char dat); //转码