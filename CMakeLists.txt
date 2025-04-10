cmake_minimum_required(VERSION 3.10)

project(shape_type_demo)

list(APPEND CMAKE_MODULE_PATH "$ENV{DDS_ROOT}/cmake")
find_package(OpenDDS REQUIRED)

set(OPENDDS_DEFAULT_NESTED OFF)
add_library(shape_type_idl)
OPENDDS_TARGET_SOURCES(shape_type_idl ShapeType.idl OPENDDS_IDL_OPTIONS -Gprinter)
target_link_libraries(shape_type_idl PUBLIC OpenDDS::Dcps)

add_executable(shape_type ShapeType_main.cxx)
target_link_libraries(shape_type OpenDDS::Rtps_Udp)
target_link_libraries(shape_type shape_type_idl)
target_compile_definitions(shape_type PUBLIC OCI_OPENDDS)
