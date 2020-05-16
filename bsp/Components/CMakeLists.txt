cmake_minimum_required(VERSION 3.15)

add_library(COMPONENT_HTS221 INTERFACE)
add_library(COMPONENT::hts221 ALIAS COMPONENT_HTS221)
target_include_directories(COMPONENT_HTS221 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/hts221)
target_sources(COMPONENT_HTS221 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/hts221/hts221.c
        ${CMAKE_CURRENT_LIST_DIR}/hts221/hts221_reg.c
        )

add_library(COMPONENT_lps22hb INTERFACE)
add_library(COMPONENT::lps22hb ALIAS COMPONENT_lps22hb)
target_include_directories(COMPONENT_lps22hb INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lps22hb)
target_sources(COMPONENT_lps22hb INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/lps22hb/lps22hb.c
        ${CMAKE_CURRENT_LIST_DIR}/lps22hb/lps22hb_reg.c
        )

add_library(COMPONENT_lsm6dsm INTERFACE)
add_library(COMPONENT::lsm6dsm ALIAS COMPONENT_lsm6dsm)
target_include_directories(COMPONENT_lsm6dsm INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lsm6dsm)
target_sources(COMPONENT_lsm6dsm INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/lsm6dsm/lsm6dsm.c
        ${CMAKE_CURRENT_LIST_DIR}/lsm6dsm/lsm6dsm_reg.c
        )

add_library(COMPONENT_lsm303agr INTERFACE)
add_library(COMPONENT::lsm303agr ALIAS COMPONENT_lsm303agr)
target_include_directories(COMPONENT_lsm303agr INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lsm303agr)
target_sources(COMPONENT_lsm303agr INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/lsm303agr/lsm303agr.c
        ${CMAKE_CURRENT_LIST_DIR}/lsm303agr/lsm303agr_reg.c
        )

add_library(COMPONENT_pcm1774 INTERFACE)
add_library(COMPONENT::pcm1774 ALIAS COMPONENT_pcm1774)
target_include_directories(COMPONENT_pcm1774 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/pcm1774)
target_sources(COMPONENT_pcm1774 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/pcm1774/PCM1774.c
        ${CMAKE_CURRENT_LIST_DIR}/pcm1774/PCM1774_reg.c)

add_library(COMPONENT_stc3115 INTERFACE)
add_library(COMPONENT::stc3115 ALIAS COMPONENT_stc3115)
target_include_directories(COMPONENT_stc3115 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/stc3115)
target_sources(COMPONENT_stc3115 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/stc3115/STC3115_Driver.c
        )
