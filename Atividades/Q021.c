#include <stdio.h>

int bit_parity(unsigned int x){
    int count =0;
    while(x){
        if(x&1)count++;

        x >>=1;
    }

    if(count%2==0) return 0;
    else return 1;
}

int main(){
    unsigned int n = 0b0011111;

    if(bit_parity(n))printf("impar\n");
    else printf("par\n");
}