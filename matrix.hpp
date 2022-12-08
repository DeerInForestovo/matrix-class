#ifndef __MATRIX__
#define __MATRIX__

#include <stddef.h> // size_t

template<typename _Tp>
class __matrix_data {
    private:
        size_t ref;
        size_t length;
        _Tp* data;
    
    public:
        __matrix_data();
        __matrix_data(size_t len);
        ~__matrix_data();
        /**
         * @brief Add a reference to the data.
        */
        inline void add();
        /**
         * @brief Release a reference to the data. If ref == 0 after this operation, the memory will be released.
        */
        inline void release();
        /**
         * @brief Get the pointer to the data.
         * @return The pointer to the data.
        */
        inline _Tp* get() const;
        /**
         * @brief Do deep copy.
         * @return A new __matrix_data object with the same data and ref = 0.
        */
        __matrix_data<_Tp>& clone() const;

        /**
         * @note I did not implement a soft copy function of __matrix_data because my class matrix will handle it for me.
        */
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
    inline void release();

    public:

    /* Constructors */

    /**
     * @brief Default constructor. It will set all pointers to NULL to avoid errors.
    */
    matrix();

    /**
     * @brief Create a new matrix with rows = row and columns = column.
    */
    matrix(size_t row, size_t column);

    /**
     * @brief (Soft) copy constructor.
    */
    matrix(const matrix<_Tp> &t);

    /* Destructor */
    
    ~matrix();
    
    /* Functions */

    /**
     * @brief Get the number of rows.
     * @return The number of rows.
    */
    inline size_t getRows() const;

    /**
     * @brief Get the number of columns.
     * @return The number of columns.
    */
    inline size_t getColumns() const;

    matrix<_Tp>& clone() const;

    /* Operators */

    /**
     * @brief Do a soft copy. Use clone() instead to do a hard copy.
    */
    void operator= (const matrix<_Tp> &t);

};

#endif /*__MATRIX__*/