cmake_minimum_required(VERSION 3.8)
project(lexAnalyzer)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SOURCES_FILES
  src/list.c
  src/lexAnalysis.c
  src/regex.c
  src/tools.c)
set(HEADERS include)
include_directories(${HEADERS})
add_executable(analyzer ${SOURCES_FILES})

project(test)
link_libraries(check)
link_libraries(m)
link_libraries(rt)
link_libraries(pthread)
link_libraries(subunit)
add_executable(test tests/regex_testcases.c ${SOURCES_FILES})
