#include <stdio.h>

#define MASK_BITS(x,m) ((x ) & (m))

void print_binary(unsigned int n){
    for(int i=7; i>=0; i--){
        printf("%d", (n>>i) &1);
    }
    printf("\n");
}

int main(){
    unsigned int valor = 0b11000011;
    unsigned int mask =  0b11111001;
    
    print_binary(MASK_BITS(valor, mask));
}