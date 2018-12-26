cmake_minimum_required(VERSION 3.5)
project(rocky_hockey C CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


find_package(OpenCV 4.0.0 REQUIRED)
find_package (Threads REQUIRED)
find_package(Boost COMPONENTS system thread)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE RELEASE)

include_directories(${CMAKE_SOURCE_DIR}/Rocky.Hockey/ext/eigen3/)
include_directories(${CMAKE_SOURCE_DIR}/Rocky.Hockey/ext/cpp-json/lib/include)
include_directories(${CMAKE_SOURCE_DIR}/Rocky.Hockey/ext/websocketpp/)


if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O3")        ## Optimize
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DBUILD_PI")
endif()

set(SOURCE_FILES
	Rocky.Hockey/main.cpp
	Rocky.Hockey/Config.cpp
	Rocky.Hockey/Config.h
	Rocky.Hockey/MotionController.cpp
	Rocky.Hockey/MotionController.h
	Rocky.Hockey/SerialConnection.cpp
	Rocky.Hockey/SerialConnection.h
	Rocky.Hockey/Tracker.cpp
	Rocky.Hockey/Tracker.h
	Rocky.Hockey/RockyHockeyMain.cpp
	Rocky.Hockey/RockyHockeyMain.h
	Rocky.Hockey/definitions.h
	Rocky.Hockey/Puck.h
	Rocky.Hockey/Puck.cpp
	Rocky.Hockey/Prediction.cpp
	Rocky.Hockey/Prediction.h
	Rocky.Hockey/ImageTransformation.h
    Rocky.Hockey/ImageTransformation.cpp
        )

add_executable(rocky_hockey ${SOURCE_FILES})

target_link_libraries(rocky_hockey ${OpenCV_LIBS})
target_link_libraries(rocky_hockey ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(rocky_hockey ${Boost_LIBRARIES})
#target_link_libraries (rocky_hockey Eigen3::Eigen)

