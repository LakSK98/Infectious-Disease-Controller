
/*
SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
SCL freq  =8000000/(16+2(TWBR).4^0)
100 000 = 800000/(16+2(TWBR).1)
TWBR = 32 = 0x20
*/

void i2c_init(void)
{
	TWBR=0x20;	// set Bit rate value
	TWSR= TWSR & ~(1<<TWPS1) & ~(1<<TWPS0);	//set prescalar value
}

void i2c_start(void)
{
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));         
	while((TWSR & 0xF8)!= 0x08);       
}

void i2c_repeated_start(void)
{
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));       
	while((TWSR & 0xF8)!= 0x10);
}

void i2c_write_address(unsigned char data)
{
	TWDR=data;
	TWCR=(1<<TWINT)|(1<<TWEN);  
	while (!(TWCR & (1<<TWINT))); 
	while((TWSR & 0xF8)!= 0x18);
}

void i2c_read_address(unsigned char data)
{
	TWDR=data;
	TWCR=(1<<TWINT)|(1<<TWEN);    
	while (!(TWCR & (1<<TWINT)));
	while((TWSR & 0xF8)!= 0x40);
}

void i2c_write_data(unsigned char data)
{
	TWDR=data;
	TWCR=(1<<TWINT)|(1<<TWEN);  
	while (!(TWCR & (1<<TWINT))); 
	while((TWSR & 0xF8) != 0x28);
}

unsigned char i2c_read_data_ACK(unsigned char ack)
{
	
	if(ack)
	{
		TWCR|=(1<<TWEA);
		TWCR|=(1<<TWEN);
		TWCR|=(1<<TWINT); 
		while (!(TWCR & (1<<TWINT)));
		while((TWSR & 0xF8) != 0x50 );
		return TWDR;
	}
	else
	{
		TWCR&=(~(1<<TWEA));
		TWCR|=(1<<TWEN);
		TWCR|=(1<<TWINT); 
		while (!(TWCR & (1<<TWINT))); 
		while((TWSR & 0xF8) != 0x58);
		return TWDR;
	}

}

void i2c_stop(void)
{
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while(TWCR & (1<<TWSTO));
}