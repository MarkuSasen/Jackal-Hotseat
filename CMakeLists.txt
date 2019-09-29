cmake_minimum_required(VERSION 3.12)
project(shakalaka)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")


set(SOURCE Sbasic.cpp units.cpp Field.cpp SHAKAL.cpp player.cpp GUI.cpp)

list(TRANSFORM SOURCE PREPEND "src/")

set(SFML_DIR "D:/Libs/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML REQUIRED COMPONENTS window graphics audio network)

#D:/Libs/SFML-2.5.1/lib/cmake/SFML
#C:/Libs/SFML-2.5.1/lib/cmake/SFML

add_executable(shakalaka main.cpp ${SOURCE})

target_link_libraries(shakalaka sfml-graphics sfml-audio sfml-network)

include_directories(/)