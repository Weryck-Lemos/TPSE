#include <stdio.h>
#define SQR(x) ((x) * (x))

int main(){
    int counter = 0;

    while(counter < 5){
        printf("x %d square %d\n", counter, SQR(++counter));
    }


    //a variável counter aumenta em 2 a cada repeticao
    //pois na macro ela é chamada duas vezes
}