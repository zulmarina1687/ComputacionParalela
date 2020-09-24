#include<bits/stdc++.h>
using namespace std;

int main(){
    for (int size = 100; size < 900; size+= 100){
        cout<<"Size: "<<size<<",";
        int A[size][size], B[size][size], C[size][size];
        for (int i = 0;i < size;i++){
            for (int  j= 0; j < size; j++){
                A[i][j] = rand() % 1000 + 1;
                B[i][j] = rand() % 1000 + 1;
                C[i][j] = 0;
            }
        }
        auto t0 = chrono::system_clock::now();
        for(int i=0; i<size; ++i){
            for(int j=0; j<size; ++j){
                for(int z=0; z<size; ++z){
                    C[i][j] += A[i][z] * B[z][j];
                }
            }
        }     
        auto t1 = chrono::system_clock::now();
        auto time = chrono::duration_cast<chrono::milliseconds>(t1-t0).count();
        cout<<"Tiempo: "<<time/1000.0<<"\n";
    }
    return 0;
}

