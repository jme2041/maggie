# CMakeLists.txt
# maggie: Maggie's Demonstration Library
# Copyright (C) 2026 Jeffrey M. Engelmann

cmake_minimum_required(VERSION 3.31)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(maggie LANGUAGES C CXX VERSION 0.1.0.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Require Windows
if(NOT WIN32)
    message(FATAL_ERROR "Only Windows builds are supported")
endif()

# Find dependencies
find_package(fmt CONFIG REQUIRED)
find_package(wil CONFIG REQUIRED)

# Build the library
add_library(maggie "src/maggie.cpp" "src/maggie.rc")

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(maggie PRIVATE /W4 /WX)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(maggie PRIVATE -Wall -Wextra -Werror
        -Wno-c++98-compat
        -Wno-extra-semi)
endif()

target_compile_definitions(maggie PRIVATE MAGGIE_VERSION=\"${PROJECT_VERSION}\")
target_compile_definitions(maggie PRIVATE MAGGIE_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
target_compile_definitions(maggie PRIVATE MAGGIE_VERSION_MINOR=${PROJECT_VERSION_MINOR})
target_compile_definitions(maggie PRIVATE MAGGIE_VERSION_PATCH=${PROJECT_VERSION_PATCH})
target_compile_definitions(maggie PRIVATE MAGGIE_VERSION_TWEAK=${PROJECT_VERSION_TWEAK})

if(BUILD_SHARED_LIBS)
    target_compile_definitions(maggie PUBLIC MAGGIE_SHARED)
    target_compile_definitions(maggie PRIVATE MAGGIE_EXPORTS)
endif()

target_link_libraries(maggie PRIVATE fmt::fmt WIL::WIL)

# Define a file set for the public header
target_sources(maggie
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "include"
    FILES "include/maggie/maggie.h")

# Installation commands
install(
    TARGETS maggie
    EXPORT maggieTargets
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/maggie")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/maggieConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/maggieConfig.cmake"
    INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}")

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/maggieConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/maggieConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/maggieConfigVersion.cmake"
    DESTINATION "${CONFIG_INSTALL_DIR}")

install(
    EXPORT maggieTargets
    NAMESPACE maggie::
    DESTINATION "${CONFIG_INSTALL_DIR}")

# Build tests?
if(BUILD_TESTING)
    include(CTest)
    enable_testing()
    add_subdirectory("test")
endif()

###############################################################################
