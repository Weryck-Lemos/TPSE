#include <stdio.h>

int CountBit(char byte){
    int count =0;
    while (byte){

        if(byte & 1)count++;
        byte >>=1;
    }
    return count;
}

int main(){
    char n;
    scanf("%c", &n);
    printf("%i", CountBit(n));
}