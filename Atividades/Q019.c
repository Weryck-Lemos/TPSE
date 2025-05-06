#include <stdio.h>

#define IS_NEGATIVE(x) (x<0)
int main(){
    int n;
    scanf("%d", &n);

    if(IS_NEGATIVE(n))printf("Verdadeiro\n");
    else printf("Falso\n");

}