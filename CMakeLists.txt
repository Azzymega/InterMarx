cmake_minimum_required(VERSION 3.28)
project(InterMarx C ASM)

set(CMAKE_C_STANDARD 99)
include_directories(include)

add_compile_options(-fshort-wchar)

add_subdirectory(pal)
add_subdirectory(mt)
add_subdirectory(rtl)
add_subdirectory(ob)
add_subdirectory(far)
add_subdirectory(ex)
add_subdirectory(ldr)
add_subdirectory(hp)

add_executable(InterMarx main.c)

target_link_libraries(InterMarx PUBLIC Ldr)
target_link_libraries(InterMarx PUBLIC Hp)
target_link_libraries(InterMarx PUBLIC Rtl)
target_link_libraries(InterMarx PUBLIC Mt)
target_link_libraries(InterMarx PUBLIC Ob)
target_link_libraries(InterMarx PUBLIC Ex)
target_link_libraries(InterMarx PUBLIC Far)
target_link_libraries(InterMarx PUBLIC Pal)
