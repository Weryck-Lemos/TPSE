#include <stdio.h>

void bin(int n){
    for(int i=7; i>=0; i--){
        printf("%d",(n>>i)&1);
    }
    printf("\n");

}

int active(int n){
    return n |= 1<<2;
}

int count1(int n){
    int ans=0;
    for(int i=7; i>=0; i--){
        if(n>>i & 1)ans++;
    }
    return ans;
}

int countw(int n, int w){
    int ans=0;
    
    for(int i=7; i>=0; i--){
        if(w){
            if(n>>i & 1)ans++;
        }

        else{
            if(!(n>>i & 1))ans++;
        }
    }

    return ans;
}

int mirror(unsigned int n){
    unsigned int ans=0;

    for(int i=0; i<8; i++){
        ans<<=1;
        ans |= (n&1);
        n>>=1;
    }

    return ans;

}

int main(){
    int n;
    scanf("%d", &n);
    bin(mirror(n));

}