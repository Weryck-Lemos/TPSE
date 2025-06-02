#include <stdio.h>
#define IS_DIGIT(x) ((x>=0 && x<=9) ? "TRUE" : "FALSE") 

int main(){
    int x;
    scanf("%d", &x);

    printf("%s\n", IS_DIGIT(x));
}