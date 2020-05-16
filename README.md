# CMake SensorTile Example
This repository is a port of the basic [STM32 SensorTile](https://www.st.com/en/evaluation-tools/steval-stlkt01v1.html)
logging example using CMake. No source code has been changed, the files have just been shuffled around and
broken down into modules. This demonstrates how one can use CMake and git to build a modular library based
embedded system.

## Techniques Used
### CMake Interface Libraries
It is an extremely common technique for C libraries (especially in the embedded world) to allow
system customization through weak symbols and compile time macros. The standard practice is to *copy*
the library source into your project, and then provide a configuration header in the include path that
has all the proper macros defined.

CMake object libraries can be used if you only need to provide implementations for functions. But
to use code that relies on configuration headers you must:
 1.  make the library code part of your project. This is not modular or scalable.
 2.  make library a subfolder/subproject and have the headers in some sort of global include path. 
 Better but you are still managing the code, and global include paths are a modern CMake anti-pattern.
 3.  Compile-install as a cmake package the library with a way to link against the correct version for
 every permutation of the different options. This is unrealistic, there would be hundreds of versions per library.
 4.  Use interface libraries.
 
CMake Interface libraries are normally used for header only libraries, but they [can also contain
sources](https://cmake.org/cmake/help/v3.17/prop_tgt/INTERFACE_SOURCES.html#prop_tgt:INTERFACE_SOURCES)
(c, cpp, asm) files. Those files will be compiled as part of whatever target links against
the interface library. This effectively recreates technique 1 (all sources are part of your project) from the 
compilers point of view, but opens up the possibility of modularity from the project management point of view.

Assume a HAL is defined in the following way (simplified CMake file):
```cmake
if(DEFINED cmsis_MCU)
    message(STATUS "STM32L4::HAL : Configuring for MCU == ${cmsis_MCU}")
else()
    message(FATAL_ERROR "STM32L4::HAL : cmsis_MCU is not defined!")
endif()

add_library(STM32L4xx_HAL INTERFACE)
add_library(STM32L4::HAL ALIAS STM32L4xx_HAL)
target_include_directories(STM32L4xx_HAL INTERFACE Include)
target_compile_definitions(STM32L4xx_HAL INTERFACE ${cmsis_MCU})
target_sources(STM32L4xx_HAL INTERFACE ${SOURCES_DEFINED_ELSEWHERE})
```
This HAL needs to know the processor type, so we provide a nice warning if it isn't defined. A project can use
this HAL in a CMake file similar to the followign (assume the HAL is in a subdirectory `hal`)
```cmake
include_subdirectory(hal)
add_executable(myProj main.cpp)
target_include_directories(myProj PUBLIC /configHeaders)
target_link_libraries(myProj STM32L4::HAL)
```
All the HAL inlude paths are added to `myProj` and all sources are added to `myProj` source list for compilation.

### Turning Interface Libraries into Static Libraries
Obviously having *ALL* source files be part of the root project can lead to long compile times. If there is a
logical place to combine interface libraries into a static library it can be easily done. This usually happens at
the point where all the config options are known (such as a Board Support Package, etc).
```cmake
add_library(board_BSP STATIC)
add_library(BSP::Board ALIAS board_BSP)
target_compile_features(board_BSP PUBLIC c_std_11)
target_include_directories(board_BSP PUBLIC configHeaders)
target_sources(board_BSP PRIVATE ${SOURCES_IF_NEEDED})  # totally optional!
target_link_libraries(board_BSP PRIVATE STM32L4::HAL)
```
and now a consuming project can link against it just like show before, but CMake will build a static library.
you could even archive that BSP static library on a binary repository somewhere or use a package manager.

### Dealing with weak symbols in a static library
**NOTE: if your library contains weak symbols you MUST use the `whole archive` linker setting**
If your library contains weak symbols (very common in a BSP, most ISRs are weak symbols!) you must
use the `whole archive` linker setting, at least on gcc. You can leave this to the consumer of your archive
but one level of indirection can make everything a little smoother.

```cmake
add_library(board_BSP STATIC)
# No alias library here, user should link against BSP::Board which is actually
# an alias to the interface library below
target_compile_features(board_BSP PUBLIC c_std_11)
target_include_directories(board_BSP PUBLIC configHeaders)
target_sources(board_BSP PRIVATE ${SOURCES_IF_NEEDED})  # totally optional!
target_link_libraries(board_BSP PRIVATE STM32L4::HAL)

add_library(BSP_WHOLE_ARCHIVE INTERFACE)
add_library(BSP::Board ALIAS BSP_WHOLE_ARCHIVE)
target_link_libraries(BSP_WHOLE_ARCHIVE INTERFACE
    "-Wl,--whole-archive" SensorTile_BSP "-Wl,--no-whole-archive"
    )
```

## Management Techniques
Now that each module can be consumed as a cmake subproject you can manage dependencies in any of the following
ways, or even a combination:
 -  Copy the source into your project as before, but use CMake now with `include_subdirectory()`
 -  Put the dependencies into separate repositories and use git submodules in conjunction with `include_subdirectory()`
 -  Use [CPM](https://github.com/TheLartians/CPM.cmake) for proper semantic versioning and getting the source out of your project.
 -  simlinks?? who knows, your imagination is the limit :)
