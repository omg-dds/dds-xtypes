cmake_minimum_required(VERSION 3.16)
project(shape LANGUAGES C)

# Find the CycloneDDS package.
if(NOT BUILD_SHARED_LIBS)
    find_package(OpenSSL REQUIRED)
endif()
find_package(CycloneDDS REQUIRED)
set(EXECUTABLE_NAME "eclipse_cyclone-${CycloneDDS_VERSION}_test_main_linux")

add_executable(${EXECUTABLE_NAME} ../test_main.c)

target_compile_definitions(${EXECUTABLE_NAME} PUBLIC -DCYCLONE_DDS)

target_link_libraries(${EXECUTABLE_NAME} CycloneDDS::ddsc CycloneDDS::dyntypelib m)
