#ifndef VLA_UTILS_H
#define VLA_UTILS_H

#include <vector>
#include <memory>
#include <utility>

// 解决可变长度数组问题的辅助宏和模板
#define VLA_ARRAY(Type, Name, Size) std::vector<Type> Name(Size)

// 针对不同维度数组的辅助宏
#ifdef _MSC_VER
// MSVC版本 - 使用动态内存分配
#define VLA_1D(Type, Name, Size) std::vector<Type> Name(Size)
#define VLA_2D(Type, Name, Size1, Size2) std::vector<std::vector<Type>> Name(Size1, std::vector<Type>(Size2))
#define VLA_3D(Type, Name, Size1, Size2, Size3) std::vector<std::vector<std::vector<Type>>> Name(Size1, std::vector<std::vector<Type>>(Size2, std::vector<Type>(Size3)))
#define VLA_MATRIX(Type, Name, Rows, Cols) std::vector<std::vector<Type>> Name(Rows, std::vector<Type>(Cols))

// 自动内存管理替代方案
#define VLA_AUTO_ARRAY(Type, Name, Size) std::unique_ptr<Type[]> Name(new Type[Size])
#define VLA_AUTO_2D(Type, Name, Size1, Size2) std::unique_ptr<Type[]> Name(new Type[Size1 * Size2])

// 访问宏 - 使代码更清晰
#define VLA_AT(Array, Index) (Array)[Index]
#define VLA_AT_2D(Array, Row, Col) (Array)[Row][Col]
#define VLA_AT_AUTO_2D(Array, Row, Col, Width) (Array)[(Row) * (Width) + (Col)]
#define VLA_AT_3D(Array, Dim1, Dim2, Dim3) (Array)[Dim1][Dim2][Dim3]

// 特殊宏 - 用于替换代码中的C风格数组声明
#define VLA_ALLOC_1D(Type, Size) (std::vector<Type>(Size))
#define VLA_ALLOC_2D(Type, Size1, Size2) (std::vector<std::vector<Type>>(Size1, std::vector<Type>(Size2)))

#else
// GCC/Clang 版本 - 直接使用VLA语法
#define VLA_1D(Type, Name, Size) Type Name[Size]
#define VLA_2D(Type, Name, Size1, Size2) Type Name[Size1][Size2]
#define VLA_3D(Type, Name, Size1, Size2, Size3) Type Name[Size1][Size2][Size3]
#define VLA_MATRIX(Type, Name, Rows, Cols) Type Name[Rows][Cols]

// 自动内存管理替代方案 - GCC下仍使用栈分配
#define VLA_AUTO_ARRAY(Type, Name, Size) Type Name[Size]
#define VLA_AUTO_2D(Type, Name, Size1, Size2) Type Name[Size1][Size2]

// 访问宏 - GCC下直接使用数组语法
#define VLA_AT(Array, Index) (Array)[Index]
#define VLA_AT_2D(Array, Row, Col) (Array)[Row][Col]
#define VLA_AT_AUTO_2D(Array, Row, Col, Width) (Array)[Row][Col]
#define VLA_AT_3D(Array, Dim1, Dim2, Dim3) (Array)[Dim1][Dim2][Dim3]

// 特殊宏 - 在GCC下返回普通数组（不可直接用于赋值）
#define VLA_ALLOC_1D(Type, Size) /* 在GCC下不做预分配 */
#define VLA_ALLOC_2D(Type, Size1, Size2) /* 在GCC下不做预分配 */
#endif

// 固定大小数组的工具函数
template<typename T, int Rows, int Cols>
using Matrix = std::vector<std::vector<T>>;

template<typename T, int Rows, int Cols>
inline Matrix<T, Rows, Cols> createMatrix() {
    return std::vector<std::vector<T>>(Rows, std::vector<T>(Cols));
}

// 动态大小矩阵创建函数
template<typename T>
inline std::vector<std::vector<T>> createMatrix(size_t rows, size_t cols) {
    return std::vector<std::vector<T>>(rows, std::vector<T>(cols));
}

// 矩阵初始化函数
template<typename T>
inline void initMatrix(std::vector<std::vector<T>>& matrix, const T& value) {
    for (auto& row : matrix) {
        std::fill(row.begin(), row.end(), value);
    }
}

// 创建预初始化的矩阵
template<typename T>
inline std::vector<std::vector<T>> createMatrix(size_t rows, size_t cols, const T& value) {
    std::vector<std::vector<T>> matrix(rows, std::vector<T>(cols, value));
    return matrix;
}

#endif // VLA_UTILS_H