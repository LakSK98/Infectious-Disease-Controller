#define LCD2_Port PORTA			/* Define LCD data port */
#define RS2 PD4				/* Define Register Select pin */
#define EN2 PD5				/* Define Enable signal pin */

void LCD2_Command( unsigned char cmnd )
{
	LCD2_Port = (LCD2_Port & 0x0F) | (cmnd & 0xF0); /* sending upper nibble */
	PORTD &= ~ (1<<RS2);		/* RS=0, command reg. */
	PORTD |= (1<<EN2);		/* Enable pulse */
	_delay_us(1);
	PORTD &= ~ (1<<EN2);

	_delay_us(200);

	LCD2_Port = (LCD2_Port & 0x0F) | (cmnd << 4);  /* sending lower nibble */
	PORTD |= (1<<EN2);
	_delay_us(1);
	PORTD &= ~ (1<<EN2);
	_delay_ms(2);
}


void LCD2_Char( unsigned char data )
{
	LCD2_Port = (LCD2_Port & 0x0F) | (data & 0xF0); /* sending upper nibble */
	PORTD |= (1<<RS2);		/* RS=1, data reg. */
	PORTD|= (1<<EN2);
	_delay_us(1);
	PORTD &= ~ (1<<EN2);

	_delay_us(200);

	LCD2_Port = (LCD2_Port & 0x0F) | (data << 4); /* sending lower nibble */
	PORTD |= (1<<EN2);
	_delay_us(1);
	PORTD &= ~ (1<<EN2);
	_delay_ms(2);
}

void LCD2_Init (void)			/* LCD Initialize function */
{
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
	LCD2_Command(0x02);		/* send for 4 bit initialization of LCD  */
	LCD2_Command(0x28);              /* 2 line, 5*7 matrix in 4-bit mode */
	LCD2_Command(0x0c);              /* Display on cursor off*/
	LCD2_Command(0x06);              /* Increment cursor (shift cursor to right)*/
	LCD2_Command(0x01);              /* Clear display screen*/
	_delay_ms(2);
}


void LCD2_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD2_Char (str[i]);
	}
}

void LCD2_String_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD2_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD2_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD2_String(str);		/* Call LCD string function */
}

void LCD2_Clear()
{
	LCD2_Command (0x01);		/* Clear display */
	_delay_ms(2);
	LCD2_Command (0x80);		/* Cursor at home position */
}