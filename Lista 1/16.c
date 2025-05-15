#include <stdio.h>
#define RECIPROCAL(number) (1.0/ (number))

int main(){
    float counter;
    
    for(counter = 1.0; counter < 10.0; counter += 1.0){
        printf("1/%f = %f\n", counter, RECIPROCAL(counter));
    }
}

//tinha um espaço entre RECIPROCAL e (number)
//ele considera a macro sem parâmetro