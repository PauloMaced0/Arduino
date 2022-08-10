default:
		avr-gcc -DF_CPU=16000000UL -mmcu=atmega328p -c -o spaceship.o spaceship.c 
		avr-gcc -o spaceship.bin spaceship.o 
		avr-objcopy -O ihex -R .eeprom spaceship.bin spaceship.hex 
		sudo avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:spaceship.hex