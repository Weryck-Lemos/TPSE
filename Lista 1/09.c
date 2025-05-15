#include <stdio.h>
#define IS_DIGIT(x) ((x>=0 && x<=9) ? 1 : 0) 

int main(){
    int x;
    scanf("%d", &x);

    printf("%d\n", IS_DIGIT(x));
}