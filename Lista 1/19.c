#include <stdio.h>

void print_binary(int n){
    for(int i=7; i>=0; i--){
        printf("%d", (n>>i)&1);
    }
    printf("\n");
}

int main(){

    int values[] = {0x0, 0x10, 0xf, 0x1f, 0xA4, 0xFF};

    for(int i=0; i<6; i++){
        printf("hex: %x  Bin: ",values[i]);
        print_binary(values[i]);
    }

}