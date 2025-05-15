#include <stdio.h>

void print_bitwise(int a, int b){
    printf("%X\n", a&b);
    printf("%X\n", a|b);
    printf("%X\n", a^b);
    
}

int main(){
    print_bitwise(0xC6, 0x35);
    print_bitwise(0x19, 0x24); 
    print_bitwise(0xD3, 0xC7);
    print_bitwise(0x17, 0xFF);
}