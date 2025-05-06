#include <stdio.h>
#define SET_BIT(REG, BIT) (REG |= (1 << BIT))

void print_binary(int n){
    for(int i=31; i>=0; i--){
        printf("%d", (n>>i)&1);
    }
    printf("\n");
}

int main(){

    int ctrl;
    scanf("%d", &ctrl);
    print_binary(SET_BIT(ctrl, 5));
}