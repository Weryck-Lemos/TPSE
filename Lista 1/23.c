#include <stdio.h>

int count_bits(int n){
    int ans=0;
    for(int i=15; i>=0; i--){
        if((n>>i)&1)ans++;

    }
    return ans;
}

int main(){
    int n;
    scanf("%d", &n);

    printf("%d\n", count_bits(n));
}