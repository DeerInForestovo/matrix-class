#ifndef __MATRIX__
#define __MATRIX__

#include <stddef.h> // size_t
#include <exception> // exception
#include <iostream> // cerr

using namespace std;

template<typename _Tp>
class __matrix_data {
    private:
        /**
         * @brief The number of references to the data.
        */
        size_t ref;
        /**
         * @brief The rows of the matrix.
        */
        size_t rows;
        /**
         * @brief The columns of the matrix.
        */
        size_t columns;
        /**
         * @brief The pointer to the head of data.
        */
        _Tp* data;
    
    public:
        __matrix_data(): data(NULL), ref(0), rows(0), columns(0) {}
        /**
         * @brief Create a __matrix_data of length len.
        */
        __matrix_data(size_t row, size_t column): __matrix_data() {
            rows = row;
            columns = column;
            try {
                data = new _Tp[row * column];
                cout << "new data" << endl;
            } catch(std::exception &e) {
                std::cerr << e.what() << std::endl;
                data = NULL;
            }
        }
        ~__matrix_data() {
            try {
                if(data != NULL) delete[] data;
                cout << "detele data" << endl;
            } catch(std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }
        /**
         * @brief Add a reference to the data.
        */
        inline void add() { ++ref; }
        /**
         * @brief Release a reference to the data. If ref == 0 after this operation, the memory will be released.
        */
        void release() {
            if(ref) {
                --ref;
                if(ref == 0 && data != NULL) {
                    try {
                        delete[] data;
                    } catch(std::exception &e) {
                        std::cerr << e.what() << std::endl;
                    }
                }
            }
        }
        /**
         * @brief Get the pointer to the data.
         * @return The pointer to the data.
        */
        inline _Tp* get() const { return data; }
        inline size_t getColumns() { return columns; }
        inline size_t getRows() { return rows; }
};

/**
 * @brief Define a pixel in the matrix with more channels. _Tp here SHOULD be a basic data type.
*/
template<typename _Tp, size_t _channels>
class matrix_pixel {
    public:
        static const size_t channels = _channels;
        _Tp arr[_channels];
        _Tp& operator[] (const size_t id) {
            return id < channels ? arr[id] : arr[channels - 1];
        }
        const _Tp& operator[] (const size_t id) const {
            return id < channels ? arr[id] : arr[channels - 1];
        }
        const matrix_pixel<_Tp, _channels>& operator= (const _Tp t) {
            for(size_t i = 0; i < channels; ++i)
                (*this)[i] = t;
            return (*this);
        }
        matrix_pixel<_Tp, _channels> operator+ (const matrix_pixel<_Tp, _channels> &t) const {
            matrix_pixel<_Tp, _channels> c = *this;
            for(size_t i = 0; i < channels; ++i)
                c[i] += t[i];
            return c;
        }
        matrix_pixel<_Tp, _channels> operator- (const matrix_pixel<_Tp, _channels> &t) const {
            matrix_pixel<_Tp, _channels> c = *this;
            for(size_t i = 0; i < channels; ++i)
                c[i] -= t[i];
            return c;
        }
        matrix_pixel<_Tp, _channels>& operator+= (const matrix_pixel<_Tp, _channels> &t) const {
            for(size_t i = 0; i < channels; ++i)
                (*this)[i] += t[i];
            return *this;
        }
        matrix_pixel<_Tp, _channels>& operator-= (const matrix_pixel<_Tp, _channels> &t) const {
            for(size_t i = 0; i < channels; ++i)
                (*this)[i] -= t[i];
            return (*this);
        }
        bool operator== (const matrix_pixel<_Tp, _channels> &t) const {
            for(size_t i = 0; i < channels; ++i)
                if((*this)[i] != t[i]) return false;
            return true;
        }
        friend ostream& operator<< (ostream& os, const matrix_pixel<_Tp, _channels> &t) {
            os << '[';
            for(int i = 0; i < channels; ++i) {
                if(i) os << ", ";
                os << t.arr[i];
            }
            os << ']';
            return os;
        }
};

template<typename _Tp> class matrix_iterator;

template<typename _Tp>
class matrix {
    private:
    
        /* Attributes */
        
        /**
         * @brief The pointer to the data class of the matrix.
         * @note Users do not have accesses to this attribute so soft copy function of __matrix_data is unnecessary.
        */
        __matrix_data<_Tp>* __data;
        
        /**
         * @brief The pointer to the first element of the matrix.
        */
        _Tp* __begin;

        /**
         * @brief The ROI of this matrix. 0 <= ROI_row_begin < ROI_row_end <= getOriginalRows(), same for column.
        */
        size_t ROI_row_begin, ROI_row_end, ROI_column_begin, ROI_column_end;

    public:

        /* Constructors */

        /**
         * @brief Default constructor. It will set all pointers to NULL to avoid errors.
        */
        matrix(): __data(NULL), __begin(NULL), ROI_column_begin(0), ROI_column_end(0), ROI_row_begin(0), ROI_row_end(0) {}

        /**
         * @brief Create a new matrix with rows = row and columns = column.
        */
        matrix(size_t row, size_t column): matrix() { create(row, column); }

        /**
         * @brief (Soft) copy constructor.
        */
        matrix(const matrix<_Tp> &t): matrix() { copy(t); }

        /* Destructor */
        
        ~matrix() { release(); }
        
        /* Functions */

        /**
         * @brief Return the number of channels of a matrix element.
        */
        static size_t channels() { return _Tp::channels; }

        /**
         * @brief Get the number of rows of the original matrix.
        */
        inline size_t getOriginalRows() const { return __data == NULL ? 0 : __data->getRows(); }

        /**
         * @brief Get the number of columns of the original matrix.
        */
        inline size_t getOriginalColumns() const { return __data == NULL ? 0 : __data->getColumns(); }

        /**
         * @brief Get the number of rows of the ROI, or, the matrix now.
        */
        inline size_t getRows() const { return ROI_row_end - ROI_row_begin; }

        /**
         * @brief Get the number of columns of the ROI, or, the matrix now.
        */
        inline size_t getColumns() const { return ROI_column_end - ROI_column_begin; }

        /**
         * @brief Release current data.
        */
        void release() {
            if(__data != NULL) {
                __data->release();
                __data = NULL;
                __begin = NULL;
            }
        }

        /**
         * @brief Do a soft copy.
        */
        void copy(const matrix<_Tp> &t) {
            release();
            __data = t.__data;
            __begin = t.__begin;
            ROI_row_begin = t.ROI_row_begin;
            ROI_row_end = t.ROI_row_end;
            ROI_column_begin = t.ROI_column_begin;
            ROI_column_end = t.ROI_column_end;
            __data->add();
        }

        /**
         * @brief Create a new matrix with rows = row and columns = column.
        */
        matrix<_Tp>& create(size_t row, size_t column) {
            release();
            __data = new __matrix_data<_Tp>(row, column);
            if(__data != NULL) {
                ROI_row_begin = ROI_column_begin = 0;
                ROI_row_end = row;
                ROI_column_end = column;
                __data->add();
                __begin = __data->get();
            }
            return *this;
        }

        /**
         * @brief Do a hard copy. Use operator= instead to do a soft copy.
        */
        matrix<_Tp> clone() const {
            matrix<_Tp> *t = new matrix<_Tp>();
            t->__data = new __matrix_data<_Tp>(getRows(), getColumns());
            t->ROI_row_begin = t->ROI_column_begin = 0;
            t->ROI_row_end = getRows();
            t->ROI_column_end = getColumns();
            _Tp *ptr = t->__begin = t->__data->get(), *source_ptr = __begin;
            /**
             * @note You cannot make a deep copy of __matrix_data here, because
             * this matrix can be just a ROI of some larger matrix.
            */
            size_t source_column = getOriginalColumns();
            for(size_t i = 0; i < t->ROI_row_end; ++i) {
                for(size_t j = 0; j < t->ROI_column_end; ++j)
                    *(ptr + j) = *(source_ptr + j);
                ptr += t->ROI_column_end;
                source_ptr += source_column;
            }
            return *t;
        }

        /**
         * @brief Set all elements to zero.
        */
        matrix<_Tp>& setZero() {
            for(size_t i = 0; i < getRows(); ++i) {
                register _Tp *ptr_a = (*this)[i];
                for(size_t j = 0; j < getColumns(); ++j)
                    ptr_a[j] = 0;
            }
            return *this;
        }

        /**
         * @brief Return if all elements in two matrices are equal.
        */
        bool equal(const matrix<_Tp> &t) const {
            if(__data == NULL || t.__data == NULL) return false;
            if(__data == t.__data && ROI_column_begin == t.ROI_column_begin && ROI_column_end == t.ROI_column_end
                && ROI_row_begin == t.ROI_row_begin && ROI_row_end == t.ROI_row_end) return true;
            for(size_t i = 0; i < getRows(); ++i) {
                register _Tp *ptr_a = (*this)[i], *ptr_b = t[i];
                for(size_t j = 0; j < getColumns(); ++j)
                    if(*ptr_a[j] != *ptr_b[j])
                        return false;
            }
            return true;
        }
        
        /**
         * @brief Return if all elements in two matrices are equal. You can define how two _Tp elements are equal by define function equ.
        */
        bool equal(const matrix<_Tp> &t, bool (*equ)(const _Tp &, const _Tp&)) const {
            if(__data == NULL || t.__data == NULL) return false;
            if(__data == t.__data && ROI_column_begin == t.ROI_column_begin && ROI_column_end == t.ROI_column_end
                && ROI_row_begin == t.ROI_row_begin && ROI_row_end == t.ROI_row_end) return true;
            for(size_t i = 0; i < getRows(); ++i) {
                register _Tp *ptr_a = (*this)[i], *ptr_b = t[i];
                for(size_t j = 0; j < getColumns(); ++j)
                    if(!equ((*this)[i][j], t[i][j]))
                        return false;
            }
            return true;
        }

        /**
         * @brief The up, down, left, right border will be added by move_up, move_down, move_left, move_right, which can be negative.
         * The up and left borders will be move first, and then the down and right borders cannot be less than them.
        */
        matrix<_Tp>& adjust_ROI(int move_upper_bondary, int move_lower_bondary, int move_left_bondary, int move_right_bondary) {
            ROI_row_begin = max((size_t)0, min(getOriginalRows(), ROI_row_begin + move_upper_bondary));
            ROI_column_begin = max((size_t)0, min(getOriginalColumns(), ROI_column_begin + move_left_bondary));
            ROI_row_end = max(ROI_row_begin, min(getOriginalRows(), ROI_row_end + move_lower_bondary));
            ROI_column_end = max(ROI_column_begin, min(getOriginalColumns(), ROI_column_end + move_right_bondary));
            if(__data != NULL) __begin = __data->get() + ROI_row_begin * getOriginalColumns() + ROI_column_begin;
            return *this;
        }

        /**
         * @brief Get the element at row = r, column = c.
        */
        _Tp& at(size_t r, size_t c) const { return (*this)[r][c]; }

        /**
         * @brief Get the element at position = pos = row * getColumns() + column.
        */
        _Tp& at(size_t pos) const { return getColumns() ? (*this)[pos / getColumns()][pos % getColumns()] : (*this)[0][0]; }

        /**
         * @brief Create a soft copy matrix for rows in [start, end).
        */
        matrix<_Tp> row(size_t start, size_t end) const {
            start = min(start, getRows());
            end = min(max(start, end), getRows() + 1);
            matrix<_Tp> s = matrix<_Tp>(*this);
            s.adjust_ROI(start, getRows() - end, 0, 0);
            return s;
        }

        /**
         * @brief Create a soft copy matrix for columns in [start, end).
        */
        matrix<_Tp> column(size_t start, size_t end) const {
            start = min(start, getColumns());
            end = min(max(start, end), getColumns() + 1);
            matrix<_Tp> s = matrix<_Tp>(*this);
            s.adjust_ROI(0, 0, start, getColumns() - end);
            return s;
        }

        /**
         * @brief Create a soft copy matrix for row r.
        */
        matrix<_Tp> row(size_t r) const { return row(r, r + 1); }

        /**
         * @brief Create a soft copy matrix for column c.
        */
        matrix<_Tp> column(size_t c) const { return column(c, c + 1); }

        /* Operators */

        /**
         * @brief Do a soft copy. Use clone() instead to do a hard copy.
        */
        matrix<_Tp>& operator= (const matrix<_Tp> &t) {
            copy(t);
            return *this;
        }

        /**
         * @brief Return if all elements in two matrices are equal.
        */
        bool operator== (const matrix<_Tp> &t) const { return equal(t); }
        
        /**
         * @brief Return if all elements in two matrices are not equal.
        */
        bool operator!= (const matrix<_Tp> &t) const { return !equal(t); }

        /**
         * @brief Get the pointer of the row_th row of the matrix.
        */
        _Tp* operator[] (size_t row) const { return __begin + row * getOriginalColumns(); }

        /**
         * @brief Do this = this + t if this and t have the same size.
        */
        matrix<_Tp>& operator+= (const matrix<_Tp> &t) {
            if(this->__data != NULL && t.__data != NULL 
                && this->getRows() == t.getRows() && this->getColumns() == t.getColumns()) {
                    for(size_t i = 0; i < getRows(); ++i) {
                        _Tp *ptr = t[i], *source_ptr = (*this)[i];
                        for(size_t j = 0; j < getColumns(); ++j)
                            *(ptr + j) += *(source_ptr + j);
                    }
            }
            return *this;
        }

        /**
         * @brief Return a new matrix s = this + t if this and t have the same size, (a clone of) this otherwise.
        */
        matrix<_Tp> operator+ (const matrix<_Tp> &t) const {
            matrix<_Tp> s = clone();
            return s += t;
        }

        /**
         * @brief Do this = this + t if this and t have the same size.
        */
        matrix<_Tp>& operator-= (const matrix<_Tp> &t) {
            if(this->__data != NULL && t.__data != NULL 
                && this->getRows() == t.getRows() && this->getColumns() == t.getColumns()) {
                    for(size_t i = 0; i < getRows(); ++i) {
                        _Tp *ptr = t[i], *source_ptr = (*this)[i];
                        for(size_t j = 0; j < getColumns(); ++j)
                            *(ptr + j) -= *(source_ptr + j);
                    }
            }
            return *this;
        }

        /**
         * @brief Return a new matrix s = this + t if this and t have the same size, (a clone of) this otherwise.
        */
        matrix<_Tp> operator- (const matrix<_Tp> &t) const {
            matrix<_Tp> s = clone();
            return s -= t;
        }

        /**
         * @brief Do a matrix multiplication.
        */
        matrix<_Tp> operator* (const matrix<_Tp> &t) const {
            size_t M = getRows(), K = getColumns(), N = t.getColumns(); // (M x K) * (K x N)
            matrix<_Tp> s = matrix<_Tp>(M, N);
            s.setZero();
            if(t.getRows() == K) {
                for(size_t i = 0; i < M; ++i)
                    for(size_t k = 0; k < K; ++k) {
                        register _Tp element_a = (*this)[i][k], *ptr_s = s[i], *ptr_b = t[k];
                        for(size_t j = 0; j < N; ++j) ptr_s[j] += element_a * ptr_b[j];
                    }
            }
            return s;
        }

        /**
         * @brief The iterator to the first element of the ROI.
        */
        matrix_iterator<_Tp> begin() { return matrix_iterator<_Tp>(this); }
        
        /**
         * @brief The null iterator which means end.
        */
        matrix_iterator<_Tp> end() { return matrix_iterator<_Tp>(); }
};

template<typename _Tp>
class matrix_iterator {
    /**
     * @note When using matrix_iterator, which is mainly prepared for range-for loop of a matrix,
     * the iterator will remember the ROI of the matrix at that moment and will never change it.
     * So range-for loop can be correctly even if you use adjust_ROI() in loop body.
     * But it can cause serious errors, of course, if you use release(), copy() or operator=.
    */
    private:
        size_t row_range, column_range;
        size_t now_row, now_column;
        size_t original_column;
        _Tp* now_ptr;

    public:
        matrix_iterator() { now_ptr = NULL; }
        matrix_iterator(matrix<_Tp> *ori, size_t nr = 0, size_t nc = 0) {
            now_ptr = NULL;
            now_row = nr;
            now_column = nc;
            row_range = ori->getRows();
            column_range = ori->getColumns();
            original_column = ori->getOriginalColumns();
            if(nr < row_range && nc < column_range) now_ptr = (*ori)[nr] + nc;
        }
        matrix_iterator& operator++ () {
            if(now_ptr != NULL) {
                if(++now_column == column_range) {
                    if(++now_row == row_range) now_ptr = NULL;
                        else {
                            now_column = 0;
                            now_ptr += original_column - column_range + 1;
                        }
                } else ++now_ptr;
            }
            return *this;
        }
        inline _Tp& operator* () { return *now_ptr; }
        inline bool operator!= (const matrix_iterator<_Tp> &t) const { return now_ptr != t.now_ptr; }
};

#endif /*__MATRIX__*/