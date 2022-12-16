# Project 5 -  A Class for Matrices

**SID:** 12111012 **Name:** 匡亮(KuangLiang)

----

#### Part One - Introduction and Highlights

这个 project 中，我实现了 ```matrix``` 类。它可以存储一个二维的矩阵，支持多种数据类型（包括自定义的多通道像素类 ```matrix_pixel``` ），支持常见的几种运算符，支持软拷贝和硬拷贝。

矩阵的具体内容并不存储在矩阵类中，而是存储在另一个类 ```__matrix_data``` 中。这个类会记录自己被多少个矩阵访问，当访问数降到 $0$ 的时候就释放记录内容占用的内存。因此 ```matrix``` 的软拷贝和设置 ROI 都是 O(1) 时间复杂度的。

如果采用了 C++11 或更高的标准，则可以使用 range_for loop ，即 ```for(auto i : matrix)``` 来按行的顺序访问矩阵中的每一个元素。

#### Part Two - Classes and Interfaces

1. ```class __matrix_data```
   
用于存储矩阵具体内容的类。每个矩阵都有一个这个类的指针，用于访问自己拥有的数据。这个类会记录当前有多少个矩阵对象拥有自己的指针，矩阵对象则负责维护这个变量。当这个变量降至 $0$ ，这个类将回收自己。

**用户没办法访问一个类指向的数据类，**只能访问它指向的数据，因此它的安全性比较高，不会发生提早或过晚释放的情况。

2. ```template<typename _Tp, size_t _channels> class matrix_pixel```

一个元素类型为 _Tp ，通道数为 _channels 的像素点，建议 _Tp 为基本数据类型之一。这个类重载过 ```std::ostream& operator<<``` ，故可以直接用 ```cout``` 输出内容。

3. ```template<typename _Tp> class matrix```

一个元素类型为 _Tp 的矩阵，建议 _Tp 为基本数据类型之一或 matrix_pixel 。

##### 构造函数

```matrix()```: 默认构造函数。

```matrix(size_t row, size_t column)```: 新建一个 $row$ 行 $column$ 列的矩阵。

```matrix(const matrix<_Tp> &t)```: （软）拷贝构造函数。

##### 成员函数

```matrix<_Tp>& clone() const```: 返回一个当前矩阵的深拷贝。

```matrix<_Tp>& setZero()```: 将所有元素置零，返回自身的引用。

```bool equal(const matrix<_Tp> &t) const```: 判断两个矩阵是否相等。也可以直接使用 == 。

```bool equal(const matrix<_Tp> &t, bool (*equ)(const _Tp &, const _Tp&)) const```: 判断两个矩阵是否相等，自定义如何比较两个元素是否相等。适用于矩阵元素类型为 float 或 double 之类，需要认可较小差距为相等的情况。

```matrix<_Tp>& adjust_ROI(int move_up, int move_down, int move_left, int move_right)```: 调整当前矩阵的 ROI 范围，返回自身的引用。

```_Tp& at(size_t r, size_t c) const```: 返回 $r$ 行 $c$ 列位置的元素的引用。

```matrix<_Tp> row(size_t start, size_t end) const```: 返回一个 $[start,end)$ 行构成的子矩阵的软拷贝（生成软拷贝后执行 adjust_ROI ）。类似的还有 ```column``` 。

