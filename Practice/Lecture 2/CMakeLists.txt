cmake_minimum_required(VERSION 3.28)
project(Lecture_2)

set(CMAKE_CXX_STANDARD 26)

#add_executable(Lecture_2 "Algoritma DDA.cpp")
#add_executable(Lecture_2 "Algoritma Bresenham.cpp")
add_executable(Lecture_2 "Post Test- Algoritma DDA.cpp")

target_link_libraries(Lecture_2 ${OPENGL_LIBRARIES} glu32)
target_link_libraries(Lecture_2 ${GULT_LIBRARIES} glu32)
target_link_libraries(Lecture_2 -lopengl32 -g -lfreeglut -lglu32)