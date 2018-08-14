#.rst:
# (c) 2017 Copyright, Real-Time Innovations, Inc. All rights reserved.
# RTI grants Licensee a license to use, modify, compile, and create derivative
# works of this file solely for use with RTI Connext DDS.  Licensee may
# redistribute copies of this file provided that all such copies are subject
# to this license.
#
# FindRTICodeGenerator
# -----------------
#
# Find RTI Code Generator.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
# This module will set the following variables in your project:
#
# - ``RTICODEGEN``
#   Path to the RTI Code Generator executable.
# - ``RTICODEGEN_VERSION``
#   RTI Code Generator version.
#
# Hints
# ^^^^^
# You must provide the path to the RTI Code Generator folder by defining the
# RTICODEGEN_DIR variable in your cmake invocation. For example:
#    cmake -DRTICODEGEN_DIR=/home/rti/rti_connext_dds-x.y.z/bin
#
# Alternatively, especially if you are combining the use of
# find_package(RTICodeGenerator) with find_package(RTIConnextDDS), you can
# simply define the CONNEXTDDS_DIR variable in your cmake invocation, and this
# module will deduce the path to the RTI Code Generator executable. For example:
#    cmake -DCONNEXTDDS_DIR=/home/rti/rti_connext_dds-x.y.z
#
# Examples
# ^^^^^
# Simple CMake project checking the RTI Code Generator version
# ::
#   cmake_minimum_required(VERSION 3.3.2)
#   project(codegentest)
#   set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
#      "/home/rti/rti_connext_dds-5.3.0/resource/cmake/modules")
#
#   find_package(RTICodeGenerator "2.5.0" EXACT)
#

# Find RTI Code Generator. We provide some hints that include the
# RTICODEGEN_DIR variable, CONNEXTDDS_DIR variable, and the $NDDSHOME
# environment variable.
if (NOT DEFINED RTICODEGEN_DIR)
    set(connextdds_root_hints
        "${CONNEXTDDS_DIR}/bin/"
        "$ENV{NDDSHOME}/bin"
        "${connextdds_root_hints}")
endif()

set(codegen_executable_name "rtiddsgen")

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(codegen_executable_name "${codegen_executable_name}.bat")
endif()

find_program(   RTICODEGEN ${codegen_executable_name}
                HINTS ${connextdds_root_hints} ${RTICODEGEN_DIR})

if (NOT RTICODEGEN)
    string(CONCAT
        error
        "RTICODEGEN_DIR not specified. Please set -DRTICODEGEN_DIR= to "
        "your RTI Code Generator directory.")
    message(FATAL_ERROR ${error})
endif()

# Execute RTI Code Generator to get the version
execute_process(COMMAND ${RTICODEGEN} -version
    OUTPUT_VARIABLE codegen_version_string)

string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+"
    RTICODEGEN_VERSION ${codegen_version_string})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RTICodeGenerator
    REQUIRED_VARS RTICODEGEN RTICODEGEN_VERSION
    VERSION_VAR RTICODEGEN_VERSION
)
