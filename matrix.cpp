#include "matrix.hpp"
#include <cstring> // memcpy
#include <exception> // exception
#include <iostream> // cerr, endl

#ifdef __MATRIX__

template<typename _Tp>
__matrix_data<_Tp>::__matrix_data(): data(NULL), ref(0), length(0) {}

template<typename _Tp>
__matrix_data<_Tp>::__matrix_data(size_t len): __matrix_data(), length(len) {
    try {
        data = new _Tp[length];
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
        data = NULL;
    }
}

template<typename _Tp>
__matrix_data<_Tp>::~__matrix_data() {
    try {
        if(data != NULL) delete[] data;
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

template<typename _Tp>
inline void __matrix_data<_Tp>::add() {
    ++ref;
}

template<typename _Tp>
inline void __matrix_data<_Tp>::release() {
    if(ref) {
        --ref;
        if(ref == 0 && data != NULL) {
            try {
                delete[] data;
            } catch(std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }
    } else std::cerr << "Warning from __matrix_data<_Tp>::release()." << std::endl; // Otherwise, an error occured.
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
matrix<_Tp>::matrix(const matrix<_Tp> &t): __data(NULL), __begin(NULL), rows(0), columns(0), gap(0) {}

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
matrix<_Tp>& matrix<_Tp>::clone() const {
    matrix t = new matrix();
    t.rows = rows;
    t.gap = t.columns = columns;
    t.__data = new __matrix_data<_Tp>(rows * columns);
    _Tp *ptr = t.__begin = t.__data->get(), *sptr = __begin;
    /**
     * @note You cannot make a deep copy of __matrix_data here, because
     * this matrix can be just a ROI of some larger matrix.
    */
    for(int i = 0; i < t.rows; ++i) {
        try {
            memcpy(ptr, sptr, sizeof(_Tp) * t.columns);
        } catch(std::exception &e) {
            for(int j = 0; j < t.columns; ++j)
                *(ptr + j) = *(sptr + j);
        }
        ptr += t.columns;
        sptr += gap;
    }
    return t;
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