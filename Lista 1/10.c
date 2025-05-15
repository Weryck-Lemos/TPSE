#include <stdio.h>
#define is_digit(c) ((c)>='0' && (c)<='9' ? 1 : 0)

#define is_hex(c) ((is_digit(c) || ((c)>='A' && (c)<='F') || ((c)>='a' && (c)<='f'))  ? 1 : 0)

int main(){
    char test;
    scanf("%c", &test);
    
    printf("%d\n", is_hex(test));
}