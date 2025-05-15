#include <stdio.h>

void print_bit(int n){
    for(int i=7; i>=0; i--){
        printf("%d",(n>>i & 1));
    }
    printf("\n");
}

int main(){
    char status = 0b11111111;

    status &= ~(1<<4);

    print_bit(status);
}