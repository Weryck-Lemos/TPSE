#include <stdio.h>

void print_bits(unsigned char n){
    for(int i=7; i>=0; i--){
        printf("%d", (n>>i)&1);
    }
    printf("\n");
}

int main(){
    unsigned char status = 0b11101111;
    status &= ~(1<<4);
    printf("%d", status);
}