#include <stdio.h>

void print_complement1(int value) {
    printf("%X\n", (unsigned char)(~value));
}

int main() {
    print_complement1(0xC6);
    print_complement1(0x35);
    print_complement1(0xD3);
    print_complement1(0xC7);
    return 0;
}
