#include "i2c.h"
#define WRITE 0
#define READ 1
 
void MLX90614_Init()
{
	i2c_init();
}

double MLX90614_Read(unsigned char D_Address,unsigned char R_Address)
{
	int dataH,dataL;
	double Tempdata;
	
	i2c_start();
	i2c_write_address((D_Address<<1)|WRITE);
	i2c_write_data(R_Address);
	i2c_repeated_start();
	i2c_read_address((D_Address<<1)|READ);
	dataL=i2c_read_data_ACK(1);
	dataH=i2c_read_data_ACK(1);
	i2c_read_data_ACK(0);
	i2c_stop();
	_delay_ms(1);
	
	Tempdata = ((dataH & 0x7F) << 8) + dataL;
	return Tempdata;
}