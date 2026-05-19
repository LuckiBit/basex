cmake_minimum_required(VERSION 3.10)

project(basex 
        VERSION 1.1.0
        LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED OFF)

# -------- Configurable variables --------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# -------- Configure version.h from template --------
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/version.h
)

# -------- Add include path --------
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

# -------- Manually list all source files --------
set(SOURCE_FILES
    main.c
    platform.c
    color.c
    font_ansi_shadow.c
    log.c
)

# -------- Build executable --------
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# -------- Link math library on non-Windows platforms --------
if(NOT WIN32)
    target_link_libraries(${PROJECT_NAME} m)
endif()
