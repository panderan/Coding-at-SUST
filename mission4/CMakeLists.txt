CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# PROJECT NAME
PROJECT(grptools)

# Sub directories
ADD_SUBDIRECTORY(./datareader/unit_test)
ADD_SUBDIRECTORY(./datareader)
ADD_SUBDIRECTORY(./dataprocess/unit_test)
ADD_SUBDIRECTORY(./dataprocess)
ADD_SUBDIRECTORY(./dataoutput)

# Set srouce files and executable
SET(APP_SRC ./grptools.cpp)
ADD_EXECUTABLE(${PROJECT_NAME} ${APP_SRC})

# Check pkgconfig
FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(EIGEN REQUIRED eigen3)

# Set flags
SET(CMAKE_CXX_FLAGS_DEBUG -g)

# Include path
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/datareader)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/dataprocess)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/dataoutput)
INCLUDE_DIRECTORIES(${EIGEN_INCLUDE_DIRS})

# Link libs
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -Wl,-whole-archive datareader -Wl,-no-whole-archive)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -Wl,-whole-archive dataprocess -Wl,-no-whole-archive)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -Wl,-whole-archive dataoutput -Wl,-no-whole-archive)

