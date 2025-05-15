#include <stdio.h>

unsigned int rotateBits( unsigned int num, int n, int ini, int fim, int d){
    int numBits = fim - ini+1;

    unsigned int mask = ((1 << numBits -1));
    unsigned int extract = (num>>ini) & mask;

    extract >>=ini;

    if(d){
        extract = (extract << n) | (extract >> ( numBits -n)); 
    }

    else{
        extract = (extract >> n) | (extract <<(numBits - n));
    }

    num &= ~(mask << ini);
    num |= (extract<< ini);

    return num;
}

int main(){

    int num;

}