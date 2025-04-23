#ifndef VLA_FIX_H
#define VLA_FIX_H

#include <vector>
#include <memory>
#include <algorithm>

// 用于替换可变长度数组的宏
#define DECLARE_DYNAMIC_ARRAY(type, name, size) std::vector<type> name(size)
#define DYNAMIC_ARRAY(name) name.data()
#define DYNAMIC_ARRAY_ELEMENT(name, index) name[index]

// 新增功能：二维动态数组
#define DECLARE_DYNAMIC_ARRAY_2D(type, name, rows, cols) std::vector<std::vector<type>> name(rows, std::vector<type>(cols))
#define DYNAMIC_ARRAY_2D_ELEMENT(name, row, col) name[row][col]

// 新增功能：使用连续内存的二维数组 (性能可能更好)
#define DECLARE_FLAT_ARRAY_2D(type, name, rows, cols) std::vector<type> name((rows) * (cols))
#define FLAT_ARRAY_2D_ELEMENT(name, row, col, width) name[(row) * (width) + (col)]

// 新增功能：清零数组
template<typename T>
inline void clear_array(std::vector<T>& arr) {
    std::fill(arr.begin(), arr.end(), T());
}

// 新增功能：深度复制数组
template<typename T>
inline void copy_array(const std::vector<T>& src, std::vector<T>& dst) {
    dst.resize(src.size());
    std::copy(src.begin(), src.end(), dst.begin());
}

// 使用智能指针管理C风格数组
template<typename T>
using SmartArray = std::unique_ptr<T[]>;

template<typename T>
SmartArray<T> make_smart_array(size_t size) {
    return SmartArray<T>(new T[size]());
}

// 重置并保留数组大小
template<typename T>
inline void reset_array(std::vector<T>& arr) {
    size_t size = arr.size();
    arr.clear();
    arr.resize(size);
}

#endif // VLA_FIX_H