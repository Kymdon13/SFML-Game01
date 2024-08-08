cmake_minimum_required(VERSION 3.17)
project(SFMLTest) # name the project

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(SFMLTest ${SOURCES})

include_directories(headers "D:\\Code\\Project\\Cpp\\nibbles-main\\nibbles-Homework\\SFML-2.6.1\\include")

set(SFML_ROOT "D:\\Code\\Project\\Cpp\\SFMLTest\\SFML-2.6.1")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio main) # find the packages you need

# Link if all components are found
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(SFMLTest ${SFML_LIBRARIES})
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Copy dlls to build directory
if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB MINGW_DEP_DLLS "D:/Program/msys64/mingw32/bin/*.dll")
    file(COPY ${MINGW_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()
