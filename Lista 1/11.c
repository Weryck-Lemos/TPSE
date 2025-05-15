#include <stdio.h>
#define SWAP(a,b) (a^=b, b^=a, a^=b)

int main(){

    int a=10, b=45;
    SWAP(a,b);
    printf("%d %d", a, b);
}