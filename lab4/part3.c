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

void initialize() {
    UCSRA = 0b010;// Double speed
    UCSRB = 0b1000;// Sets TXEN bit to transmit
    UCSRC = 0b10000110; // Run async + 8bit char size
    UBRRL = 12; // BAUD RATE 9600

    DDRC =  ~(0x0)
}
int main(void) {
    initialize();

    stdout = &serial_stream;

    int count = 0;

    while(1)
    {
        _delay_ms(1000);
        printf("PORTA VALUE IS %d\n", PINA);
    }
}
