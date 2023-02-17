cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(LINUX UNIX AND NOT APPLE)
set(CMAKE_DEBUG_POSTFIX d)

project(Nucleus VERSION 0.1 LANGUAGES C CXX)

add_compile_definitions(NUCLEUS_VERSION="${CMAKE_PROJECT_VERSION}")

file(GLOB_RECURSE NUCLEUS_HEADERS ${CMAKE_CURRENT_LIST_DIR}/Include/Nucleus/*.h*)
file(GLOB_RECURSE NUCLEUS_INLINE ${CMAKE_CURRENT_LIST_DIR}/Include/Nucleus/*.inl)
file(GLOB_RECURSE NUCLEUS_SOURCES ${CMAKE_CURRENT_LIST_DIR}/Source/*.c*)
file(GLOB_RECURSE NUCLEUS_TESTS ${CMAKE_CURRENT_LIST_DIR}/Tests/*.c*)

function(filterSources LIST)
    foreach(FILE ${LIST})
        get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
        string(REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" GROUP "${PARENT_DIR}")
        string(REPLACE "/" "\\" GROUP "${GROUP}")
        source_group("${GROUP}" FILES "${FILE}")
    endforeach()
endfunction()

filterSources("${NUCLEUS_SOURCES}")
filterSources("${NUCLEUS_HEADERS}")
filterSources("${NUCLEUS_TESTS}")
filterSources("${NUCLEUS_INLINE}")

set(NUCLEUS_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/Include/" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_library(Nucleus SHARED ${NUCLEUS_SOURCES} ${NUCLEUS_HEADERS} ${NUCLEUS_INLINE})
target_include_directories(Nucleus PUBLIC ${NUCLEUS_INCLUDE})
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Nucleus)

add_subdirectory(Tests)
set_target_properties(Nucleus PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/Build/)
