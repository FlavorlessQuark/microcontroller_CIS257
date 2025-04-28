#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 1000000
#define UDRE_BIT 1 << 5

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

    int count = 0;
    uart_putchar('B', &serial_stream);
    uart_putchar('C', &serial_stream);


    while(1)
    {
                uart_putchar('Z', &serial_stream);
                // uart_putchar('T', &serial_stream);
                // uart_putchar(' ', &serial_stream);
                // uart_putchar('W', &serial_stream);
                // uart_putchar('O', &serial_stream);
                // uart_putchar('R', &serial_stream);
                // uart_putchar('K', &serial_stream);
                // uart_putchar('S', &serial_stream);
                // uart_putchar('!', &serial_stream);
                // uart_putchar('\n',&serial_stream);
    }
}
