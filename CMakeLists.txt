CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

project("parser-lab2-sample-programming-language" VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_NAME spl_parser)
configure_file(config.hpp.in config.hpp)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# determine flex flags for windows and unix
if (WIN32)
	set(FLEX_FLAGS "-DYY_NO_UNISTD_H -DWIN_IO_H")
else()
	set(FLEX_FLAGS "")
endif()

BISON_TARGET(spl_parser
	spl_parser.y
	${CMAKE_CURRENT_BINARY_DIR}/spl_parser.cpp
	)
FLEX_TARGET(spl_lexer
	spl_lexer.l
	${CMAKE_CURRENT_BINARY_DIR}/spl_lexer.cpp
	COMPILE_FLAGS ${FLEX_FLAGS}
)
ADD_FLEX_BISON_DEPENDENCY(spl_lexer spl_parser)

add_library(tree OBJECT tree.cpp)
add_library(parser_lexer OBJECT ${BISON_spl_parser_OUTPUTS} ${FLEX_spl_lexer_OUTPUTS})
add_executable(${EXECUTABLE_NAME} main.cpp)
target_link_libraries(${EXECUTABLE_NAME} tree parser_lexer)
target_include_directories(${EXECUTABLE_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}
		${FLEX_INCLUDE_DIRS}
)
target_include_directories(parser_lexer
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}
		${FLEX_INCLUDE_DIRS}
)

enable_testing()

add_test(NAME Usage COMMAND ${EXECUTABLE_NAME} -h)
set_tests_properties(Usage
	PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*spl_parser.*\[options\].*INPUT_FILE"
)

add_test(NAME Division COMMAND ${EXECUTABLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/samples/division.spl)
set_tests_properties(
	Division
	PROPERTIES
	PASS_REGULAR_EXPRESSION "PROGRAM[(]COMP_STMT[(]COMP_STMT[(]ASSING_STMT[(]ID[(]q[)],CONST[(]0[)][)],ASSING_STMT[(]ID[(]r[)],ID[(]m[)][)][)],WHILE_STMT[(]B_EXPR[(]ID[(]r[)],>=,ID[(]n[)][)],COMP_STMT[(]ASSING_STMT[(]ID[(]q[)],AR_EXPR[(]ID[(]q[)],[+],CONST[(]1[)][)][)],ASSING_STMT[(]ID[(]r[)],AR_EXPR[(]ID[(]r[)],-,ID[(]n[)][)][)][)][)][)][)]"
	FAIL_REGULAR_EXPRESSION "syntax error"
)

add_test(NAME Comments COMMAND ${EXECUTABLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/samples/comments.spl)
set_tests_properties(
	Comments
	PROPERTIES
	PASS_REGULAR_EXPRESSION "PROGRAM[(]COMP_STMT[(]COMP_STMT[(]ASSING_STMT[(]ID[(]q[)],CONST[(]0[)][)],ASSING_STMT[(]ID[(]r[)],ID[(]m[)][)][)],WHILE_STMT[(]B_EXPR[(]ID[(]r[)],>=,ID[(]n[)][)],COMP_STMT[(]ASSING_STMT[(]ID[(]q[)],AR_EXPR[(]ID[(]q[)],[+],CONST[(]1[)][)][)],ASSING_STMT[(]ID[(]r[)],AR_EXPR[(]ID[(]r[)],-,ID[(]n[)][)][)][)][)][)][)]"
	FAIL_REGULAR_EXPRESSION "syntax error"
)

add_test(NAME Wrong_Assigment COMMAND ${EXECUTABLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/samples/wrong_assign.spl)
set_tests_properties(
	Wrong_Assigment
	PROPERTIES
	PASS_REGULAR_EXPRESSION "syntax error"
	FAIL_REGULAR_EXPRESSION "PROGRAM"
)

add_test(NAME Invalid_Input COMMAND ${EXECUTABLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/samples/faulty.spl)
set_tests_properties(
	Invalid_Input
	PROPERTIES
	PASS_REGULAR_EXPRESSION "syntax error"
	FAIL_REGULAR_EXPRESSION "PROGRAM"
)
