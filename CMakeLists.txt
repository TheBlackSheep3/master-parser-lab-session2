CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

project("parser-lab2-sample-programming-language" VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_NAME spl_parser)
configure_file(config.hpp.in config.hpp)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(spl_parser
	spl_parser.y
	${CMAKE_CURRENT_BINARY_DIR}/spl_parser.cpp)
FLEX_TARGET(spl_lexer
	spl_lexer.l
	${CMAKE_CURRENT_BINARY_DIR}/spl_lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(spl_lexer spl_parser)

add_executable(${EXECUTABLE_NAME} main.cpp)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
