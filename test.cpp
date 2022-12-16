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
    // cout << "cmp " << A << ' ' << B << ' ' << (abs(A - B) <= 1) << endl;
    return abs(A - B) <= 1;
}
int main() {
    int N = 3, M = 4, K = 2;
    // cout << "create1" << endl;
    matrix<int> Mat1(N, M);
    matrix<int> Mat2(N, M);
    // cout << "finish" << endl;
    int cnt = 0;
    for(auto& i : Mat1) i = ++cnt;
    cnt = 0;
    for(auto& i : Mat2) i = ++cnt;
    Mat2.for_each([](int &A){++A;});
    matrix<int> Mat3 = Mat2 + Mat1.clone().set_zero();
    matrix<double> Mat4(N, M);
    Mat4.set_zero();
    Mat3 += Mat4;
    cout << Mat3.equal(Mat2) << endl;
    cout << Mat1.equal(Mat2) << endl;
    cout << Mat1.equal(Mat2, [](int A, int B){return abs(A - B) <= 1;}) << endl;
    cout << Mat1.equal(Mat2, cmp) << endl;
}