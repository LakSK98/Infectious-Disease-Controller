#define LCD1_Port PORTB			/* Define LCD data port */
#define RS1 PD4				/* Define Register Select pin */
#define EN1 PD5				/* Define Enable signal pin */

void LCD1_Command( unsigned char cmnd )
{
	LCD1_Port = (LCD1_Port & 0x0F) | (cmnd & 0xF0); /* sending upper nibble */
	PORTD &= ~ (1<<RS1);		/* RS=0, command reg. */
	PORTD |= (1<<EN1);		/* Enable pulse */
	_delay_us(1);
	PORTD &= ~ (1<<EN1);

	_delay_us(200);

	LCD1_Port = (LCD1_Port & 0x0F) | (cmnd << 4);  /* sending lower nibble */
	PORTD |= (1<<EN1);
	_delay_us(1);
	PORTD &= ~ (1<<EN1);
	_delay_ms(2);
}


void LCD1_Char( unsigned char data )
{
	LCD1_Port = (LCD1_Port & 0x0F) | (data & 0xF0); /* sending upper nibble */
	PORTD |= (1<<RS1);		/* RS=1, data reg. */
	PORTD|= (1<<EN1);
	_delay_us(1);
	PORTD &= ~ (1<<EN1);

	_delay_us(200);

	LCD1_Port = (LCD1_Port & 0x0F) | (data << 4); /* sending lower nibble */
	PORTD |= (1<<EN1);
	_delay_us(1);
	PORTD &= ~ (1<<EN1);
	_delay_ms(2);
}

void LCD1_Init (void)			/* LCD Initialize function */
{
	
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
	LCD1_Command(0x02);		/* send for 4 bit initialization of LCD  */
	LCD1_Command(0x28);              /* 2 line, 5*7 matrix in 4-bit mode */
	LCD1_Command(0x0c);              /* Display on cursor off*/
	LCD1_Command(0x06);              /* Increment cursor (shift cursor to right)*/
	LCD1_Command(0x01);              /* Clear display screen*/
	_delay_ms(2);
}


void LCD1_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD1_Char (str[i]);
	}
}

void LCD1_String_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD1_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD1_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD1_String(str);		/* Call LCD string function */
}

void LCD1_Clear()
{
	LCD1_Command (0x01);		/* Clear display */
	_delay_ms(2);
	LCD1_Command (0x80);		/* Cursor at home position */
}
