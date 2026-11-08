cmake_minimum_required(VERSION 3.24)

project(RED4api
    VERSION 1.0.0
    DESCRIPTION "C++ SDK for Cyberpunk 2077 mod development"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



include(FetchContent)
include(GNUInstallDirs)

option(RED4API_BUILD_CLI "Build RED4api CLI" ON)
option(RED4API_BUILD_TESTS "Build tests" ON)

# RED4ext.SDK is consumed as headers by the DLL support library.
set(RED4EXT_HEADER_ONLY ON CACHE BOOL "Use RED4ext.SDK as headers" FORCE)
FetchContent_Declare(
    RED4extSDK
    GIT_REPOSITORY https://github.com/WopsS/RED4ext.SDK.git
    GIT_TAG 5246e659d783e5834cc08a90e56c8932c9ed3449
)
FetchContent_MakeAvailable(RED4extSDK)

# Core owns mod data and does not depend on code generation.
add_library(RED4apiCore STATIC
    src/Core/Mod.cpp
)
add_library(RED4api::Core ALIAS RED4apiCore)
target_include_directories(RED4apiCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_definitions(RED4apiCore PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)

# Static support library for RED4ext-facing code. A consuming plugin can link it.
add_library(RED4apiDLL STATIC
    src/DLL/Dll.cpp
)
add_library(RED4api::DLL ALIAS RED4apiDLL)
target_link_libraries(RED4apiDLL
    PUBLIC
        RED4api::Core
        RED4ext::SDK
)
target_include_directories(RED4apiDLL
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# The Transpiler uses Core's public types, but Core does not call back into it.
add_library(RED4apiTranspiler STATIC
    src/Transpiler/Transpiler.cpp
    src/Transpiler/Events/InputEvents.cpp
    src/Transpiler/AST.cpp
)
add_library(RED4api::Transpiler ALIAS RED4apiTranspiler)
target_link_libraries(RED4apiTranspiler PUBLIC RED4api::Core)
target_include_directories(RED4apiTranspiler
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(RED4API_BUILD_CLI)
    add_executable(RED4apiCLI tools/CLI/main.cpp)
    set_target_properties(RED4apiCLI PROPERTIES OUTPUT_NAME red4api)
    target_link_libraries(RED4apiCLI PRIVATE RED4api::Core RED4api::Transpiler)
endif()



if(RED4API_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()