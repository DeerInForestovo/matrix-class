#include <bits/stdc++.h>
#include "matrix.hpp"
using namespace std;
template<typename _Tp>
void Display(matrix<_Tp> m) {
    for(auto i : m) cout << i << ' ';
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
    Matrix2.adjust_ROI(1, 1, -1, -1);

}