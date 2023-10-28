#include <avr/io.h>
#include <util/delay.h>
//Standard speed
#define I2C_TLOW 4.7
#define I2C_THIGH 4.0


#define SDA_HIGH() {PORTB |= (1 << PB0);}
#define SDA_LOW()  {PORTB &= ~(1 << PB0);}

#define SCL_HIGH() {PORTB |= (1 << PB2);}
#define SCL_LOW()  {PORTB &= ~(1 << PB2);}

#define SDA_OUT()  {DDRB |= (1 << PB0);}
#define SDA_IN()   {DDRB &= ~(1 << PB0);}

#define SCL_OUT()  {DDRB |= (1 << PB2);}
#define SCL_IN()   {DDRB &= ~(1 << PB2);}

#define WAIT_HIGH() { _delay_us(I2C_THIGH); }
#define WAIT_LOW()  { _delay_us(I2C_TLOW);  } 


#define USISR_TRANSFER_8_BIT 0b11110000 | (0x00)
#define USISR_TRANSFER_1_BIT 0b11110000 | (0b00001110)


//Set 2-wire USI (i2c),  internal clock, toggle clock port
#define CLOCK_STROBE() {USICR |= (1 << USIWM1) | (1 << USICS1) | (1 << USICLK) | (1 << USITC);}

char send() {
	char temp = USIDR;
	do {
		WAIT_LOW();
		CLOCK_STROBE(); //Positive edge SCL
		while(!(PINB & (1 << PB2))); //Wait for scl to go high	
		WAIT_HIGH();
		CLOCK_STROBE(); //Negative edge SCL
	} while(!(USISR & (1 << USIOIF))); //Cointinue until counter Overflow, could be interrupt instead of poll
	WAIT_LOW(); 
	//USIDR = 0xFF;
	//SDA_OUT();
	return temp;
}
void start_transfer() {
	_delay_ms(10);
	USIDR = 0xff;
	SCL_HIGH();
	SDA_HIGH();
	SDA_OUT();
	SCL_OUT();
	//CLOCK_STROBE();	
	//Start condition fig 15-5 p. 113

	//SCL_HIGH();
	//Wait for SCL to go high
	while(!(PINB & (1 << PB2)));	

	WAIT_LOW();
	SDA_LOW();
	WAIT_HIGH();
	SCL_LOW();
	WAIT_LOW();
	SDA_HIGH();	
	
	//Address and r/w
	//SCL_LOW();
	USIDR = 0x78; //Data
	USISR = USISR_TRANSFER_8_BIT; //Amount of data
	send();
	
	SDA_IN();
	USISR = USISR_TRANSFER_1_BIT;
	//Check for ack
	if((send() & (1 << 0))) {
		//Error-LED
		//PORTB |= (1 << PB3);
	}
	WAIT_LOW();
	SDA_OUT();
}
void stop() {
	SDA_LOW();
	WAIT_LOW();
	SCL_IN();
	while(!(PINB & (1 << PB2)));
	WAIT_HIGH();
	SDA_IN();
}
int main() {

	_delay_ms(1000);
	
	DDRB |= (1 << PB3);

	start_transfer();	

	
	USIDR = 0x00;
	USISR = USISR_TRANSFER_8_BIT;
	send();
	SDA_IN();
	USISR = USISR_TRANSFER_1_BIT;
	//Check for ack
	if((send() & (1 << 0))) {
		//Error-LED
		PORTB |= (1 << PB3);
	}
	SDA_OUT();
	
	USIDR = 0xA5;
	USISR = USISR_TRANSFER_8_BIT;
	send();
	SDA_IN();
	USISR = USISR_TRANSFER_1_BIT;
	//Check for ack
	if((send() & (1 << 0))) {
		//Error-LED
		PORTB |= (1 << PB3);
	}
	SDA_OUT();

	
	USIDR = 0x00;
	USISR = USISR_TRANSFER_8_BIT;
	send();
	SDA_IN();
	USISR = USISR_TRANSFER_1_BIT;
	//Check for ack
	if((send() & (1 << 0))) {
		//Error-LED
		PORTB |= (1 << PB3);
	}
	SDA_OUT();
	
	USIDR = 0xAF;
	USISR = USISR_TRANSFER_8_BIT;
	send();
	SDA_IN();
	USISR = USISR_TRANSFER_1_BIT;
	//Check for ack
	if((send() & (1 << 0))) {
		//Error-LED
		PORTB |= (1 << PB3);
	}


	stop();

	for(;;) {

	}


} 
