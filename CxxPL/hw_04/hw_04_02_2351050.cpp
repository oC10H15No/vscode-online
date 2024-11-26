/***
 Implement the class of a 4X4 elemType matrix, providing interface as follows:
# matrix add
# matrix multiply
# print
# operator + =
# subscripting function object
## elemType& operator()(int row, int column);
## elemType operator(int row, int column) const;
# Default constructor that accepts 16 values from standard device
# Another constructor that accept an array of size 16
*/

#include <iostream>

typedef float elemType;

class Matrix4X4
{
private:
    elemType _matrix[4][4];
public:
    // 构造函数，接受一个大小为16的数组
    Matrix4X4(const elemType *arr)
    {
        int arr_index = 0;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                _matrix[i][j] = arr[arr_index++];
            }
        }
    }
    // 构造函数，接受16个浮点数
    Matrix4X4(elemType a11 = 0, elemType a12 = 0, elemType a13 = 0, elemType a14 = 0,
              elemType a21 = 0, elemType a22 = 0, elemType a23 = 0, elemType a24 = 0,
              elemType a31 = 0, elemType a32 = 0, elemType a33 = 0, elemType a34 = 0,
              elemType a41 = 0, elemType a42 = 0, elemType a43 = 0, elemType a44 = 0)
    {
        _matrix[0][0] = a11;
        _matrix[0][1] = a12;
        _matrix[0][2] = a13;
        _matrix[0][3] = a14;
        _matrix[1][0] = a21;
        _matrix[1][1] = a22;
        _matrix[1][2] = a23;
        _matrix[1][3] = a24;
        _matrix[2][0] = a31;
        _matrix[2][1] = a32;
        _matrix[2][2] = a33;
        _matrix[2][3] = a34;
        _matrix[3][0] = a41;
        _matrix[3][1] = a42;
        _matrix[3][2] = a43;
        _matrix[3][3] = a44;
    }
    
   // 重载 + 运算符
    Matrix4X4 operator+(const Matrix4X4 &m) const
    {
        Matrix4X4 result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result._matrix[i][j] = _matrix[i][j] + m._matrix[i][j];
            }
        }
        return result;
    }
    // 重载 += 运算符
    void operator+=(const Matrix4X4 &m)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                _matrix[i][j] += m._matrix[i][j];
            }
        }
    }
    // 重载 * 运算符
    Matrix4X4 operator*(const Matrix4X4 &m) const
    {
        Matrix4X4 result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result._matrix[i][j] = 0;
                for (int k = 0; k < 4; ++k)
                {
                    result._matrix[i][j] += _matrix[i][k] * m._matrix[k][j];
                }
            }
        }
        return result;
    }
    // 打印矩阵
    std::ostream &print(std::ostream &os) const
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                os << _matrix[i][j] << ' ';
            }
            os << std::endl;
        }
        return os;
    }
    // 重载 << 运算符
    friend std::ostream &operator<<(std::ostream &os, const Matrix4X4 &matrix)
    {
        return matrix.print(os);
    }
    // 重载 () 运算符
    elemType &operator()(int row, int col)
    {
        return _matrix[row][col];
    }
    // 重载 () 运算符, const
    elemType operator()(int row, int col) const
    {
        return _matrix[row][col];
    }
};

int main()
{
    Matrix4X4 m0;
    std::cout << "Default Matrix: " << std::endl;
    std::cout << m0 << std::endl;

    Matrix4X4 m1(1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);

    std::cout << "Identity Matrix accepts 16 values from standard device: " << std::endl;
    std::cout << m1 << std::endl;

    Matrix4X4 m2(m1);
    std::cout << "Copy of Identity Matrix using copy assignment: " << std::endl;
    std::cout << m2 << std::endl;

    Matrix4X4 m3 = m1;
    std::cout << "Copy of Identity Matrix using \'=\' operator: " << std::endl;
    std::cout << m3 << std::endl;

    elemType arr[16] = {0.2, 0.3, 0.4, 0.6,
                        0.6, 0.9, 0.8, 1.2,
                        1.0, 1.5, 1.2, 1.8,
                        1.4, 2.1, 1.6, 2.4};
    Matrix4X4 m4(arr);
    std::cout << "Matrix accepts an array of size 16: " << std::endl;
    std::cout << m4 << std::endl;

    Matrix4X4 m5 = m4 * m1;
    std::cout << "Matrix multiplication: " << std::endl;
    std::cout << m5 << std::endl;

    Matrix4X4 m6 = m4 + m5;
    std::cout << "Matrix addition: " << std::endl;
    std::cout << m6 << std::endl;

    m4 += m5;
    std::cout << "Matrix addition using \'+=\' operator: " << std::endl;
    std::cout << m4 << std::endl;

    // 使用非常量版本的 () 运算符修改矩阵元素
    std::cout << "M1(2, 2) before modification: " << m1(2, 2) << std::endl;
    m1(2, 2) = 2.0f;
    std::cout << "M1(2, 2) after modification: " << m1(2, 2) << std::endl;

    // 使用常量版本的 () 运算符读取矩阵元素
    const Matrix4X4 m7(m1);
    std::cout << "M7(2, 2) const: " << m7(2, 2) << std::endl;

    return 0;
}