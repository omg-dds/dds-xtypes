cmake_minimum_required(VERSION 3.16)
project(shape LANGUAGES C)

# Find the CycloneDDS package.
if(NOT BUILD_SHARED_LIBS)
    find_package(OpenSSL REQUIRED)
endif()
find_package(CycloneDDS REQUIRED)
set(EXECUTABLE_NAME "eclipse_cyclone-${CycloneDDS_VERSION}_test_main_linux")

add_executable(${EXECUTABLE_NAME} ../test_main.c
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/dyntypelib.c
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/dyntypelib.h 
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/domtree.c
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/domtree.h
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/type_cache.c
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/type_cache.h
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/print_sample.c
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/print_type.c
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/print_type.h
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/compare_samples.c
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/compare_samples.h
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/scan_sample.c
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/size_and_align.c
    ${CMAKE_PREFIX_PATH}/share/CycloneDDS/examples/dynsub/size_and_align.h
    )

target_compile_definitions(${EXECUTABLE_NAME} PUBLIC -DCYCLONE_DDS)

target_link_libraries(${EXECUTABLE_NAME} CycloneDDS::ddsc m)
