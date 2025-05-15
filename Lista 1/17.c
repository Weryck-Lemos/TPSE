#include <stdio.h>
#define MAX 10

int main(){
    int counter;

    for(counter = MAX; counter>0; --counter){
        printf("Hi there\n");
    }

}
//tinha um sinal de = na macro, isso é um erro de sintaxe