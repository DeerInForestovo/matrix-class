# Project 5 -  A Class for Matrices

**SID:** 12111012 **Name:** 匡亮(KuangLiang)

----

#### Part One - Introduction and Highlights

这个 project 中，我实现了 ```matrix``` 类。它可以存储一个二维的矩阵，支持多种数据类型（包括自定义的多通道像素类 ```matrix_pixel``` ），支持常见的几种运算符，支持软拷贝和硬拷贝。

矩阵的具体内容并不存储在矩阵类中，而是存储在另一个类 ```__matrix_data``` 中。这个类会记录自己被多少个矩阵访问，当访问数降到 $0$ 的时候就释放记录内容占用的内存。因此 ```matrix``` 的软拷贝和设置 ROI 都是 O(1) 时间复杂度的。

如果采用了 C++11 或更高的标准，则可以使用一些适合新版本的功能，如 ```forEach``` 等。如果不采用，同样可以通过编译。

#### Part Two - Classes and Interfaces

1. ```class __matrix_data```
   
用户不应该用这个类，因此不提供文字说明（但代码中有完整的成员变量解释和接口说明）。

2. ```template<typename _Tp, size_t _channels> class matrix_pixel```

一个元素类型为 _Tp ，通道数为 _channels 的像素点，_Tp 应为基本数据类型之一，否则它需要重载过 += 、-= 、== 这三个运算符。

3. ```template<typename _Tp> class matrix```

一个元素类型为 _Tp 的矩阵，建议 _Tp 为基本数据类型之一或 matrix_pixel 。

```matrix<_Tp>& clone() const```: 返回一个当前矩阵的深拷贝。

