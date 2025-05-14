#include "includes/decoder.h"

void print_bits(char c) {
    for (int i = 0b10000000; i > 0; i >>= 1) {
        printf("%d", !!(c & i));
    }
    printf("  ");
}

void print_data(char buffer[3]) {
    Packet packet = {
        .data.raw = buffer[0],
        .horizontal = buffer[1],
        .vertical = buffer[2]
        };

    printf("\nSign Y %d sign X %d | X %d , Y %d\n",
        packet.data.bits.y_sign,
        packet.data.bits.x_sign,
        packet.horizontal,
        packet.vertical
    );
}
