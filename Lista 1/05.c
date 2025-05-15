#include <stdio.h>

void print_bit(int n){
    for(int i=7; i>=0; i--){
        printf("%d", (n>>i)&1);
    }
    printf("\n");
}


int count_bit( int n, int m){

    unsigned int x = ~(n^m);
    int ans=0;
    for(int i=7; i>=0; i--){
        if(x>>i & 1)ans++;
    }
    return ans;
}


int main(){
    int a = 0b11111111;
    int b = 0b11010001;

    printf("%d\n",count_bit(a,b));

}