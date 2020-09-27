cmake_minimum_required(VERSION 3.12)
project(recap_server VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS -D_WIN32_WINNT=0x0601 -D_CRT_SECURE_NO_WARNINGS -DWIN32_LEAN_AND_MEAN -DNOMINMAX -DBOOST_NETWORK_ENABLE_HTTPS -DRAPIDJSON_HAS_STDSTRING)
add_executable(recap_server source/main.cpp)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

# Boost
set(Boost_INCLUDE_DIR libs/boost_1_70_0)
set(Boost_LIBRARY_DIR libs/boost_1_70_0/stage/lib)
find_package(Boost 1.70.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
target_link_libraries(recap_server ${Boost_LIBRARIES} )

# pugixml
add_subdirectory(libs/pugixml-1.10)
target_link_libraries(recap_server pugixml)

# RapidJSON
set(RapidJSON_INCLUDE_DIRS libs/rapidjson-1.1.0/include)
add_subdirectory(libs/rapidjson-1.1.0)
target_link_libraries(recap_server ${RapidJSON_LIBRARIES})
include_directories(${RapidJSON_INCLUDE_DIRS})

# OpenSSL
set(OpenSSL_INCLUDE_DIR libs/openssl-1.1.1b/include)
set(OpenSSL_LIBRARY_DIR libs/openssl-1.1.1b/lib)
find_package(OpenSSL)
include_directories(${OpenSSL_INCLUDE_DIR})
link_directories(${OpenSSL_LIBRARY_DIR})
target_link_libraries(recap_server ${OpenSSL_LIBRARIES} )
