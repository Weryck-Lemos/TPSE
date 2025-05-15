#include <stdio.h>
#define SQR(x) (x*x)

int main(){
    int counter;
    for(counter =0; counter < 5; counter++){
        printf("x %d squared %d\n", counter+1, SQR(counter+1));
    }

    //2+1 * 2+1 = 3
    //3+1 * 3+1 = 5
    //4+1 * 4+1 = 7
    //5+1 * 5+1 = 9
}