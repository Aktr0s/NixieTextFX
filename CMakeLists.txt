cmake_minimum_required(VERSION 3.14)
project(NixieTextFX VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Add an option to toggle warnings for unused/uninitialized variables
option(ENABLE_STRICT_WARNINGS "Enable strict warnings for unused/uninitialized variables" ON)

# Configure compiler warnings based on the option
if(ENABLE_STRICT_WARNINGS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wunused
            -Wuninitialized
        )
    elseif (MSVC)
        add_compile_options(
            /W4
            /permissive-
        )
    endif()
endif()

# Find and link packages installed via Homebrew or system-wide
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include directories based on platform
if(APPLE)
    # macOS specific includes
    include_directories(/opt/homebrew/include)  # For macOS M1/ARM
    link_directories(/opt/homebrew/lib)
    set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX15.2.sdk")
elseif(UNIX AND NOT APPLE)
    # Linux specific includes (if needed)
    link_directories(/usr/local/lib)
endif()

# Add executable and source files
add_executable(NixieTextFX
    src/main.cpp
)

# Link libraries
target_link_libraries(NixieTextFX
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
)
