set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 2.8)
project( detectLetters )
find_package( OpenCV REQUIRED )
add_executable( detectLetters detectLetters.cpp )
target_link_libraries( detectLetters ${OpenCV_LIBS} stdc++fs)

