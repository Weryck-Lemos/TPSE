#include <stdio.h>


int main(){

    int reg;
    scanf("%d", &reg);
    reg |= (1<<2);

    printf("%i", reg);
}