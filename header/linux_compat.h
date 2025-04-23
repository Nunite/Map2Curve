#ifndef LINUX_COMPAT_H
#define LINUX_COMPAT_H

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include <algorithm>
#include <functional>

// 基本类型定义
typedef int btable;

// 跨平台宏
#define COMPAT_STOF(x) std::stof(std::string(x))
#define COMPAT_STOD(x) std::stod(std::string(x))
#define COMPAT_STOI(x) std::stoi(std::string(x))
#define STRING_2D_ARRAY(name, dim1, dim2) std::vector<std::vector<std::string>> name(dim1, std::vector<std::string>(dim2))
#define INT_ARRAY(name, size) int* name = new int[size]
#define NO_DIRENT 1

#endif // LINUX_COMPAT_H