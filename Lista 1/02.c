#include <stdio.h>

int swap_bits(int x, int i, int j){
    if( ((x>>i)&1) ^ ((x>>j)&1) ) x ^=(1<<i) | (1<<j);
    return x;
}

void print_bit(int n){
    for(int i=7; i>=0; i--){
        printf("%d",(n>>i & 1));
    }
    printf("\n");
}

int main(){
    int n= 0b00000001;

    print_bit(swap_bits(n,0,2));
}