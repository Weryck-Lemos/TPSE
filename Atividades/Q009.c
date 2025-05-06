#include <stdio.h>

unsigned char mirror_bits(unsigned char value){
    unsigned char result = 0;

    for(int i=0; i<8; i++){
        result <<=1;
        result |= (value&1);
        value>>=1;
    }

    return result;
}

void print_binary(unsigned char n){
    for(int i=7; i>=0; i--){
        printf("%d", (n>>i) & 1);
    }
    printf("\n");
}

int main(){
    unsigned char value;
    scanf("%hhu", &value);

    print_binary(mirror_bits(value));
}