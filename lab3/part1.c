#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int count = 0;

int main(void) {
    DDRC = 0b11111111;//set all LEDS off
    TCCR0 = 0b010;// Use prescaler clock / 8
    TCNT0 = 0;//reset timer
    PORTC = 0;
    while(1)
    {
        if (TCNT0 == 250)
        {
            TCNT0 = 0;
            count++;
            if (count == 250) {
                PORTC ^= 0xFF;// inver PORTC bits
                count = 0;
            }
        }
    }
}
