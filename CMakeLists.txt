cmake_minimum_required (VERSION 3.0.0)
project (TURTLE_GEANT4_TEST
    LANGUAGES CXX
    VERSION 0.9
)

include(ExternalProject)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release
            RelWithDebInfo MinSizeRel." FORCE)
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR} CACHE PATH
            "Install path prefix." FORCE)
endif()

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")


# Geant4 includes
find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})
include_directories(${Geant4_INCLUDE_DIR})


# Configure or install TURTLE
set(TURTLE_PREFIX "" CACHE STRING "Installation prefix of TURTLE")
if(TURTLE_PREFIX STREQUAL "")
    # INSTALL the TURTLE library
    ExternalProject_Add(TURTLE
        URL https://github.com/kvernet/turtle-geant4/archive/refs/heads/dev-kvernet-v${TURTLE_GEANT4_TEST_VERSION}.zip
        CONFIGURE_COMMAND cmake ../TURTLE -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        BUILD_COMMAND make
        INSTALL_COMMAND make install
    )
    
    link_directories(${CMAKE_INSTALL_PREFIX}/lib)
    include_directories(${CMAKE_INSTALL_PREFIX}/include)
else()
    link_directories(${TURTLE_PREFIX}/lib)
    include_directories(${TURTLE_PREFIX}/include)
endif()

include_directories(${CMAKE_SOURCE_DIR}/src/include)

file(GLOB srcs ${CMAKE_SOURCE_DIR}/src/src/*.cpp)
file(GLOB headers ${CMAKE_SOURCE_DIR}/src/include/*.hh)

add_executable(turtle-rock ${srcs} ${headers} turtle-rock.cpp)
target_link_libraries(turtle-rock G4turtle ${Geant4_LIBRARIES})
install(TARGETS turtle-rock DESTINATION bin)
