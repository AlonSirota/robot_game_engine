cmake_minimum_required(VERSION 3.11)
project(RobotGameEngine VERSION 0.1.0 LANGUAGES C CXX)

SET(libraries_to_include freeglut)

include(CTest)
enable_testing()

find_package(GLUT)
find_package(OpenGL)

add_executable(RobotGameEngine source/main.cpp source/ui/ui.hpp source/ui/ui.cpp
                                source/ui/UIElement.hpp
                                source/ui/TextLable.hpp source/ui/TextLable.cpp
                                source/ui/TextBox.hpp source/ui/TextBox.cpp
                                source/ui/SolidColor.hpp source/ui/SolidColor.cpp
                                source/ui/UIMenue.hpp source/ui/UIMenue.cpp
                                source/ui/MainMenue.hpp source/ui/MainMenue.cpp
                                source/ui/HelpMenue.hpp source/ui/HelpMenue.cpp
                                source/ui/OptionsMenue.hpp source/ui/OptionsMenue.cpp
                                source/ui/UIManager.hpp source/ui/UIManager.cpp
)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

target_link_libraries(RobotGameEngine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)