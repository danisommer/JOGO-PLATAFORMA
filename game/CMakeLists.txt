cmake_minimum_required(VERSION 3.10)

project(JOGO_2D)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add include directories
include_directories(JOGO_2D/Headers)

# Add all source files
file(GLOB_RECURSE SOURCES "JOGO_2D/Sources/*.cpp")

# Debug detected sources
message(STATUS "Detected sources: ${SOURCES}")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Copy assets to build directory after building the project
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/JOGO_2D/Assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/JOGO_2D/Assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/JOGO_2D/Menu $<TARGET_FILE_DIR:${PROJECT_NAME}>/Menu
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/JOGO_2D/Saves $<TARGET_FILE_DIR:${PROJECT_NAME}>/Saves
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/JOGO_2D/Fases $<TARGET_FILE_DIR:${PROJECT_NAME}>/Fases
)

# Find and link SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# Target-specific include directories (optional)
target_include_directories(${PROJECT_NAME} PRIVATE JOGO_2D/Headers)
