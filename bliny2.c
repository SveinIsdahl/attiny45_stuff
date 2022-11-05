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




void start_transfer() {
	
	SDA_OUT();
	SCL_OUT();

	//Set 2-wire USI (i2c),  internal clock, toggle clock port
	USICR |= (1 << USIWM0) | (1 << USIWM1) | (1 << USICLK) | (1 << USICLK);
	
	//Start condition fig 15-5 p. 113

	SCL_HIGH();
	//SHITTY FIX, should wait for SCL to go high
	WAIT_LOW();

	WAIT_LOW();
	SDA_LOW();
	WAIT_HIGH();
	SCL_LOW();
	WAIT_LOW();
	SDA_HIGH();	
	//Address
	send(0x3c);
	

}
void send(char *msg) {
	SCL_LOW();
	USIDR = *(msg);
	//Increment for ACK
	msg += 1;
	


}
int main() {

	start_transfer();	
	for(;;) {
		
	}


} 
