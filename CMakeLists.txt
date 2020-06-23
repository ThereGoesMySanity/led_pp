cmake_minimum_required(VERSION 3.0.0)
project(led_pp VERSION 0.1.0)

add_executable(led_pp main.cpp api.cpp connection.cpp display.cpp mode.cpp settings.cpp)

target_include_directories(led_pp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(led_pp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rpi-rgb-led-matrix/include)

target_link_libraries(led_pp curl)
target_link_libraries(led_pp ${CMAKE_CURRENT_SOURCE_DIR}/rpi-rgb-led-matrix/lib/librgbmatrix.so.1)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
