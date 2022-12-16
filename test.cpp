#include <bits/stdc++.h>
#include "matrix.hpp"
using namespace std;
template<typename _Tp>
void Display(matrix<_Tp> m) {
    for(int i = 0; i < m.getRows(); ++i, printf("\n"))
        for(int j = 0; j < m.getColumns(); ++j)
            cout << m[i][j] << ' ';
    cout << endl;
}
int main() {
    int N = 3, M = 4;
    matrix<int> Matrix(N, M);
    int cnt = 0;
    for(auto& i : Matrix) {
        i = ++cnt;
    }
    matrix<int> Matrix2 = Matrix;
    Matrix2.adjust_ROI(1, -1, 1, -1);
    Display(Matrix);
    Display(Matrix2);
    matrix<int> Matrix3 = Matrix2.clone();
    Display(Matrix3);
    Display(Matrix);
    Display(Matrix2);
    Display(Matrix3);
    matrix<matrix_pixel<int, 3> > m(3, 4);
    for(auto &i : m) {
        i = 3;
    }
    Matrix.setZero();
    Display(Matrix);
    Display(Matrix2);
    Display(m);
}