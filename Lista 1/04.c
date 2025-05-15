#include <stdio.h>
#define mirror(x) (~x)
int mirror_bits(int n){
    return ~n;
}

void print_bit(int n){
    for(int i=7; i>=0; i--){
        printf("%d", (n>>i)&1);
    }
    printf("\n");
}

int main(){
    char status = 0b10001111;

    print_bit(mirror_bits(status));
}