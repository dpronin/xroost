cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(xroost VERSION 0.4.0 LANGUAGES CXX)

option(ENABLE_DEB "Enable 'package' target to build DEB packages from artifacts" OFF)

message("Building with CMake version: ${CMAKE_VERSION}")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
  $<$<CONFIG:Debug>:-ggdb3>
)

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} INTERFACE
    PUBLIC FILE_SET HEADERS BASE_DIRS include FILES
        include/xroost/algo/partition.hpp
        include/xroost/algo/sorting/bubble_sort.hpp
        include/xroost/algo/sorting/heap_sort.hpp
        include/xroost/algo/sorting/insertion_sort.hpp
        include/xroost/algo/sorting/quick_sort.hpp
        include/xroost/algo/sorting/quick_sort_r.hpp
        include/xroost/algo/sorting/selection_sort.hpp
        include/xroost/algo/sorting/sort_all.hpp
        include/xroost/lockless/detail.hpp
        include/xroost/lockless/spmcqueue.hpp
        include/xroost/lockless/spscqueue.hpp
        include/xroost/utility/aligned_storage.hpp
)

target_include_directories(${PROJECT_NAME} INTERFACE include)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
    LIBRARY
    FILE_SET HEADERS
        COMPONENT libxroost_dev
)
