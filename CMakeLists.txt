cmake_minimum_required(VERSION 3.10)

project(shape_type_demo)

# This CMakeLists.txt is intended to be used with any DDS implementation that
# supports CMake.  An example using OpenDDS is below, and other implementations
# can be added.  In each case, if find_package() doesn't find the target
# implementation then processing should continue (REQUIRED is not used) to
# attempt to find other implementations.

find_package(OpenDDS)

if(${OpenDDS_FOUND})
  add_executable(opendds_xtypes_test ShapeType_main.cxx)
  opendds_target_sources(opendds_xtypes_test ShapeType.idl
    OPENDDS_IDL_OPTIONS -Gxtypes-complete --no-default-nested)
  target_link_libraries(opendds_xtypes_test OpenDDS::Rtps_Udp)
  target_compile_definitions(opendds_xtypes_test PUBLIC OCI_OPENDDS)
endif()
