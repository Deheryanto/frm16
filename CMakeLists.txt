cmake_minimum_required(VERSION 3.16)

project(FRM16 
    VERSION 0.1.0 
    DESCRIPTION "Fast RISC Machine 16-bit Emulator"
    LANGUAGES CXX
)

# --- Compiler Standards Configuration ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Output Directories Configuration ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 1. Collect all source files recursively from the src directory
file(GLOB_RECURSE ALL_SOURCES "src/*.cpp")

# 2. Define the main Virtual Machine (VM) executable target
add_executable(frmrun ${ALL_SOURCES})

# 3. Configure include directories for the VM target
target_include_directories(frmrun PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu
    ${CMAKE_CURRENT_SOURCE_DIR}/src/memory
)

# 4. Find and link SDL2 using pkg-config (robust approach for Linux environments)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

target_include_directories(frmrun PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(frmrun PRIVATE ${SDL2_LIBRARIES})

# 5. Define the Assembler executable target
add_executable(frmas tools/assembler.cpp)
target_include_directories(frmas PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu
)

# 6. Configuration status messages
message(STATUS "==========================================")
message(STATUS " FRM16 Build Configuration")
message(STATUS " Sources Found: ${ALL_SOURCES}")
message(STATUS " SDL2 Libs    : ${SDL2_LIBRARIES}")
message(STATUS " Output Dir   : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "==========================================")