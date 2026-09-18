cmake_minimum_required(VERSION 3.16)
project(rpsc_engine VERSION 0.25.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(RPSC_SOURCES
  src/types.cpp
  src/orientation.cpp
  src/move.cpp
  src/position.cpp
  src/movegen.cpp
  src/evaluate.cpp
  src/tt.cpp
  src/search.cpp
  src/notation.cpp
  src/perft.cpp
  src/engine.cpp
  src/protocol.cpp
)

add_library(rpsc_core ${RPSC_SOURCES})
target_include_directories(rpsc_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
if(MSVC)
  target_compile_options(rpsc_core PRIVATE /W4 /permissive-)
else()
  target_compile_options(rpsc_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(rpsc-engine src/main.cpp)
target_link_libraries(rpsc-engine PRIVATE rpsc_core)

enable_testing()
add_executable(engine_tests tests/engine_tests.cpp)
target_link_libraries(engine_tests PRIVATE rpsc_core)
add_test(NAME engine_tests COMMAND engine_tests)

add_executable(search_regressions tests/search_regressions.cpp)
target_link_libraries(search_regressions PRIVATE rpsc_core)
add_test(NAME search_regressions COMMAND search_regressions)

find_program(BASH_EXECUTABLE bash)
if(BASH_EXECUTABLE)
  add_test(NAME protocol_school_role_boundary
           COMMAND ${BASH_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/protocol_semantics.sh $<TARGET_FILE:rpsc-engine>)
endif()

find_program(NODE_EXECUTABLE node)
if(NODE_EXECUTABLE)
  add_test(NAME browser_contract_tests
           COMMAND ${NODE_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/browser_contract_tests.cjs)
endif()

find_package(Python3 COMPONENTS Interpreter QUIET)
if(Python3_Interpreter_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../Games.tex")
  add_test(NAME handbook_record_contract
           COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/handbook_records.py)
endif()
