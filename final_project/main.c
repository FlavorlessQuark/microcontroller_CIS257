
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
// #include <stdbool.h>

#define MAX_TICKS_BEFORE_TRANSMIT 1000
#define H 0
#define V 1
#define HORIZONTAL_DIST (PINF & 0b1000000) >> 6
#define HORIZONTAL_DIR (PINF & 0b100000) >> 5
#define VERTICAL_DIST (PINF & 0b10000000) >> 7
#define VERTICAL_DIR (PINF & 0b10000) >> 4
#define SET_BIT(number, n, value) (number | (value << n ))
// A0 A3 UD (PF7.PF4)
// A1 A2 LR (PF6 PF5)

typedef struct packet {
    char _0;
    char _1;
    char _2;
    char parity;
}           Packet;

unsigned int ticks;
uint8_t pin_status[2] = {0};
char pin_accumulator[2] = {0};
char rm_stat = 0;

static int USART_Transmit(char data, FILE *_ );
static FILE serial_stream = FDEV_SETUP_STREAM(USART_Transmit, NULL, _FDEV_SETUP_WRITE);


void USART_Init( unsigned char baud )
{
    UBRR1 = (unsigned char)(baud << 4) | (unsigned char)baud;
    /* Enable transmitter */
    UCSR1B = (1<<TXEN1);
    /* Set frame format: 8data, 2stop bit */
    UCSR1C = (1<<USBS1)|(3<<UCSZ10);
}


static int USART_Transmit(char data, FILE *_ )
{
        /* Wait for empty transmit buffer */
        while ( !( UCSR1A & (1<<UDRE1)) )
        ;
        /* Put data into buffer, sends the data */
        UDR1 = data ;

        return 1;
}


void get_mouse_vector() {
    ticks++;

        if (!pin_status[H] && HORIZONTAL_DIST){
            if (HORIZONTAL_DIR)
                pin_accumulator[H] += 1;
            else
                pin_accumulator[H] -= 1;
        }

        pin_status[H] = HORIZONTAL_DIST;

        if (!pin_status[V] && VERTICAL_DIST){
            if (VERTICAL_DIR)
                pin_accumulator[V] -= 1;
            else
                pin_accumulator[V] += 1;
        }
        rm_stat = PINB & 0b1;
        pin_status[V] = VERTICAL_DIST;
}

static char clock;
static char parity;
static char bit;
void send_bit(char _bit) {
    clock ^= 0b10;
    PORTD = _bit | clock;
    _delay_us(30);

}
void send_byte(char byte) {
    char pinval = 0;
    clock = 0b00;

    bit = 0;
    send_bit(bit);
    send_bit(bit);

    for (char i = 0; i < __CHAR_BIT__; ++i) {
        pinval = 0;
        if (byte & (1U << i)) {
            pinval = 1;
            parity ^= 1;
        }
        else {
            pinval = 0;
        }
        bit = pinval;
        send_bit(bit);
        send_bit(bit);
    }
}



void send_packets() {
    Packet packet;
    parity = 0;
    clock = 0;
    /*
        This changes a negative number into a positive number and a positive number to itself
        This is because the expected data is a bit for direction ( + or - ) and a byte for magnitude

        2's complement : inverse number then add 1

        pin ^ (pin >> char_bit) Shift one whole byte - right shift duplicates msb-
        if msb is 1 (negative number) this results in 11111111
        if msb is 0 (positibe number) this results in 00000000

        pin ^ 1 is it;s inverse
        pin ^ 0 is itself

        + 1 * !!(pin & 0b10000000)

        pin & 0b100000  => 0b100000 when - (msb always set in 2s complement when negative) else 0

        !! => ! changes 0 to 1 and anything else to 0, !! changes 0 to 0 and non-zero to 1

        if number is negative this is + 1 * (1)
        if number is positive this is + 1 * (0)
     */
    packet._1 = (pin_accumulator[H] ^ (pin_accumulator[H] >> (__CHAR_BIT__ - 1))) + !!(1 * (pin_accumulator[H] & 0b10000000)); //inverse two's complement
    packet._2 = (pin_accumulator[V] ^ (pin_accumulator[V] >> (__CHAR_BIT__ - 1))) + !!(1 * (pin_accumulator[V] & 0b10000000)); //inverse two's complement

    /*
        Here we just want to set the X direction and Y direction bits (number always starts at 0)
        SET_BIT simply uses an OR operation (we don't care about setting a bit to 0 since it's initial value is 0)
        SET_BIT(intial_value, bit to set, value to set as)
        !!(pin & 0b1000000) sets the value to be the sign (msb) of pin (!! operator explained in previous comment)
     */
    packet._0 = SET_BIT(
            (uint8_t)SET_BIT((uint8_t)0b00001000, (uint8_t)5, !!((uint8_t)(pin_accumulator[V] & 0b10000000))),
            (uint8_t)4,
            !!((uint8_t)(pin_accumulator[H] & 0b10000000)));

    packet._0 |= (rm_stat);
    printf("%c%c%c", packet._0, packet._1, packet._2);// This for serial
        send_bit(0); //LOW start bit
        send_byte(packet._0);
        send_byte(packet._1);
        send_byte(packet._2);
        send_bit(parity);
        send_bit(1); //HIGH STOP BIT
    pin_accumulator[H] = 0;
    pin_accumulator[V] = 0;
}




void send_packet_byte(char byte) {

    bit = 1;
    clock = 0;
    send_bit(bit);
    _delay_us(200);

    send_byte(byte);

    send_bit(parity);
    bit = 1;
    clock = 0;
    send_bit(bit);
}

int main() {
    DDRF = 0b00000011;
    DDRD = 0b11111111;
    DDRB = 0b11111111;
    // CLKPR = 0b10000011;
    PORTF = 0b00000000;
    PORTD = 0b00000000;
    PORTB = 0b00000000;

    pin_accumulator[H] = 0;
    pin_accumulator[V] = 0;

    USART_Init(6);
    stdout = &serial_stream;
    int i = 0;
    while(1) {

        ticks++;

        get_mouse_vector();
        if (rm_stat || (ticks >= 10 && (pin_accumulator[H] != 0 || pin_accumulator[V] != 0))) { //delay sending packets to collect data
            send_packets();
            ticks = 0;
        }
    }
}
