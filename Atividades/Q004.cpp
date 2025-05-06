#include <iostream>
using namespace std;

int countBits(bool w, unsigned int value){
    int count =0;

    while(value){
        if(w){

            if(value & 1) count++;
        }

        else{
            if((value &1) == 0 )count++;
        }
        value >>=1;
    }

    return count;
}

int main(){
    int v;
    bool w;

    cin>>v>>w;
    cout<<countBits(w, v)<<"\n";
}