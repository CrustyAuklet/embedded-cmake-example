cmake_minimum_required(VERSION 3.16)

if(NOT CMAKE_TOOLCHAIN_FILE)
    message(WARNING "[WARN] CMAKE_TOOLCHAIN_FILE not specified: Using defaults")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

    # specify cross compilers and tools
    set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
    set(CMAKE_C_COMPILER    arm-none-eabi-gcc)
    set(CMAKE_CXX_COMPILER  arm-none-eabi-g++)
    set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
    set(CMAKE_AR            arm-none-eabi-ar)
    set(CMAKE_OBJCOPY       arm-none-eabi-objcopy)
    set(CMAKE_OBJDUMP       arm-none-eabi-objdump)
    set(SIZE                arm-none-eabi-size)

    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

    set(MCPU_FLAGS "-mthumb -mcpu=cortex-m4")
    SET(FPU_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
    #add_definitions(-DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -D__FPU_PRESENT=1)
    #SET(FPU_FLAGS "-mfloat-abi=soft")

    set(CMAKE_C_FLAGS_INIT   "${MCPU_FLAGS} ${FPU_FLAGS} -g3 -ffunction-sections -fdata-sections -fstack-usage")
    set(CMAKE_CXX_FLAGS_INIT "${MCPU_FLAGS} ${FPU_FLAGS} -g3 -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fstack-usage")
    set(CMAKE_ASM_FLAGS_INIT "${MCPU_FLAGS} ${FPU_FLAGS} -x assembler-with-cpp")
    set(CMAKE_EXE_LINKER_FLAGS_INIT "${MCPU_FLAGS} ${FPU_FLAGS} -static -Wl,--gc-sections")
endif()

project(SensorTile_Example
        LANGUAGES C CXX ASM
        )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(bsp)

add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17 c_std_11)
target_link_libraries(${PROJECT_NAME} PUBLIC BSP::SensorTile)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")

target_include_directories(${PROJECT_NAME} PUBLIC Src)
target_sources(${PROJECT_NAME} PUBLIC
        Src/datalog_application.c
        Src/main.c
        )

# STM32 IDE linked in math and cstdlib explicitly?
# -Wl,--start-group -lc -lm -Wl,--end-group
target_link_options(${PROJECT_NAME} PUBLIC
        --specs=nano.specs          # link against newlib-nano
        --specs=nosys.specs         # enable retargeting
        -Wl,--print-memory-usage    # Print memory breakdown when linking
        -Wl,--cref                  # add cross reference info to map file
        -u _printf_float            # enable %f in printf
        -T ${bsp_DEFAULT_LINKER_FILE} # Use default linker file from BSP
        -Wl,-Map=${PROJECT_NAME}.map  # enable map file output
        )

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
