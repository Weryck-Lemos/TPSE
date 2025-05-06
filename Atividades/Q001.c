#include <stdio.h>
#define is_upper(c) (c>= 'A' && c<='Z')

int main(){
    char n;
    scanf("%c", &n);

    if(is_upper(n)) printf("maiúscula\n");
    else printf("minúscula\n");
}