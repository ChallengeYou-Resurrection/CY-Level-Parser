cmake_minimum_required(VERSION 3.1)

project(cy_parser)
#Set up the C++ flags
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_LIBRARIES -lstdc++fs)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

set(SOURCES
	src/main.cpp
	src/BinaryFile.cpp
	src/CYObject.cpp
	src/FileParser.cpp
	src/FileWriter.cpp
	src/TextureConverter.cpp
	src/Utilities.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(Threads)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
