cmake_minimum_required(VERSION 3.20)
project(SigMaker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IDA_VERSION "9" CACHE STRING "IDA version: 8, 9, 9beta")
set_property(CACHE IDA_VERSION PROPERTY STRINGS 8 9 9beta)
option(IDA_64BIT "Build 64-bit plugin" ON)

set(IDA_SDK_ROOT "${CMAKE_SOURCE_DIR}/SDK" CACHE PATH "Path to IDA SDK root")

set(IDA_SDK_DIR "${IDA_SDK_ROOT}/${IDA_VERSION}/src")
set(IDA_INCLUDE_DIR "${IDA_SDK_DIR}/include")

if (WIN32)
    if (IDA_64BIT)
        set(IDA_LIB_DIR "${IDA_SDK_DIR}/lib/x64_win_vc_64_pro")
        set(PLUGIN_SUFFIX "_64")
    else()
        set(IDA_LIB_DIR "${IDA_SDK_DIR}/lib/x64_win_vc_32_pro")
        set(PLUGIN_SUFFIX "")
    endif()
else()
    if (IDA_64BIT)
        set(IDA_LIB_DIR "${IDA_SDK_DIR}/lib/x64_linux_gcc_64")
        set(PLUGIN_SUFFIX "_64")
    else()
        set(IDA_LIB_DIR "${IDA_SDK_DIR}/lib/x86_linux_gcc_32")
        set(PLUGIN_SUFFIX "")
    endif()
endif()

if (IDA_VERSION STREQUAL "8")
    set(BASE_NAME "SigMaker8")
elseif (IDA_VERSION STREQUAL "9")
    set(BASE_NAME "SigMaker9")
elseif (IDA_VERSION STREQUAL "9beta")
    set(BASE_NAME "SigMaker9beta")
endif()

set(TARGET_NAME "${BASE_NAME}${PLUGIN_SUFFIX}")

add_library(${TARGET_NAME} SHARED
    src/Main.cpp
    src/Plugin.cpp
    src/SignatureUtils.cpp
    src/Utils.cpp
)

target_include_directories(${TARGET_NAME} PRIVATE
    ${IDA_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/submodules/signature/include
)
find_package(TBB REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE TBB::tbb)
target_compile_options(${TARGET_NAME} PRIVATE -mavx2 -mfma -mbmi)

if (WIN32)
    target_compile_definitions(${TARGET_NAME} PRIVATE __NT__)
endif()

if (IDA_64BIT)
    target_compile_definitions(${TARGET_NAME} PRIVATE __EA64__)
endif()

if (IDA_VERSION STREQUAL "9beta")
    target_compile_definitions(${TARGET_NAME} PRIVATE __SDK_BETA__)
endif()

if (MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE
        /O2
        /GL
        /W3
        /MP
    )
else()
    target_compile_options(${TARGET_NAME} PRIVATE
        -O3
        -fPIC
        -Wall
        -Wextra
    )
endif()

target_link_directories(${TARGET_NAME} PRIVATE ${IDA_LIB_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE ida)

if (WIN32 AND MSVC)
    set_target_properties(${TARGET_NAME} PROPERTIES
        LINK_FLAGS "/EXPORT:PLUGIN"
    )
endif()

set_target_properties(${TARGET_NAME} PROPERTIES
    PREFIX ""
)

set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
