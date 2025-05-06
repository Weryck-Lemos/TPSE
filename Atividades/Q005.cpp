#include <iostream>
using namespace std;

unsigned int setBits(bool w, unsigned int i, unsigned int j, unsigned int value){
    unsigned int mask = ((1 <<(j-i+1)) -1 )<< i;

    if(w) value |= mask;
    else value &= ~mask;

    return value;

}

void print_binary(unsigned int n){
    for(int i=31; i>=0; i--){
        cout<< ((n>>i)&1);
    }

    cout<<"\n";
}

int main(){

    unsigned int i, j, value;
    bool w;
    cin>>w>>i>>j>>value;

    print_binary(setBits(w, i, j, value));
}