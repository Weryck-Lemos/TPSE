#include <stdio.h>
#include "mirror.h"

void print_bit(int n){
    for(int i=7; i>=0; i--){
        printf("%d", (n>>i)&1);
    }
    printf("\n");
}

int main(){
    char status = 0b11100110;
    print_bit(mirror_bits(status));
    return 0;
}
