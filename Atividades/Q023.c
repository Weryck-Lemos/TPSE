#include <stdio.h>

int compare_bits(unsigned int a, unsigned int b){
    unsigned int xor_result = ~(a^b);

    int count = 0;
    for(int i=0; i<8; i++){
        if((xor_result>>i) &1){
            count++;
        }
    }

    return count;
}

int main(){
    unsigned int a = 0b11110000;
    unsigned int b = 0b00001000;

    
    printf("%d\n", compare_bits(a,b));
}