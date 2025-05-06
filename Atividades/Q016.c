#include <stdio.h>

void set_bit_if_zero(unsigned int *reg, unsigned int bit){
    if((*reg & (1<<bit)) == 0){
        *reg |=(1<<bit);
    }
}

void print_bits(unsigned int n){
    for(int i=31; i>=0; i--){
        printf("%d", (n>>i)&1);
    }
}

int main(){
    unsigned int reg = 0b11010100;
    int n;
    scanf("%d", &n);
    set_bit_if_zero(&reg, n);
    print_bits(reg);
}