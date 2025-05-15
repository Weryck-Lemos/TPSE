#include <stdio.h>

#define CLEAR_BIT(x,b) (x |= (1<<b))

void print_bit(int n){
    for(int i=7; i>=0; i--){
        printf("%d",(n>>i & 1));
    }
    printf("\n");
}

int main(){
    int n;
    scanf("%d", &n);

    CLEAR_BIT(n,5);
    print_bit(n);
}