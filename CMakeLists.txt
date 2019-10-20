cmake_minimum_required(VERSION 3.12)
project(shakalaka)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")

#C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin
set(SOURCE Sbasic.cpp units.cpp Field.cpp SHAKAL.cpp player.cpp GUI.cpp)

list(TRANSFORM SOURCE PREPEND "src/")

set(SFML_DIR "D:/Libs/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML REQUIRED COMPONENTS window graphics audio network)

#D:/Libs/SFML-2.5.1/lib/cmake/SFML
#C:/Libs/SFML-2.5.1/lib/cmake/SFML

add_executable(shakalaka main.cpp ${SOURCE})

target_link_libraries(shakalaka sfml-window sfml-graphics sfml-audio)

include_directories(/)