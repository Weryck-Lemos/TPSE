#include <stdio.h>

int count_consecutive_bits(unsigned int value, int bit_value){
    int count = 0, maxcount = 0;
    
    while(value){
        if((value & 1)== bit_value)count++;
        else count=0;

        value>>=1;
        if(count> maxcount)maxcount = count;
    }

    return maxcount;
}

int main(){

    unsigned int value = 0b1101000;

    int n;
    scanf("%d", &n);

    printf("%d\n", count_consecutive_bits(value, n));

}