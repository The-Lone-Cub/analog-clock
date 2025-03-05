cmake_minimum_required(VERSION 3.10)

# Set project name
project(clock)

# Define build directory
set(BUILD_DIR "${CMAKE_SOURCE_DIR}/build")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 package
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Collect all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/include"    # Add this line
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)

# Add custom target to run the executable
add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Add custom target for repository cleanup
add_custom_target(clean_build
    COMMAND ${CMAKE_COMMAND} -E echo "Cleaning build files..."
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${BUILD_DIR}/*"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Set compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -pedantic
    )
endif()