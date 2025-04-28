#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_COUNT 7

int count = 0x00;
int led_position = 0;
int toggle;

ISR(TIMER0_OVF_vect) {
    TCNT0 = 0;
    count++;

    if (count == 200) {
        count = 0;

        // Walk left if toggled else right
        if (toggle) {
            PORTC = ~(0b1000000 >> led_position);
        }
        else
            PORTC = ~(1 << led_position);
        if (led_position++ == LED_COUNT)// Toggle direction when end is reached
        {
            led_position = 0;
            toggle ^= 1;
        }
    }
}

void initialize() {
    DDRC = 0b11111111;
    TCCR0 = 0b010;
    PORTC = 0x00;
    TIMSK = 0b1;
}
int main(void) {
    initialize();
    sei();
   while (1) {

   }
}
