avr-gcc -DF_CPU=32000000UL -mmcu=atmega32u4 -std=gnu99 main.c -o main.elf
avr-objcopy -O ihex -j .text -j .data main.elf main.hex
sudo avrdude -p atmega32u4 -P usb  -B 125kHz -U flash:w:main.hex
