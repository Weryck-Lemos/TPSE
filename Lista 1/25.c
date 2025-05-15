#include <stdio.h>

int empurra(int valor) {
    int count=0;

    for (int i=0; i<32;i++) {
        if ((valor>>i) & 1) count++;
    }

    int ans=0;
    for (int i =0; i<count; i++) {
        ans |= (1 << (31 - i));
    }

    return ans;
}

void print_bin(int n){
    for(int i=31; i>=0; i--){
        printf("%d", (n>>i)&1);
    }
    printf("\n");
}

int main() {
    int valor = 0x1011001;
    print_bin(empurra(valor));
}
