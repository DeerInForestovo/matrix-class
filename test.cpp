#include <bits/stdc++.h>
#include "matrix.hpp"
using namespace std;
template<typename _Tp>
void Display(const matrix<_Tp> &m) {
    for(int i = 0; i < m.getRows(); ++i, printf("\n"))
        for(int j = 0; j < m.getColumns(); ++j)
            cout << m[i][j] << ' ';
    cout << endl;
}
typedef matrix_pixel<int, 3> pix;
bool cmp(int &A, int &B) {
    cout << "cmp " << A << ' ' << B << ' ' << (abs(A - B) <= 1) << endl;
    return abs(A - B) <= 1;
}
int main() {
    int N = 3, M = 4, K = 2;
    cout << "create1" << endl;
    matrix<int> Mat1(N, M), Mat2(M, K), Mat3;
    cout << "finish" << endl;
    int cnt = 0;
    for(auto& i : Mat1) i = ++cnt;
    Display(Mat1);
    Mat1.for_each([](int &a){++a;});
    Display(Mat1);
    
}