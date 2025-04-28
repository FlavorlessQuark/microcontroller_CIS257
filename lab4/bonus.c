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
void print_binary(char binary) {
    int i = 1;

    uart_putchar('\n', stdout);
    uart_putchar('0', stdout);
    uart_putchar('b', stdout);
    while (i < 0xFF) { // i is the bitmask

        //(binary & i) is 0 if nothing matches else >= 1
        // ! sets 0 to 1 and everything else to 0
        // ! sets 0 t0 1 and 1 to 0
        // + '0' adds 48 to the number and puts it in the correct ASCII range for numbers
        uart_putchar(!!(binary & i) + '0', stdout);
        i <<= 1;//shift the bitmask
    }
}

void initialize() {
    UCSRA = 0b010;// Double speed
    UCSRB = 0b1000;// Sets TXEN bit to transmit
    UCSRC = 0b10000110; // Run async + 8bit char size
    UBRRL = 12; // BAUD RATE 9600
    DDRA = 0x0;

}
int main(void) {
    initialize();

    stdout = &serial_stream;

    int count = 0;

    while(1)
    {
        _delay_ms(250);
        print_binary(PINA);
    }
}
