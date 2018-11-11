cmake_minimum_required(VERSION 3.7.2)
project(Mantis-SPIRV-Cross)

set(CMAKE_CXX_STANDARD 14)

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")
set(SOURCES
        "${SRC_DIR}/main.cpp"
        "${SRC_DIR}/cmd_line/CMDParser.h"
        "${SRC_DIR}/cmd_line/CMDParser.cpp"
        "${SRC_DIR}/cmd_line/CMDArgument.h"
        "${SRC_DIR}/cmd_line/CMDArgument.cpp")

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

set(CMAKE_EXE_LINKER_FLAGS "-static")

# spirv-cross
set(SPIRV_CROSS_DIR "${LIB_DIR}/spirv-cross")
set(SPIRV_CROSS_SOURCES
        "${SPIRV_CROSS_DIR}/spirv_glsl.cpp"
        "${SPIRV_CROSS_DIR}/spirv_cross.cpp"
        "${SPIRV_CROSS_DIR}/spirv_cfg.cpp")
add_library("spirv-cross" "${SPIRV_CROSS_DIR}/spirv_glsl.cpp" "${SPIRV_CROSS_SOURCES}")
target_include_directories("spirv-cross" PRIVATE "${SPIRV_CROSS_DIR}")
target_include_directories(${PROJECT_NAME}  PRIVATE "${SPIRV_CROSS_DIR}")
target_link_libraries(${PROJECT_NAME} "spirv-cross")