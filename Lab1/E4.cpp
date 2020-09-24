#include<bits/stdc++.h>
using namespace std;

int min(int a, int b){
    return a < b ? a : b;
}

int main(){
    for (int size = 100; size < 800; size += 100){
        cout<<"Size "<<size<<":"<<"\n";
        int A[size][size], B[size][size], C[size][size], CB[size][size];
        for (int i = 0;i < size;i++){
            for (int  j= 0; j < size; j++){
                A[i][j] = rand() % 10000 + 1;
                B[i][j] = rand() % 10000 + 1;
                C[i][j] = 0;
                CB[i][j] = 0;
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
        cout<<"Clásica: "<<time/1000.0<<"\n";


        auto t2 = chrono::system_clock::now();
        int s = size / 25;
        int temp = 0;
        for(int jj=0;jj<size;jj+= s){
            for(int kk=0;kk<size;kk+= s){
                for(int i=0;i<size;i++){
                    for(int j = jj; j<((jj+s)>size?size:(jj+s)); j++){
                        temp = 0;
                        for(int k = kk; k<((kk+s)>size?size:(kk+s)); k++){
                            temp += A[i][k]*B[k][j];
                        }
                        CB[i][j] += temp;
                    }
                }
            }
        }
        auto t3 = chrono::system_clock::now();
        auto time1 = chrono::duration_cast<chrono::milliseconds>(t3-t2).count();
        cout<<"Bloques: "<<time1/1000.0<<"\n";
    }
    return 0;
}
