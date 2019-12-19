PROJECT(cpprequests C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(external/curl-7.67.0/include)
INCLUDE_DIRECTORIES(external/fmt/include)
INCLUDE_DIRECTORIES(external/spdlog/include)
INCLUDE_DIRECTORIES(external/gzip-hpp/include)
INCLUDE_DIRECTORIES(external/json/include)
INCLUDE_DIRECTORIES(external/cpp-httplib/)
include_directories(external/googletest/googletest/include)

ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src)

ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/test)
#ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/external/curl-7.67.0)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/external/fmt)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/external/spdlog)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/external/json)
