#include <stdio.h>
#define SIZE 10
#define FUDGE SIZE-2

int main(){
    int size;

    size= FUDGE;
    printf("size is %d\n", size);

    //existia ; nas macros e isso estava causando erros
}