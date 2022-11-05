#include <avr/io.h>
#include <util/delay.h>

int main() {
	DDRB |= _BV(DDB3);
	for(;;) {
		PORTB |= _BV(PORTB3);
		_delay_ms(250);
		PORTB &= ~_BV(PORTB3);
		_delay_ms(250);
	}
}; 
