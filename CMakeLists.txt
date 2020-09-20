cmake_minimum_required(VERSION 3.0)
project(recap_server VERSION 1.0.0)
add_executable(recap_server source/main.cpp)

# Boost
set(Boost_INCLUDE_DIR /work/libs/boost_1_70_0)
set(Boost_LIBRARY_DIR /work/libs/boost_1_70_0/stage/lib)
find_package(Boost 1.70.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
target_link_libraries(recap_server ${Boost_LIBRARIES} )

# pugixml
add_subdirectory(/work/libs/pugixml-1.10)
target_link_libraries(recap_server pugixml)

# RapidJSON
add_subdirectory(/work/libs/rapidjson-1.1.0)
target_link_libraries(recap_server RapidJSON)
