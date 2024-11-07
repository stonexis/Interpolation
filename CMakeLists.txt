cmake_minimum_required(VERSION 3.10)

project(MyCppProject)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
add_executable(main main.cpp)

target_include_directories(main PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(main PRIVATE ${Python3_LIBRARIES})

set(PLOTTER_SCRIPT "${CMAKE_SOURCE_DIR}/plotter.py")

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${PLOTTER_SCRIPT} $<TARGET_FILE_DIR:main>
)

