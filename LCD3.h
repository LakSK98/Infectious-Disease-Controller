#define LCD3_Port PORTA			/* Define LCD data port */
#define RS3 PA0				/* Define Register Select pin */
#define EN3 PA1				/* Define Enable signal pin */

void LCD3_Command( unsigned char cmnd )
{
	LCD3_Port = (LCD3_Port & 0xF0) | (cmnd>>4); /* sending upper nibble */
	PORTA &= ~ (1<<RS3);		/* RS=0, command reg. */
	PORTA |= (1<<EN3);		/* Enable pulse */
	_delay_us(1);
	PORTA &= ~ (1<<EN3);

	_delay_us(200);

	LCD3_Port = (LCD3_Port & 0xF0) | (cmnd & 0x0F);  /* sending lower nibble */
	PORTA |= (1<<EN3);
	_delay_us(1);
	PORTA &= ~ (1<<EN3);
	_delay_ms(2);
}


void LCD3_Char( unsigned char data )
{
	LCD3_Port = (LCD3_Port & 0xF0) | (data >>4); /* sending upper nibble */
	PORTA |= (1<<RS3);		/* RS=1, data reg. */
	PORTA|= (1<<EN3);
	_delay_us(1);
	PORTA &= ~ (1<<EN3);

	_delay_us(200);

	LCD3_Port = (LCD3_Port & 0xF0) | (data & 0x0F); /* sending lower nibble */
	PORTA |= (1<<EN3);
	_delay_us(1);
	PORTA &= ~ (1<<EN3);
	_delay_ms(2);
}

void LCD3_Init (void)			/* LCD Initialize function */
{
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
	LCD3_Command(0x02);		/* send for 4 bit initialization of LCD  */
	LCD3_Command(0x28);              /* 2 line, 5*7 matrix in 4-bit mode */
	LCD3_Command(0x0c);              /* Display on cursor off*/
	LCD3_Command(0x06);              /* Increment cursor (shift cursor to right)*/
	LCD3_Command(0x01);              /* Clear display screen*/
	_delay_ms(2);
}


void LCD3_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD3_Char (str[i]);
	}
}

void LCD3_String_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD3_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD3_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD3_String(str);		/* Call LCD string function */
}

void LCD3_Clear()
{
	LCD3_Command (0x01);		/* Clear display */
	_delay_ms(2);
	LCD3_Command (0x80);		/* Cursor at home position */
}