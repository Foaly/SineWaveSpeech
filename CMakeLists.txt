cmake_minimum_required(VERSION 3.2)
set(PROJECT_NAME "SineWaveSpeech")
project(${PROJECT_NAME})


# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

message(STATUS "Configuring in ${CMAKE_BUILD_TYPE} mode.")

# enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3")

# define sources
include_directories("src/*")
set(EXECUTABLE_NAME "${PROJECT_NAME}")
set(SOURCE_FILES src/FFT.hpp
                 src/FFT.cpp
                 src/MagnitudeSpectrum.hpp
                 src/MagnitudeSpectrum.cpp
                 src/Sinusoid.hpp
                 src/Sawtooth.hpp
                 src/Triangle.hpp
                 src/SineWaveSpeech.hpp
                 src/SineWaveSpeech.cpp
                 src/ResourcePath.hpp
                 )

# when we are on MacOS create a bundle and
# put the rescources in it
if(APPLE)
  set(SOURCE_FILES ${SOURCE_FILES}
                   src/main.cpp
                   src/ResourcePath.mm)

  file(GLOB_RECURSE RESOURCE_FILES "data/*")

  add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE ${SOURCE_FILES} ${RESOURCE_FILES})

  set_source_files_properties(${RESOURCE_FILES}
    PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
else()
    if(UNIX)
        set(SOURCE_FILES ${SOURCE_FILES}
                         src/CaptainJack.cpp)
    else()
        set(SOURCE_FILES ${SOURCE_FILES}
                         src/main.cpp)
    endif()


  # on windows/linux there are no bundles
  set(SOURCE_FILES ${SOURCE_FILES}
                   src/ResourcePath.cpp)

  add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
endif()


# Add our CMake scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})


# Detect and add any version 2.X of SFML
# See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 COMPONENTS system window graphics network audio REQUIRED)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()


# Detect and add FFTW
# Find FFTW 3
find_package(FFTW COMPONENTS fftw3f REQUIRED)
if(FFTW_FOUND)
    include_directories(${FFTW_INCLUDES})
    target_link_libraries(${EXECUTABLE_NAME} ${FFTW_LIBRARIES})
endif()


if(UNIX AND NOT APPLE)
    find_library(JACK_LIBRARY
                 NAMES jack
                 PATH_SUFFIXES "lib64" "lib"
                 PATHS /usr/lib /usr/local/lib /opt/local/lib /sw/lib)
    target_link_libraries(${EXECUTABLE_NAME} ${JACK_LIBRARY})
    MESSAGE(STATUS "Jack Library: ${JACK_LIBRARY}")

    find_library (JACKCPP_LIBRARIES
                  NAMES jackcpp
                  PATH_SUFFIXES "lib64" "lib"
                  PATHS "/usr/local/lib" "/bin" /usr/local)
    target_link_libraries(${EXECUTABLE_NAME} ${JACKCPP_LIBRARIES})
    MESSAGE(STATUS "JackCPP Library: ${JACKCPP_LIBRARIES}")
endif()
