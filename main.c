#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 1000000
#define UDRE_BIT 0b010000

static int uart_putchar(char c, FILE *stream);

static FILE serial_stream = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);


static int uart_putchar(char c, FILE *stream) {
    while ((UCSRA & (1 << 5)) == 0x00) {
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
    while (i < 0xFF) {
        uart_putchar(!!(binary & i) + '0', stdout);
        i <<= 1;
    }
}

void initialize() {
    UCSRA = 0b010;
    UCSRB = 0b1000;// Sets TXEN bit to transmit
    UCSRC = 0b10000110; // Run async + 8bit char size
    UBRRL = 12;
    DDRA = 0x0;

    // TCCR0 = 0b010;
    // UCSRA = 0x02; // double speed mode
    // UCSRB = 0x08; // enable transmitter
    // UCSRC = 0x86; // 8 data bits, 1 stop, Async
    // UBRRL = 12;
}
int main(void) {
    initialize();

    stdout = &serial_stream;

    int count = 0;

    while(1)
    {
        _delay_ms(250);
        printf("\nCarriage returns are bad %d\n", PINA);
        print_binary(PINA);
    }
}
