#ifndef __MATRIX__
#define __MATRIX__

#include <stddef.h> // size_t
#include <exception> // exception
#include <iostream> // cerr

template<typename _Tp>
class __matrix_data {
    private:
        /**
         * @brief The number of references to the data.
        */
        size_t ref;
        /**
         * @brief The length of the data.
        */
        size_t length;
        /**
         * @brief The pointer to the head of data.
        */
        _Tp* data;
    
    public:
        __matrix_data(): data(NULL), ref(0), length(0) {}
        /**
         * @brief Create a __matrix_data of length len.
        */
        __matrix_data(size_t len): __matrix_data(), length(len) {
            try {
                data = new _Tp[length];
            } catch(std::exception &e) {
                std::cerr << e.what() << std::endl;
                data = NULL;
            }
        }
        ~__matrix_data() {
            try {
                if(data != NULL) delete[] data;
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
        inline void release() {
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
        bool operator== (const matrix_pixel<_Tp, _channels> &t) const {
            for(size_t i = 0; i < channels; ++i)
                if((*this)[i] != t[i]) return false;
            return true;
        }
};

template<typename _Tp>
class matrix {
    private:
    
    /* Attributes */

    /**
     * @brief The number of rows of the matrix.
    */
    size_t rows;
    
    /**
     * @brief The number of columns of the matrix.
    */
    size_t columns;
    
    /**
     * @brief The number of elements between two rows.
    */
    size_t gap;
    
    /**
     * @brief The pointer to the data class of the matrix.
     * @note Users do not have accesses to this attribute so soft copy function of __matrix_data is unnecessary.
    */
    __matrix_data<_Tp>* __data;
    
    /**
     * @brief The pointer to the first element of the matrix.
    */
    _Tp* __begin;

    /* Functions */

    /**
     * @brief Release current data.
    */
    inline void release() {
        if(__data != NULL) {
            __data->release();
            __data = NULL;
            __begin = NULL;
        }
    }

    /**
     * @brief Do a soft copy.
    */
    inline void copy(const matrix<_Tp> &t) {
        release();
        this->rows = t.rows;
        this->columns = t.columns;
        this->gap = t.gap;
        this->__data = t.__data;
        this->__begin = t.__begin;
        this->__data->add();
    }

    public:

    /* Constructors */

    /**
     * @brief Default constructor. It will set all pointers to NULL to avoid errors.
    */
    matrix(): __data(NULL), __begin(NULL), rows(0), columns(0), gap(0) {}

    /**
     * @brief Create a new matrix with rows = row and columns = column.
    */
    matrix(size_t row, size_t column): matrix(), rows(row), columns(column), gap(column) {
        __data = __matrix_data<_Tp>(row * column);
        __data->add();
        __begin = __data->get();
    }

    /**
     * @brief (Soft) copy constructor.
    */
    matrix(const matrix<_Tp> &t): matrix() {
        this->copy(t);
    }

    /* Destructor */
    
    ~matrix() { release(); }
    
    /* Functions */

    /**
     * @brief Get the number of rows.
     * @return The number of rows.
    */
    inline size_t getRows() const { return this->rows; }

    /**
     * @brief Get the number of columns.
     * @return The number of columns.
    */
    inline size_t getColumns() const { return this->columns; }

    /**
     * @brief Do a hard copy. Use operator= instead to do a soft copy.
    */
    matrix<_Tp>& clone() const {
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
            for(int j = 0; j < t.columns; ++j) *(ptr + j) = *(sptr + j);
            ptr += t.columns;
            sptr += gap;
        }
        return t;
    }

    /* Operators */

    /**
     * @brief Do a soft copy. Use clone() instead to do a hard copy.
    */
    void operator= (const matrix<_Tp> &t) {
        this->copy(t);
    }

};

#endif /*__MATRIX__*/