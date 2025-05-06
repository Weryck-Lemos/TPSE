#include <stdio.h>

unsigned char inverter(unsigned char x){
    return ~x;
}

void print_bits(unsigned char n){
    for(int i=7; i>=0; i--){
        printf("%d", (n>>i)&1);
    }
}

int main(){
    unsigned char value = 0b11101000;

    print_bits(inverter(value));

}