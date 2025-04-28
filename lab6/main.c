#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>

#define F_CPU 1000000
#define UDRE_BIT 0b010000

volatile unsigned char adc_data;


void initialize() {
    DDRC = 0xFF;
    ADMUX = 0b00100000;
    ADCSRA = 0b11001100;
    PORTC=0xFF;
    DDRD=0b01000000;
    TCCR0B = 0b01101000;// Bits for phase correct PWM + clear when up count , set when down count
    // UCSRA = 0b010;
    // UCSRB = 0b1000;// Sets TXEN bit to transmit
    // UCSRC = 0b10000110; // Run async + 8bit char size
    // UBRRL = 12;
    // DDRC = 0x0;


}

ISR(ADC_vect) {
    adc_data = OCDR2;
}

int main(void) {
    initialize();
    sei();
    adc_data = 0xF0;
    while (1) {
        PORTC = (0x0 & (0b10000000 >>(char)ceil(adc_data / 8)));

    }
}

