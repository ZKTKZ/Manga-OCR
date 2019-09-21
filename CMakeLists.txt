set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 2.8)
project( detect_text )
find_package( OpenCV REQUIRED )
FIND_PACKAGE(OpenMP REQUIRED)
#if(OPENMP_CXX_FOUND)
#        SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -MP")
#endif()

#add_library( libccv STATIC ccv/lib/libccv.a ) 
#set_target_properties( libccv PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/ccv/lib/libccv.a ) 
#target_include_directories( libccv PUBLIC ${PROJECT_SOURCE_DIR}/include )
#include_directories( ${CMAKE_CURRENT_BINARY_DIR}/ccv/lib ) 

#include_directories( ${CMAKE_CURRENT_BINARY_DIR}/Robust-Text-Detection/RobustTextDetection ) 
#add_library( mser RobustTextDetection.cpp RobustTextDetection.h ConnectedComponent.cpp ConnectedComponent.h )
#set_target_properties( mser PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/Robust-Text-Detection/RobustTextDetection )
#target_include_directories( mser PUBLIC ${PROJECT_SOURCE_DIR}/include )


# set(LIBS ${LIBS} "${CMAKE_CURRENT_BINARY_DIR}/src/libccv_project/lib/libccv.a;dispatch;jpeg;png")
#add_definitions(${PNG_DEFINITIONS})
#find_package( PNG REQUIRED )
#include_directories(${PNG_INCLUDE_DIRS})
#set(LIBS ${LIBS} ${JPEG_LIBRARIES})

#find_package( JPEG REQUIRED )
#include_directories( ${JPEG_INCLUDE_DIRS} )
#set(LIBS ${LIBS} ${PNG_LIBRARIES})

add_executable( detect_text detect_text.cpp ) #ccv/lib/nnc/ccv_nnc_tfb.h )
target_link_libraries( detect_text ${OpenCV_LIBS} stdc++fs tesseract lept) # libccv 
#set_target_properties( libccv PROPERTIES LINKER_LANGUAGE CXX)
