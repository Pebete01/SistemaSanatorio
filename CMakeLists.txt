cmake_minimum_required(VERSION 3.20)
project(SistemaSanatorio CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CURSES_NEED_WIDE TRUE)
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
add_executable(SistemaSanatorio
  src/main.cpp
  src/SistemaSanatorio.cpp
  src/Paciente.cpp
  src/Profesional.cpp
  src/Especialidad.cpp
  # ...otros .cpp...
  ui/menu.cpp
)
target_include_directories(SistemaSanatorio PRIVATE ${CURSES_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/ui)
target_link_libraries(SistemaSanatorio PRIVATE ${CURSES_LIBRARIES})
if(NOT CURSES_LIBRARIES)
  target_link_libraries(SistemaSanatorio PRIVATE ncursesw)
endif()
