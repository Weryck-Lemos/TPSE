#include <stdio.h>

void print_bit(int n){
    for(int i=7; i>=0; i--){
        printf("%d", (n>>i)&1);
    }
    printf("\n");
}

int circular_right(unsigned int x, int n){
    int bites = 8;

    return x>>n | x<<(bites-n);

}

int main(){

    int n = 0b00000110;

    print_bit(circular_right(n, 5));
}