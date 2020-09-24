#include <bits/stdc++.h>
using namespace std;
void inicializar(double **&A, double *&x, double *&y, int MAX){
    A = new double*[MAX];
    x = new double[MAX];
    y = new double[MAX];
    for(int i=0;i<MAX;++i){
        A[i]=new double[MAX];
    }
}
void bucle1(int MAX){
    double **A,*x,*y;
    inicializar(A,x,y,MAX);
    auto t0 = chrono::system_clock::now();
    for(int i=0;i<MAX;i++){
        for(int j=0;j<MAX;j++){
            y[i] += A[i][j]*x[j];
        }
    }
    auto t1 = chrono::system_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(t1-t0).count();
    cout<<"Bucle 1: "<<time/1000.0<<"\n";
}

void bucle2(int MAX){
    double **A,*x,*y;
    inicializar(A,x,y,MAX);
    auto t0 = chrono::system_clock::now();
    for(int j=0;j<MAX;j++){
        for(int i=0;i<MAX;i++){
            y[i] += A[i][j]*x[j];
        }
    }
    auto t1 = chrono::system_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(t1-t0).count();
    cout<<"Bucle 2: "<<time/1000.0<<"\n";
}
int main(){
    int MAX = 10000;
    for(int i=0;i<10;++i){
        cout<<"Prueba "<<i+1<<" con MAX = "<<MAX<<": "<<endl;
        bucle1(MAX);
        bucle2(MAX);
        MAX +=10000;
    }
    return 0;
}