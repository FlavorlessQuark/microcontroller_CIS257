avr-gcc -DF_CPU=16000000UL -mmcu=atmega32u4 -std=gnu99 main.c -o main.elf -O1
avr-objcopy -O ihex -j .text -j .data main.elf main.hex
# sudo avrdude -p atmega32u4 -c avr109 -P usb  -B 125kHz -U flash:w:main.hex
 sudo avrdude -p atmega32u4 -cavr109 -P/dev/ttyACM0 -b57600 -D -U flash:w:main.hex -v
