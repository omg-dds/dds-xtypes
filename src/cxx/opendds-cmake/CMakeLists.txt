cmake_minimum_required(VERSION 3.12)
project(opendds_shape_main CXX)

find_package(OpenDDS REQUIRED)

add_executable(shape_main
  ../test_main.cxx
)

target_compile_definitions(shape_main
  PRIVATE OPENDDS
)

target_link_libraries(shape_main OpenDDS::Rtps_Udp)
