#include <stdio.h>

unsigned int circular_right(unsigned int x, int n){
    const int bits = 32;

    return (x>>n) | (x<<(bits-n));
}

void print_binary(unsigned int value){
    for(int i=31; i>=0; i--){
        printf("%d", (value>>i) &1);
    }
    printf("\n");
}

int main(){

    unsigned int a= 0b11111;

    print_binary(circular_right(a,5));
}