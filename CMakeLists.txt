CMAKE_MINIMUM_REQUIRED(VERSION 3.3)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

PROJECT(xtypes-interop CXX)
FIND_PACKAGE(GTest REQUIRED)


IF(NOT DEFINED VENDOR_NAME_MASTER)
    MESSAGE(FATAL_ERROR "Please tell me what DDS vendor to use by passing -DVENDOR_NAME_MASTER=<name>")
ENDIF()

IF(NOT DEFINED VENDOR_NAME_SLAVE)
    SET(VENDOR_NAME_SLAVE ${VENDOR_NAME_MASTER})
    MESSAGE("VENDOR_NAME_SLAVE not specified, using ${VENDOR_NAME_MASTER}")
    MESSAGE("You can set a different vendor for slave processes by passing -DVENDOR_NAME_SLAVE=<name>")
ENDIF()


SET(IDL_GEN_ROOT    ${CMAKE_CURRENT_SOURCE_DIR}/types)
IF (NOT EXISTS ${IDL_GEN_ROOT})
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory ${IDL_GEN_ROOT})
ENDIF ()

SET(CONFORMANCE_ROOT  ${CMAKE_CURRENT_SOURCE_DIR}/conformance)
SET(CONFORMANCE_MASTER  ${CMAKE_CURRENT_SOURCE_DIR}/conformance/${VENDOR_NAME_MASTER})
SET(CONFORMANCE_SLAVE  ${CMAKE_CURRENT_SOURCE_DIR}/conformance/${VENDOR_NAME_SLAVE})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)

SET(IDL_SOURCE_C commands.idl shapes.idl)

FUNCTION(ADD_DDS_VENDOR_LIB VENDOR_NAME)
    SET(IDL_GEN_DIR   ${IDL_GEN_ROOT}/${VENDOR_NAME})
    SET(CONFORMANCE_DIR   ${CONFORMANCE_ROOT}/${VENDOR_NAME})
    IF (NOT EXISTS ${IDL_GEN_DIR})
        EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory ${IDL_GEN_DIR})
    ENDIF ()

    INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/${VENDOR_NAME}.cmake")
    SET(ALL_IDL_GEN_FILES)

    FOREACH(idl ${IDL_SOURCE_C})
        GET_FILENAME_COMPONENT(filename ${idl} NAME)
        STRING(REGEX REPLACE "\\.idl" "" basename ${filename})

        SET(IDL_GEN_FILES)
        GET_DDS_GEN_FILES(${basename} IDL_GEN_FILES)

        SET(IDL_GEN_COMMAND)
        GET_DDS_GEN_COMMAND(${CMAKE_CURRENT_SOURCE_DIR}/idl/${idl} ${IDL_GEN_DIR} IDL_GEN_COMMAND)

        SET(IDL_GEN_FILES_FULL_PATH)
        FOREACH(file ${IDL_GEN_FILES})
            LIST(APPEND ALL_IDL_GEN_FILES ${IDL_GEN_DIR}/${file})
            LIST(APPEND IDL_GEN_FILES_FULL_PATH ${IDL_GEN_DIR}/${file})
        ENDFOREACH()

        ADD_CUSTOM_COMMAND(OUTPUT   ${IDL_GEN_FILES_FULL_PATH}

                           COMMAND  ${IDL_GEN_COMMAND}
                           WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                           DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}/idl/${idl})

        SET(DDS_INCLUDE_DIRS)
        SET(DDS_LINK_LIBS)
        SET(DDS_COMPILE_DEFINITIONS)
        GET_DDS_INFO(DDS_INCLUDE_DIRS DDS_LINK_LIBS DDS_COMPILE_DEFINITIONS)
        ADD_LIBRARY(${basename}_${VENDOR_NAME} ${IDL_GEN_FILES_FULL_PATH})
        TARGET_COMPILE_DEFINITIONS(${basename}_${VENDOR_NAME} 
                                    PUBLIC ${DDS_COMPILE_DEFINITIONS} 
                                    INTERFACE -D${VENDOR_NAME})
        TARGET_LINK_LIBRARIES(${basename}_${VENDOR_NAME} ${DDS_LINK_LIBS})
        TARGET_INCLUDE_DIRECTORIES(${basename}_${VENDOR_NAME} 
                                    PUBLIC ${DDS_INCLUDE_DIRS} ${IDL_GEN_DIR} 
                                    INTERFACE ${CONFORMANCE_DIR})
    ENDFOREACH()
    SET_SOURCE_FILES_PROPERTIES(${ALL_IDL_GEN_FILES} PROPERTIES GENERATED 1)


ENDFUNCTION()


SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/objs/)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/objs/)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/objs/)

ADD_DDS_VENDOR_LIB(${VENDOR_NAME_MASTER})
IF(NOT ${VENDOR_NAME_MASTER} STREQUAL ${VENDOR_NAME_SLAVE})
    ADD_DDS_VENDOR_LIB(${VENDOR_NAME_SLAVE})
ENDIF()



SET(COMMON_SRC  ${CMAKE_CURRENT_SOURCE_DIR}/src/TestController.cxx 
                ${CMAKE_CURRENT_SOURCE_DIR}/src/TestController.h
                ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils.cxx
                ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils.h
                ${CMAKE_CURRENT_SOURCE_DIR}/src/DDS_Variants.h)

ADD_EXECUTABLE(xtypes-interop 
               ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cxx
               ${CMAKE_CURRENT_SOURCE_DIR}/src/TestDataRepresentationQos/TestDataRepresentationQos.cxx
               ${CMAKE_CURRENT_SOURCE_DIR}/src/TestHelloWorld/TestHelloWorld.cxx
               ${CMAKE_CURRENT_SOURCE_DIR}/src/TestExtensibility/TestExtensibility.cxx
               ${CMAKE_CURRENT_SOURCE_DIR}/src/TestSerialization/TestSerialization.cxx
               ${CMAKE_CURRENT_SOURCE_DIR}/src/TestSerialization/SerializationData.cxx
               ${CMAKE_CURRENT_SOURCE_DIR}/src/TestSerialization/SerializationData.h
               ${CONFORMANCE_MASTER}/Conformance.h
               ${COMMON_SRC}
               )

ADD_EXECUTABLE(TestDataRepresentationQosHelper 
                ${CMAKE_CURRENT_SOURCE_DIR}/src/TestDataRepresentationQos/TestDataRepresentationQosHelper.cxx
                ${CONFORMANCE_SLAVE}/Conformance.h
                ${COMMON_SRC}
                )

ADD_EXECUTABLE(TestHelloWorldHelper 
                ${CMAKE_CURRENT_SOURCE_DIR}/src/TestHelloWorld/TestHelloWorldHelper.cxx
                ${COMMON_SRC}
                ${CONFORMANCE_SLAVE}/Conformance.h
                )

ADD_EXECUTABLE(TestExtensibilityHelper 
                ${CMAKE_CURRENT_SOURCE_DIR}/src/TestExtensibility/TestExtensibilityHelper.cxx
                ${COMMON_SRC}
                ${CONFORMANCE_SLAVE}/Conformance.h
                )

ADD_EXECUTABLE(TestSerializationHelper 
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/TestSerialization/TestSerializationHelper.cxx
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/TestSerialization/SerializationData.cxx
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/TestSerialization/SerializationData.h
                 ${COMMON_SRC}
                 ${CONFORMANCE_SLAVE}/Conformance.h
                 )

TARGET_INCLUDE_DIRECTORIES(xtypes-interop                   PRIVATE ${GTEST_INCLUDE_DIRS})


TARGET_LINK_LIBRARIES(TestDataRepresentationQosHelper   
                        commands_${VENDOR_NAME_SLAVE} shapes_${VENDOR_NAME_SLAVE})
TARGET_LINK_LIBRARIES(TestHelloWorldHelper              
                        commands_${VENDOR_NAME_SLAVE} shapes_${VENDOR_NAME_SLAVE})
TARGET_LINK_LIBRARIES(TestExtensibilityHelper           
                        commands_${VENDOR_NAME_SLAVE} shapes_${VENDOR_NAME_SLAVE})
TARGET_LINK_LIBRARIES(TestSerializationHelper           
                        commands_${VENDOR_NAME_SLAVE} shapes_${VENDOR_NAME_SLAVE})

TARGET_LINK_LIBRARIES(xtypes-interop                    
                        commands_${VENDOR_NAME_MASTER} 
                        shapes_${VENDOR_NAME_MASTER} 
                        ${GTEST_LIBRARIES})


ADD_DEPENDENCIES(xtypes-interop TestDataRepresentationQosHelper TestHelloWorldHelper TestExtensibilityHelper TestSerializationHelper)
