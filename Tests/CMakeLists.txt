cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(LINUX UNIX AND NOT APPLE)
set(CMAKE_DEBUG_POSTFIX d)

project(NucleusTests VERSION 0.1 LANGUAGES C CXX)
add_executable(NucleusTests ${NUCLEUS_TESTS})
add_dependencies(NucleusTests Nucleus)
filterSources("${NUCLEUS_TESTS}")

target_include_directories(NucleusTests PUBLIC ${NUCLEUS_INCLUDE})
target_link_libraries(NucleusTests Nucleus)
set_target_properties(NucleusTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/Build/)
