#include <stdio.h>
#define div10(x) !(x%10)

int main(){
    int n;
    scanf("%d", &n);

    if(div10(n))printf("divisivel\n");
    else printf("nao e divisivel\n");
}