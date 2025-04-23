cmake_minimum_required(VERSION 3.10)
project(Map2Curve)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加详细的编译日志
set(CMAKE_VERBOSE_MAKEFILE OFF)

# 根据不同编译器设置选项
if(MSVC)
    # Microsoft Visual C++ 编译器选项
    # 禁用特定警告，包括C4244（可能的数据丢失）和C4996（不安全函数）
    add_compile_options(
        /W3          # 警告级别3
        /wd4244      # 忽略数据类型转换的警告
        /wd4267      # 忽略size_t到int转换的警告
        /wd4305      # 忽略double到float截断警告
        /wd4996      # 忽略不安全函数警告
        /wd4819      # 忽略Unicode警告
        /wd4189      # 忽略未使用变量警告
        /wd4456      # 忽略局部变量隐藏警告
        /wd4458      # 忽略类成员隐藏警告
        /wd4100      # 忽略未使用参数警告
        /wd4127      # 忽略条件表达式是常量的警告
        /wd4551      # 忽略缺少参数列表的函数调用警告
        /wd4828      # 忽略字符集编码警告
        /bigobj      # 支持大型对象文件
    )
    # 启用多处理器编译
    add_compile_options(/MP)
    # 处理 Unicode 字符集问题
    add_compile_options(/utf-8)
else()
    # GCC/Clang 编译器选项
    add_compile_options(-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers)
    # 允许使用可变长度数组
    add_compile_options(-Wno-vla)
    
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # 特定于GCC的选项
        add_compile_options(-Wno-pedantic -Wno-double-promotion -Wno-int-in-bool-context)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # 特定于Clang的选项
        add_compile_options(-Wno-unused-private-field -Wno-vla-extension)
    endif()
endif()

# 设置库类型 (静态 vs 共享)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# 根据操作系统添加特定定义
if(WIN32)
    # 添加图标资源
    set(MAP2CURVE_ICON_RESOURCE "${CMAKE_SOURCE_DIR}/Map2Curve_private.rc")
    # Windows特定定义
    add_definitions(
        -DWINDOWS
        -D_USE_MATH_DEFINES
        -D_CRT_SECURE_NO_WARNINGS
        -DNOMINMAX
    )
elseif(APPLE)
    add_definitions(-DAPPLE -D_USE_MATH_DEFINES)
elseif(UNIX AND NOT APPLE)
    add_definitions(-DLINUX -D_USE_MATH_DEFINES)
endif()

# 应用程序版本设置
set(VERSION_MAJOR 0)
set(VERSION_MINOR 8)
set(VERSION_PATCH 0)
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# 搜索项目文件 - 修复源文件搜索路径
file(GLOB CPP_FILES "*.cpp")
file(GLOB HEADER_CPP_FILES "header/*.cpp")
file(GLOB HEADERS "header/*.h" "header/*.hpp")

# 移除问题文件 dirent.h (与MSVC不兼容)
list(REMOVE_ITEM HEADERS "${CMAKE_SOURCE_DIR}/header/dirent.h")

set(SOURCES ${CPP_FILES} ${HEADER_CPP_FILES})

# 添加库搜索路径
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/header
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${MAP2CURVE_ICON_RESOURCE})

# MSVC特定定义 - 增加兼容性
if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX # 避免min/max宏冲突
        _MSVC_VLA_COMPAT_ # 自定义标记，表示使用MSVC VLA兼容模式
        _SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING # 禁用result_of警告
        _MSVC_VLA_WORKAROUND # 添加VLA替代方案标记
        WIN32_LEAN_AND_MEAN # 减少Windows头文件包含
        _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS # 禁用所有C++17弃用警告
    )
    
    # 强制包含兼容性头文件
    target_compile_options(${PROJECT_NAME} PRIVATE 
        /FI"header/msvc_compat.h" # 强制包含兼容性头文件
    )
else()
    # GCC兼容性定义
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        _GCC_VLA_SUPPORT # 表示GCC原生支持VLA
    )
endif()

# 设置可执行文件的版本属性
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${VERSION})

# 如果在Windows平台下，添加清理目标
if(WIN32)
    add_custom_target(clean-all
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
        COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
        COMMENT "Cleaning all generated files...")
        
    # 为MSVC添加对dirent.h的处理
    if(MSVC)
        # 完全排除dirent.h
        add_compile_definitions(NO_DIRENT=1)
        
        # 排除dirent.h相关的源文件
        set_source_files_properties(
            "${CMAKE_SOURCE_DIR}/header/utils.cpp"
            PROPERTIES COMPILE_DEFINITIONS "NO_DIRENT=1;USE_WIN32_FIND=1"
        )
        
        # 创建一个空的dirent.h替代文件（如果需要）
        file(WRITE "${CMAKE_BINARY_DIR}/dirent_stub.h" 
            "#ifndef _DIRENT_STUB_H_\n#define _DIRENT_STUB_H_\n"
            "// 此文件是dirent.h的存根，用于禁用相关功能\n"
            "#define NO_DIRENT 1\n"
            "#ifdef _MSC_VER\n"
            "typedef void DIR;\n"
            "struct dirent { char d_name[260]; };\n"
            "#endif\n"
            "#endif // _DIRENT_STUB_H_\n"
        )
        
        # 将存根目录添加到包含路径
        include_directories(BEFORE "${CMAKE_BINARY_DIR}")
    endif()
endif()