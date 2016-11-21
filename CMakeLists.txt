cmake_minimum_required (VERSION 2.8)
project (HELLO CXX)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake" )

MESSAGE(STATUS "This is BINARY dir " ${HELLO_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir " ${HELLO_SOURCE_DIR})

# Add sources here... #
add_executable (demo main.cpp PixyCamera.cpp PixyCamera.h)

# libpixyusb should always come before libboost and libusb #

find_package( OpenCV REQUIRED )
find_package ( libpixyusb REQUIRED )
find_package ( libusb-1.0 REQUIRED )
find_package ( Boost 1.49 COMPONENTS thread system chrono REQUIRED)

target_link_libraries (demo ${LIBPIXYUSB_0_LIBRARY})
target_link_libraries (demo ${Boost_LIBRARIES})
target_link_libraries (demo ${LIBUSB_1_LIBRARY})
target_link_libraries( demo ${OpenCV_LIBS} )

file(STRINGS "cmake/VERSION" LIBPIXY_VERSION)
add_definitions(-D__LIBPIXY_VERSION__="${LIBPIXY_VERSION}")

include_directories (
        ${Boost_INCLUDE_DIR}
        ${LIBUSB_1_INCLUDE_DIRS})