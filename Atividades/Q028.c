#include <stdio.h>

unsigned int merge_fields(unsigned int a, unsigned int b, unsigned int mask){
    return (a& ~mask) | (b& mask);
}

void print_binary(unsigned int value){
    for(int i=31; i>=0; i--){
        printf("%d", (value>>i) &1);
    }
    printf("\n");
}

int main(){
    unsigned int a = 0b11001100;
    unsigned int b =0b10101010;
    unsigned int mask = 0b00001111;

    print_binary(merge_fields(a, b, mask));
}