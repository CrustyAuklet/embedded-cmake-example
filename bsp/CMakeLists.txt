cmake_minimum_required(VERSION 3.16)
project(BSP_SensorTile
        LANGUAGES C CXX ASM)

set(cmsis_MCU "STM32L476xx")
# Set the Highspeed external clock value (HSE) in MHz
set(cmsis_HSE_VAL 8000000)

include(cmake/CPM.cmake)

CPMAddPackage(
        NAME CMSIS5
        GITHUB_REPOSITORY CrustyAuklet/cmsis5-device-m
        GIT_TAG 5.7.0
)

CPMAddPackage(
        NAME stm32l4_device
        GITHUB_REPOSITORY CrustyAuklet/cmsis_device_l4
        GIT_TAG v1.6.1
)

set(STM32l4xx_USE_HAL_DRIVER ON)
CPMAddPackage(
        NAME stm32l4_hal
        GITHUB_REPOSITORY CrustyAuklet/stm32l4xx_hal_driver
        GIT_TAG v1.11.1
)

CPMAddPackage(
        NAME stm32l4_usb_device
        GITHUB_REPOSITORY CrustyAuklet/STM32-usb-device
        GIT_TAG v2.6.0
)

CPMAddPackage(
        NAME FatFS
        GITHUB_REPOSITORY CrustyAuklet/fatfs-cmake
        GIT_TAG v2.1.4
)

set(freertos_PORT_CPU "ARM_CM4F")
set(freertos_HEAP_TYPE 4)
CPMAddPackage(
        NAME FreeRTOS
        GITHUB_REPOSITORY CrustyAuklet/FreeRTOS-kernel-cmake
        GIT_TAG v10.3.1
)
add_subdirectory(Components)

# set as cache variable so that consuming projects can see it. Normal variables only propagate "down"
set(bsp_DEFAULT_LINKER_FILE ${CMAKE_CURRENT_LIST_DIR}/linker/STM32L476RGTx_FLASH.sensortile.ld CACHE INTERNAL "Default Linker Script" FORCE)

add_library(SENSORTILE_SENSORTILE INTERFACE)
# No alias library here, please link against BSP::SensorTile unless you really know what you are doing
target_include_directories(SENSORTILE_SENSORTILE INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/SensorTile
        ${CMAKE_CURRENT_LIST_DIR}/Components/Common
        )

target_sources(SENSORTILE_SENSORTILE INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/SensorTile/SensorTile_bus.c
        ${CMAKE_CURRENT_LIST_DIR}/SensorTile/SensorTile_env_sensors_ex.c
        ${CMAKE_CURRENT_LIST_DIR}/SensorTile/SensorTile_env_sensors.c
        ${CMAKE_CURRENT_LIST_DIR}/SensorTile/SensorTile_motion_sensors_ex.c
        ${CMAKE_CURRENT_LIST_DIR}/SensorTile/SensorTile_motion_sensors.c
        ${CMAKE_CURRENT_LIST_DIR}/SensorTile/SensorTile_sd.c
        ${CMAKE_CURRENT_LIST_DIR}/SensorTile/SensorTile.c
#        ${CMAKE_CURRENT_LIST_DIR}/SensorTile/SensorTile_audio.c
#        ${CMAKE_CURRENT_LIST_DIR}/SensorTile/SensorTile_gg.c
        )

add_library(SensorTile_BSP STATIC)
# No alias library here, we use the name for a trick later on that makes consumption better
# Please link against BSP::SensorTile unless you really know what you are doing
target_compile_features(SensorTile_BSP PUBLIC c_std_11)
target_include_directories(SensorTile_BSP PUBLIC config)
target_sources(SensorTile_BSP PRIVATE
        config/cube_hal_l4.c
        config/sd_diskio.c
        config/stm32l4xx_hal_msp.c
        config/stm32l4xx_it.c
        config/usbd_cdc_interface.c
        config/usbd_conf.c
        config/usbd_desc.c
        )

target_link_libraries(SensorTile_BSP PRIVATE
        CMSIS::CORE
        STM32L4::DEVICE
        STM32L4::HAL
        SENSORTILE_SENSORTILE
        STM32_USB::DEVICE
        STM32_USB::CDC
        COMPONENT::hts221
        COMPONENT::lps22hb
        COMPONENT::lsm303agr
        COMPONENT::lsm6dsm
#        COMPONENT::pcm1774
#        COMPONENT::stc3115
        FreeRTOS::Kernel
        FreeRTOS::CMSIS
        FatFS::FatFS
        )

message(STATUS " *** SensorTile BSP Static Library ***")
get_target_property(OUT SensorTile_BSP LINK_LIBRARIES)
set(INCLUDE_LIST "")
set(DEFINITION_LIST "")
foreach(DEP ${OUT})
    get_target_property(TGT_INCLUDES ${DEP} INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(TGT_DEFS ${DEP} INTERFACE_COMPILE_DEFINITIONS)
    list(APPEND INCLUDE_LIST ${TGT_INCLUDES})
    message(STATUS "  BSP using " ${DEP} " with includes " ${TGT_INCLUDES})
    message(STATUS "    Includes: " ${TGT_INCLUDES})
    if(TGT_DEFS)
        list(APPEND DEFINITION_LIST ${TGT_DEFS})
        message(STATUS "    Definitions: " ${TGT_DEFS})
    endif()
endforeach()
message(STATUS " * FINAL INCLUDE LIST: " "${INCLUDE_LIST}")
message(STATUS " * FINAL DEFINE LIST: " "${DEFINITION_LIST}")
target_include_directories(SensorTile_BSP PUBLIC ${INCLUDE_LIST})
target_compile_definitions(SensorTile_BSP PUBLIC ${DEFINITION_LIST})

## This is a trick so that the consuming binary doesn't need to manually invoke 'whole-archive'
## `whole-archive` link option ensures that weak symbols are imported from a static lib, and work as expected
## since this is a BSP weak symbols are kind of important :)
add_library(SensorTile_BSP_WHOLE_ARCHIVE INTERFACE)
add_library(BSP::SensorTile ALIAS SensorTile_BSP_WHOLE_ARCHIVE)
target_link_libraries(SensorTile_BSP_WHOLE_ARCHIVE INTERFACE
    "-Wl,--whole-archive" SensorTile_BSP "-Wl,--no-whole-archive"
    )
