#ifndef MSVC_COMPAT_H
#define MSVC_COMPAT_H

// 此文件解决MSVC编译器兼容性问题

#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include <iostream>
#include <string>

// 优先检查MSVC
#ifdef _MSC_VER
    // MSVC编译器

    // 1. 可变长度数组(VLA)不被MSVC支持，使用std::vector替代
    #define VLA(type, name, size) std::vector<type> name(size)
    
    // 访问VLA的元素
    #define VLA_ELEMENT(name, i) name[i]
    
    // 更多VLA替代宏
    #define VLA_2D(type, name, rows, cols) std::vector<std::vector<type>> name(rows, std::vector<type>(cols))
    #define VLA_2D_AT(name, r, c) name[r][c]
    
    // 更高性能的VLA替代方案 - 使用单一内存块的扁平数组
    #define VLA_FLAT_2D(type, name, rows, cols) std::vector<type> name((rows) * (cols))
    #define VLA_FLAT_2D_AT(name, r, c, cols) name[(r) * (cols) + (c)]
    
    // 2. 解决GetVector一个参数的调用问题
    #include "vertex.h"
    extern vertex Zero;
    #define GET_VECTOR(arg) GetVector(Zero, arg)
    
    // 3. 支持round函数在MSVC中的使用
    #ifndef round
    inline double round(double value) {
        return floor(value + 0.5);
    }
    #endif
    
    // 4. 用于替换数组声明中的非常量表达式
    #define ARRAY_SIZE(size) ((size > 0) ? size : 1)
    
    // 5. 函数兼容性宏，对多个版本的GetVector提供兼容性
    #define COMPAT_GETVECTOR1(a) GetVector(Zero, a)
    #define COMPAT_GETVECTOR2(a, b) GetVector(a, b)
    
    // 6. 智能指针版本的动态数组
    #define SMART_ARRAY(type, name, size) std::unique_ptr<type[]> name(new type[size])
    #define SMART_ARRAY_AT(name, i) name[i]
    
    // 7. 处理结构体/类成员中的可变长度数组
    #define MEMBER_VLA(type, name, size) std::vector<type> name = std::vector<type>(size)
    
    // 8. 处理特定函数的VLA问题 (frames.cpp中的特定问题)
    #define PATH_CORNER_ARRAY(name, size) std::vector<path_corner> name(size)
    #define GVECTOR_ARRAY(name, size) std::vector<gvector> name(size)
    #define FLOAT_ARRAY(name, size) std::vector<float> name(size)
    #define INT_ARRAY(name, size) std::vector<int> name(size)
    #define BOOL_ARRAY(name, size) std::vector<bool> name(size)
    
    // 9. 预处理条件指令
    #define IF_MSVC(code) code
    #define IF_NOT_MSVC(code)
    
    // 10. 处理更多特殊类型的数组
    #define CHAR_ARRAY(name, size) std::vector<char> name(size)
    #define STRING_ARRAY(name, size) std::vector<std::string> name(size)
    #define STRING_2D_ARRAY(name, rows, cols) std::vector<std::vector<std::string>> name(rows, std::vector<std::string>(cols))
    
    // 11. 解决std::stof/stod调用不明确的问题
    namespace compat {
        inline float stof(const std::string& str) {
            return std::stof(str);
        }
        inline double stod(const std::string& str) {
            return std::stod(str);
        }
    }
    #define COMPAT_STOF(str) compat::stof(str)
    #define COMPAT_STOD(str) compat::stod(str)
    
    // 12. 处理类成员中的可变长度数组，也可传递初始值
    #define MEMBER_VLA_INIT(type, name, size, init_val) std::vector<type> name = std::vector<type>(size, init_val)
    
    // 13. 特定于RMF.cpp的兼容宏
    #define CHAR_VLA(name, length) std::vector<char> name(length)
    
#else
    // GCC/其他编译器
    
    // 对于支持VLA的编译器，直接使用原生语法
    #define VLA(type, name, size) type name[size]
    #define VLA_ELEMENT(name, i) name[i]
    
    // 扩展的VLA宏
    #define VLA_2D(type, name, rows, cols) type name[rows][cols]
    #define VLA_2D_AT(name, r, c) name[r][c]
    
    // 扁平化VLA
    #define VLA_FLAT_2D(type, name, rows, cols) type name[(rows) * (cols)]
    #define VLA_FLAT_2D_AT(name, r, c, cols) name[(r) * (cols) + (c)]
    
    // 不需要处理GetVector的兼容性问题
    #define GET_VECTOR(arg) GetVector(arg)
    
    // 传递常量表达式
    #define ARRAY_SIZE(size) size
    
    // 原始函数调用
    #define COMPAT_GETVECTOR1(a) GetVector(a)
    #define COMPAT_GETVECTOR2(a, b) GetVector(a, b)
    
    // 普通数组版本
    #define SMART_ARRAY(type, name, size) type name[size]
    #define SMART_ARRAY_AT(name, i) name[i]
    
    // 成员VLA
    #define MEMBER_VLA(type, name, size) type name[size]
    
    // frames.cpp特定宏
    #define PATH_CORNER_ARRAY(name, size) path_corner name[size]
    #define GVECTOR_ARRAY(name, size) gvector name[size]
    #define FLOAT_ARRAY(name, size) float name[size]
    #define INT_ARRAY(name, size) int name[size]
    #define BOOL_ARRAY(name, size) bool name[size]
    
    // 更多特殊类型数组
    #define CHAR_ARRAY(name, size) char name[size]
    #define STRING_ARRAY(name, size) std::string name[size]
    #define STRING_2D_ARRAY(name, rows, cols) std::string name[rows][cols]
    
    // 函数调用兼容性
    #define COMPAT_STOF(str) std::stof(str)
    #define COMPAT_STOD(str) std::stod(str)
    
    // 成员VLA初始化
    #define MEMBER_VLA_INIT(type, name, size, init_val) type name[size]
    
    // RMF.cpp特定宏
    #define CHAR_VLA(name, length) char name[length]
    
    // 预处理条件指令
    #define IF_MSVC(code)
    #define IF_NOT_MSVC(code) code
#endif

// 对所有编译器，定义一个简化的错误捕获宏
#define TRY_CATCH(code, error_msg) try { code; } catch(...) { std::cerr << error_msg << std::endl; }

// 提供通用的辅助函数
namespace compat {
    // 创建二维数组
    template<typename T>
    inline std::vector<std::vector<T>> create2DArray(size_t rows, size_t cols) {
        return std::vector<std::vector<T>>(rows, std::vector<T>(cols));
    }
    
    // 创建二维数组并初始化
    template<typename T>
    inline std::vector<std::vector<T>> create2DArray(size_t rows, size_t cols, const T& initValue) {
        return std::vector<std::vector<T>>(rows, std::vector<T>(cols, initValue));
    }
    
    // 创建一维数组
    template<typename T>
    inline std::vector<T> createArray(size_t size) {
        return std::vector<T>(size);
    }
    
    // 创建一维数组并初始化
    template<typename T>
    inline std::vector<T> createArray(size_t size, const T& initValue) {
        return std::vector<T>(size, initValue);
    }
    
    // 安全字符串转浮点数
    inline float safeStof(const std::string& str, float defaultValue = 0.0f) {
        try {
            return std::stof(str);
        } catch (...) {
            return defaultValue;
        }
    }
    
    // 安全字符串转双精度浮点数
    inline double safeStod(const std::string& str, double defaultValue = 0.0) {
        try {
            return std::stod(str);
        } catch (...) {
            return defaultValue;
        }
    }
}

#endif // MSVC_COMPAT_H
