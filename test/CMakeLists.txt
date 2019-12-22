PROJECT(test_cpprequests LANGUAGES CXX)

FILE(GLOB CPPREQUESTS_TEST "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
MESSAGE("Test file found: ${CPPREQUESTS_TEST}")

ADD_COMPILE_OPTIONS(-g3)
ADD_COMPILE_OPTIONS(-O0)

FOREACH(testfile ${CPPREQUESTS_TEST})

    GET_FILENAME_COMPONENT(testname ${testfile} NAME_WE)
    MESSAGE("Target `${testname}` for current test file ${testfile}. ")

    ADD_EXECUTABLE(${testname} ${testfile})
    TARGET_LINK_LIBRARIES(${testname} curl gtest fmt spdlog z)
    TARGET_COMPILE_FEATURES(${testname} PUBLIC cxx_std_17)

ENDFOREACH(testfile ${CUPPER_TEST})
