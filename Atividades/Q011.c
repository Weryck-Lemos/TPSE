#include <stdio.h>

void print_binary(unsigned int value){
    for(int i=0; i<=31; i++){
        printf("%d", (value>>i)&1);
    }
    printf("\n");
}

int main(){
    int n;
    scanf("%d", &n);

    print_binary(n);
}