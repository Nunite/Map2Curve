#ifndef CMAKE_CONFIG_H
#define CMAKE_CONFIG_H

// 用于解决 MSVC 中可变长度数组问题的配置头文件

#ifdef _MSC_VER
// MSVC 不支持可变长度数组，使用动态分配
#define DECLARE_DYNAMIC_ARRAY(type, name, size) type* name = new type[size]
#define DELETE_DYNAMIC_ARRAY(name) delete[] name
#else
// GCC/Clang 支持可变长度数组
#define DECLARE_DYNAMIC_ARRAY(type, name, size) type name[size]
#define DELETE_DYNAMIC_ARRAY(name) 
#endif

#endif // CMAKE_CONFIG_H