cmake_minimum_required(VERSION 2.8)
project( BubbleDetectorApp )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
set( CMAKE_BUILD_TYPE Debug )
add_executable( BubbleDetectorApp src/main.cpp )
target_link_libraries( BubbleDetectorApp ${OpenCV_LIBS} )

# cmake -DCMAKE_BUILD_TYPE=Debug -DOMS_DEBUG=true
# cmake -DCMAKE_BUILD_TYPE=Release -DOMS_DEMO=true
