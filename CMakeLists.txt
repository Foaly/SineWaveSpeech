cmake_minimum_required(VERSION 3.2)
set(PROJECT_NAME "SineWaveSpeech")
project(${PROJECT_NAME})


# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)


# enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Define sources and executable
include_directories("src/*")
set(EXECUTABLE_NAME "${PROJECT_NAME}")
set(SOURCE_FILES src/main.cpp
                 src/FFT.hpp
                 src/FFT.cpp
                 src/MagnitudeSpectrum.hpp
                 src/MagnitudeSpectrum.cpp
                 src/Sinusoid.hpp
                 )

# when we are on MacOS create a bundle and
# put the rescources in it
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(SOURCE_FILES ${SOURCE_FILES}
                   src/ResourcePath.hpp
                   src/ResourcePath.mm)

  file(GLOB_RECURSE RESOURCE_FILES "data/*")

  add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE ${SOURCE_FILES} ${RESOURCE_FILES})

  set_source_files_properties(${RESOURCE_FILES}
    PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
else()
  # on windows there are no bundles
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
