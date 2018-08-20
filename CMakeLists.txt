cmake_minimum_required(VERSION 2.6)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project(knight-general)

set(PROJECT_VERSION "0.1.0")

message(STATUS "CMAKE_SYSTEM_NAME          = ${CMAKE_SYSTEM_NAME}"         )
message(STATUS "CMAKE_CXX_COMPILER_ID      = ${CMAKE_CXX_COMPILER_ID}"     )
message(STATUS "CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_COMPILER_IS_GNUCXX   = ${CMAKE_COMPILER_IS_GNUCXX}"  )

set(EXECUTABLE_NAME ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -pedantic")

# cpack

set(CPACK_PACKAGE_CONTACT "lchsk.com")
set(CPACK_GENERATOR "DEB;ZIP;")
set(CPACK_PACKAGE_DESCRIPTION, "Turn-based strategy game")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

# deb stuff
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://lchsk.com")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-dev")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "lchsk.com")

include(CPack)

# end cpack

file(GLOB SOURCES "src/*.hpp" "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(PkgConfig)

pkg_check_modules(SFML sfml-all)
message(STATUS "SFML_VERSION: ${SFML_VERSION}")

include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

install(TARGETS knight-general DESTINATION games)
install(DIRECTORY knight_general_assets DESTINATION games)
