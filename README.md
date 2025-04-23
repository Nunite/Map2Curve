# Map2Curve

Map2Curve is a tool for processing Goldsource map formats and their elements.


## Manual Build Steps

### Prerequisites
- CMake 3.10 or higher
- Compatible C++ compiler (supporting C++11)
- Windows operating system

### Windows (Using Command Line)

1. Create and enter the build directory:
```
mkdir build
cd build
```

2. Configure the project:
```
cmake ..
```

3. Build the project:
```
cmake --build . --config Release
```


### Windows (Using Visual Studio)

1. Create and enter the build directory:
```
mkdir build
cd build
```

2. Generate Visual Studio project files:
```
cmake .. -G "Visual Studio 16 2019" -A Win32
```
Note: Adjust the command according to your Visual Studio version

3. Open the generated `.sln` file and build the project in Visual Studio

## Compilation Compatibility Notes

The project was originally written for GCC compiler but now includes MSVC support through the following modifications:

1. Using `msvc_compat.h` header file to handle compiler differences
2. Using `std::vector` instead of Variable Length Arrays (VLA)
3. Added macro definitions to resolve function parameter compatibility issues
4. Disabled certain warnings that would prevent compilation

If you encounter compilation issues, they typically relate to:
- VLAs (Variable Length Arrays) not being supported by MSVC
- GetVector function parameter differences
- Data type conversion warnings

## Project Information

This project is based on GNU C++ 11, originally developed using DevC++ 5.11.

For more information, please refer to the code comments or contact the original author:
- totac@web.de
- http://www.gibshooter.com

## Migration from Dev-C++ to CMake

This project was originally using Dev-C++ IDE's .dev project file format and has been converted to the cross-platform CMake build system. The conversion maintains the original functionality and structure while improving the build process.