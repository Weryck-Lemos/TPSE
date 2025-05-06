#include <stdio.h>
#define CLEAR_BIT(x, b) (x &= ~(1 << b))

void print_binary(int n){
    for(int i=31; i>=0; i--){
        printf("%d", (n>>i)&1);
    }
    printf("\n");
}

int main(){

    int ctrl, b;
    scanf("%d %d", &ctrl, &b);
    print_binary(CLEAR_BIT(ctrl,b));
}