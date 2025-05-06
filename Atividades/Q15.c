#include <stdio.h>

unsigned int get_masked_bits(unsigned int value, unsigned int mask){
    return value & mask;
}

void print_bits(unsigned char n){
    for(int i=31; i>=0; i--){
        printf("%d", (n>>i)&1);
    }
    printf("\n");
}

int main(){

    unsigned int value = 0b110101101;
    unsigned int mask = 0b111000100;
    unsigned int result = get_masked_bits(value, mask);
    
    print_bits(result);
}