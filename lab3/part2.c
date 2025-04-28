#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_COUNT 7

int count = 0x00;
int led_position = 0;

ISR(TIMER0_OVF_vect) {
    TCNT0 = 0;
    count++;

    if (count == 200) {
        PORTC = ~ (1 << led_position); // LED ON at led_position
        count = 0;
        led_position = (led_position + 1) % LED_COUNT; // when led position +reaches LED_COUNT, set it to 0
    }
}

void initialize() {
    DDRC = 0b11111111;//set led off
    TCCR0 = 0b010;// prescaler clock / 8;
    PORTC = 0x00;
    TIMSK = 0b1;//set timer interrupt on overflow
}
int main(void) {
    initialize();
    sei();
   while (1) {

   }
}
