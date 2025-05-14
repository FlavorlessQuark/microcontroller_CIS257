#ifndef _DECODER_H
# define _DECODER_H

# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>
# include <termios.h>

# define SERIALSOURCE "/dev/ttyUSB0"
# define _POSIX_SOURCE 1
# define CHAR_COUNT 3
# define BAUDRATE B9600
# define CRTSCTS 020000000000


typedef struct Bits {
    unsigned char button_middle : 1;//LSB
    unsigned char button_right : 1;
    unsigned char button_left : 1;
    unsigned char _ : 1;
    unsigned char x_sign : 1;
    unsigned char y_sign : 1;
    unsigned char x_overflow : 1;
    unsigned char y_overflow : 1;//MSB
} Bits;

typedef union Data {
    char raw;
    Bits bits;
} Data;

typedef struct Packet {
    Data data;
    char horizontal;
    char vertical;
}    Packet;

void print_bits(char c);
void print_data(char buffer[3]);

#endif
