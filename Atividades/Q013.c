#include <stdio.h>
#define IS_EVEN(x) (x&1==0)

int main(){
    int n;
    scanf("%d", &n);

    if(IS_EVEN(n))printf("PAR\n");
    else printf("IMPAR\n");
}