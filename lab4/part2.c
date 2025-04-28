#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 1000000
#define UDRE_BIT 0b010000

static int uart_putchar(char c, FILE *stream);

static FILE serial_stream = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);


static int uart_putchar(char c, FILE *stream) {
    while (!(UCSRA & UDRE_BIT)) { // While the UDRE_BIT (5th bit) is 0, wait
        continue;
    }
    UDR = c;

    return 1;
}

void initialize() {
    UCSRA = 0b010;// Double speed
    UCSRB = 0b1000;// Sets TXEN bit to transmit
    UCSRC = 0b10000110; // Run async + 8bit char size
    UBRRL = 12; // BAUD RATE 9600

}
int main(void) {
    initialize();
    stdout = &serial_stream;

    int count = 0;
    uart_putchar('A', stdout);

    uart_putchar('B', &serial_stream);

    uart_putchar('C', &serial_stream);


    while(1)
    {
                uart_putchar(0x0A, stdout);
                uart_putchar(0x0D, stdout);
                uart_putchar(0x43, stdout);
                uart_putchar(0x4F, stdout);
                uart_putchar(0x4D, stdout);
                uart_putchar(0x50, stdout);
                uart_putchar(0x20, stdout);
                uart_putchar(0x32, stdout);
                uart_putchar(0x35, stdout);
                uart_putchar(0x31, stdout);
                uart_putchar(0x20, stdout);
                uart_putchar(0x52, stdout);
                uart_putchar(0x4F, stdout);
                uart_putchar(0x43, stdout);
                uart_putchar(0x4B, stdout);
                uart_putchar(0x53, stdout);
                uart_putchar(0x21, stdout);
    }
}
