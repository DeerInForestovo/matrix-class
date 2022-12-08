#include "matrix.hpp"
#ifdef __MATRIX__

template<typename _Tp>
__matrix_data<_Tp>::__matrix_data(): data(NULL), ref(0) {}

template<typename _Tp>
__matrix_data<_Tp>::__matrix_data(size_t length): __matrix_data() {
    data = new _Tp[length];
}

template<typename _Tp>
__matrix_data<_Tp>::~__matrix_data() {
    if(data != NULL) delete[] data;
}

template<typename _Tp>
inline void __matrix_data<_Tp>::add() {
    ++ref;
}

template<typename _Tp>
inline void __matrix_data<_Tp>::release() {
    if(ref) {
        --ref;
        if(ref == 0 && data != NULL) delete[] data;
    } // Otherwise, an error occured.
}

template<typename _Tp>
inline _Tp* __matrix_data<_Tp>::get() const {
    return data;
}

template<typename _Tp>
inline void matrix<_Tp>::release() {
    if(__data != NULL) {
        __data->release();
        __data = NULL;
        __begin = NULL;
    }
}

template<typename _Tp>
matrix<_Tp>::matrix(): __data(NULL), __begin(NULL), rows(0), columns(0), gap(0) {}

template<typename _Tp>
matrix<_Tp>::matrix(size_t row, size_t column): matrix(), rows(row), columns(column), gap(column) {
    __data = __matrix_data<_Tp>(row * column);
    __data->add();
    __begin = __data->get();
}

template<typename _Tp>
matrix<_Tp>::~matrix() {
    release();
}

template<typename _Tp>
inline size_t matrix<_Tp>::getRows() const {
    return this->rows;
}

template<typename _Tp>
inline size_t matrix<_Tp>::getColumns() const {
    return this->columns;
}

template<typename _Tp>
void matrix<_Tp>::operator= (const matrix<_Tp> &t) {
    release();
    rows = t.rows;
    columns = t.columns;
    gap = t.gap;
    __data = t.__data;
    __data->add();
    __begin = t.__begin;
}

#endif /*__MATRIX__*/