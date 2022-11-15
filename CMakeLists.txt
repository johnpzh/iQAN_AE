cmake_minimum_required(VERSION 3.9)
project(PANNS LANGUAGES CXX)

# Verbose Makefile
set(CMAKE_VERBOSE_MAKEFILE ON)

# C++14
set(CMAKE_CXX_STANDARD 14)

# Boost
find_package(Boost REQUIRED)

# OpenMP
find_package(OpenMP REQUIRED)

# Debug Options
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(CXX_OPTION ${CXX_OPTION} -O0)
endif()
if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(CXX_OPTION ${CXX_OPTION} -Wall -Wextra -pedantic -Werror)
endif()

set(CXX_OPTION ${CXX_OPTION} -march=native)
IF(DEFINED DEBUG_PRINT)
        string(TOLOWER ${DEBUG_PRINT} DEBUG_PRINT_lower)
        IF(NOT ${DEBUG_PRINT_lower} MATCHES "off")
            add_compile_definitions(DEBUG_PRINT)
        ENDIF()
ENDIF()

# NSG as a Library
add_library(NSG
        include/efanna2e/index_nsg.cpp
        include/efanna2e/index.cpp)
target_compile_options(NSG PRIVATE ${CXX_OPTION} -march=native)
target_link_libraries(NSG PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

# Bin
add_executable(
        PSS_v5_find_L_index_adaptive_merge_L_master_LG
        app/PSS_v5_find_L_index_adaptive_merge_L_master_LG.cpp
        core/Searching.202111152031.PSS_v5.index_adaptive_merge_L_master.LG.cpp
        include/utils.cpp)
target_compile_options(
        PSS_v5_find_L_index_adaptive_merge_L_master_LG
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_find_L_index_adaptive_merge_L_master_LG
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_find_L_index_adaptive_merge_L_master
        app/PSS_v5_find_L_index_adaptive_merge_L_master.cpp
        core/Searching.202111151933.PSS_v5.index_adaptive_merge_L_master.cpp
        include/utils.cpp)
target_compile_options(
        PSS_v5_find_L_index_adaptive_merge_L_master
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_find_L_index_adaptive_merge_L_master
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_distance_threshold_profiling
        app/PSS_v5_distance_threshold_profiling.cpp
        core/Searching.202102022027.PSS_v5.dist_thresh.profiling.cpp)
target_compile_options(
        PSS_v5_distance_threshold_profiling
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_distance_threshold_profiling
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        PSS_v5_LG_distance_threshold_profiling
        app/PSS_v5_LG_distance_threshold_profiling.cpp
        core/Searching.202102031939.PSS_v5.large_graph.dist_thresh.profiling.cpp)
target_compile_options(
        PSS_v5_LG_distance_threshold_profiling
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        PSS_v5_LG_distance_threshold_profiling
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)

add_executable(
        profile_find_L_seq_single_query_simple_search
        app/profile_find_L_seq_single_query_simple_search.cpp)
target_compile_options(
        profile_find_L_seq_single_query_simple_search
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_find_L_seq_single_query_simple_search
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX NSG)

add_executable(
        profile_find_L_seq_single_query_simple_search_large_graph
        app/profile_find_L_seq_single_query_simple_search_large_graph.cpp)
target_compile_options(
        profile_find_L_seq_single_query_simple_search_large_graph
        PRIVATE ${CXX_OPTION})
target_link_libraries(
        profile_find_L_seq_single_query_simple_search_large_graph
        PRIVATE ${Boost_LIBRARIES} OpenMP::OpenMP_CXX NSG)

add_executable(
        hnsw_search
        app/hnsw_search.cpp)
target_link_libraries(
        hnsw_search
        PRIVATE OpenMP::OpenMP_CXX)