# CMakeLists.txt (test directory)
# maggie: Maggie's Demonstration Library
# Copyright (C) 2026 Jeffrey M. Engelmann

function(make_test target_name)
    add_executable(${target_name} "${target_name}.cpp")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target_name} PRIVATE /W4 /WX)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${target_name} PRIVATE -Wall -Wextra -Werror)
    endif()
    target_link_libraries(${target_name} PRIVATE maggie)
    add_test(
        NAME ${target_name}
        COMMAND ${target_name}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endfunction()

make_test(test_maggie)

###############################################################################
