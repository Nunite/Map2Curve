# Linux 兼容性补丁

为了解决 Linux 和 macOS 平台的编译问题，请按照以下步骤手动修改源代码文件：

## 1. 创建 Linux 兼容性头文件

创建文件 `header/linux_compat.h` 并添加以下内容：

```cpp
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
```

## 2. 修改 entity.cpp

在 `header/entity.cpp` 文件的开头添加以下代码：

```cpp
#ifdef __linux__
#include "linux_compat.h"
#endif
```

在 entity.cpp 中找到字符串 `INT_ARRAY(btable, t_brushes);`，将其替换为：

```cpp
#ifdef __linux__
int* btable = new int[t_brushes];
#else
INT_ARRAY(btable, t_brushes);
#endif
```

找到字符串 `STRING_2D_ARRAY(b_import, t_faces, 22);`，将其替换为：

```cpp
#ifdef __linux__
std::vector<std::vector<std::string>> b_import(t_faces, std::vector<std::string>(22));
#else
STRING_2D_ARRAY(b_import, t_faces, 22);
#endif
```

## 3. 修改 WAD3.cpp 和 RMF.cpp

在 `header/WAD3.cpp` 和 `header/RMF.cpp` 文件的开头添加以下代码：

```cpp
#ifdef __linux__
#include "linux_compat.h"
#endif
```

## 4. 替换 msvc_compat.h

如果在 Linux 或 macOS 上构建，创建一个空的 msvc_compat.h 文件：

```cpp
// 空的兼容性存根，用于非 Windows 平台
#ifndef MSVC_COMPAT_H
#define MSVC_COMPAT_H
// 无内容
#endif
```

## 5. 修改 CMakeLists.txt

在 CMakeLists.txt 中找到编译器选项部分，添加以下内容：

```cmake
if(UNIX AND NOT APPLE)
    # Linux 特定编译选项
    add_compile_options(-fpermissive -std=c++17 -Wno-narrowing)
    add_compile_definitions(COMPAT_LINUX)
elseif(APPLE)
    # macOS 特定编译选项
    add_compile_options(-fpermissive -std=c++17 -Wno-narrowing)
    add_compile_definitions(COMPAT_MACOS)
endif()
```

完成上述修改后，项目应该能够在 Linux 和 macOS 平台上成功编译。 