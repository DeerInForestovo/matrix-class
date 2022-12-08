#ifndef __MATRIX__
#define __MATRIX__

#include <stddef.h> // size_t

template<typename _Tp>
class __matrix_data {
    private:
        size_t ref;
        _Tp* data;
    
    public:
        __matrix_data();
        __matrix_data(size_t length);
        ~__matrix_data();
        /**
         * @brief Add a reference to the data.
        */
        inline void add();
        /**
         * @brief Release a reference to the data. If there's no reference after this operation, the memory will be released.
        */
        inline void release();
        /**
         * @brief Get the pointer to the data.
         * @return The pointer to the data.
        */
        inline _Tp* get() const ;
};

template<typename _Tp>
class matrix {
    private:
    
    /* Attributes */

    /**
     * @brief The number of rows of the matrix
    */
    size_t rows;
    
    /**
     * @brief The number of columns of the matrix
    */
    size_t columns;
    
    /**
     * @brief The number of elements between two rows
    */
    size_t gap;
    
    /**
     * @brief The pointer to the data class of the matrix
    */
    __matrix_data<_Tp>* __data;
    
    /**
     * @brief The pointer to the first element of the matrix
    */
    _Tp* __begin;

    /* Functions */

    /**
     * @brief Release current data
    */
    inline void release();

    public:

    /* Constructors */
    matrix();
    matrix(size_t row, size_t column);

    /* Destructor */
    ~matrix();
    
    /* Functions */

    /**
     * @brief Get the number of rows
     * @return The number of rows
    */
    inline size_t getRows() const;

    /**
     * @brief Get the number of columns
     * @return The number of columns
    */
    inline size_t getColumns() const;

    /* Operators */

    /**
     * @brief Copy constructor
    */
    void operator= (const matrix<_Tp> &t);

};

#endif /*__MATRIX__*/