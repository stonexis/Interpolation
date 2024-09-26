cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(MyCppProject)

# Найдите Python
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Добавьте исполняемый файл
add_executable(main main.cpp)

# Добавьте путь к заголовочным файлам Python
target_include_directories(main PRIVATE ${Python3_INCLUDE_DIRS})

# Линкуйте с библиотеками Python
target_link_libraries(main PRIVATE ${Python3_LIBRARIES})

# Указываем исходный файл plotter.py
set(PLOTTER_SCRIPT "${CMAKE_SOURCE_DIR}/plotter.py")

# Добавляем команду копирования в директорию сборки
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${PLOTTER_SCRIPT} $<TARGET_FILE_DIR:main>
)

