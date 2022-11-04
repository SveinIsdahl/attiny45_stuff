# attiny45_stuff

### Compile
avr-gcc -Os -DF_CPU=1000000L -o blink blink.c -mmcu=attiny45

avr-objcopy -R .eeprom -O ihex blink blink.hex

sudo avrdude -F -v -c stk500v1 -p attiny45 -U flash:w:blink.hex -P /dev/ttyACM0 -b 19200 
