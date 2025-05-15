#include <stdio.h>
#include <stdlib.h>
#define DIE {fprintf(stderr, "Fatal Error: Abourt\n"); exit(8); }

int main(){
    int value;
    scanf("%d", &value);

    if(value < 0)DIE;

    printf("We did not die\n");
}

//alterei a linha 13: retirei o \ e coloquei o bloco de comando dentro de chaves