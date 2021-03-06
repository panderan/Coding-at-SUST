CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(dataprocess_unit_test)
SET(DP_UNIT_TEST_SRC ./main.cpp
                ./display_dataprocess_test.cpp
                ./spanningtree_dataprocess_test.cpp
                ../dataprocess_factory.cpp
                ../display_dataprocess.cpp
                ../spanningtree_dataprocess.cpp)

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-D_DEBUG_ON_)
ENDIF()

FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(EIGEN REQUIRED eigen3)
PKG_CHECK_MODULES(CPPUNIT REQUIRED cppunit)

INCLUDE_DIRECTORIES(${EIGEN_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CPPUNIT_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../..)
LINK_DIRECTORIES(${CPPUNIT_LIBRARY_DIRS})
SET(CMAKE_CXX_FLAGS_DEBUG -g)

ADD_EXECUTABLE(${PROJECT_NAME} ${DP_UNIT_TEST_SRC})

#Settings for test
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../datareader)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -Wl,--whole-archive datareader -Wl,--no-whole-archive)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CPPUNIT_LIBRARIES})
