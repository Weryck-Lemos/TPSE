#include <stdio.h>

typedef struct{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;

void print_rgb_hex(RGB color){
    printf("%X %X %X\n", color.r, color.g, color.b);
}

int main(){
    RGB cor = {255, 165, 0};
    print_rgb_hex(cor);

}