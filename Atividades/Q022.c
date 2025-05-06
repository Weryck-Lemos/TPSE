#include <stdio.h>

void print_bit(unsigned char x){
    for(int i=7; i>=0; i--){
        printf("%d", (x>>i)&1);
    }
    printf("\n");
}

void clear_even_bits(unsigned char *x){
    unsigned char mask = 0x55; //01010101

    *x &= ~mask;
}

int main(){

    unsigned char value = 0b1111111;
    clear_even_bits(&value);
    print_bit(value);
}