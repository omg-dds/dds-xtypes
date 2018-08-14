#.rst:
# (c) 2017 Copyright, Real-Time Innovations, Inc. All rights reserved.
# RTI grants Licensee a license to use, modify, compile, and create derivative
# works of this file solely for use with RTI Connext DDS.  Licensee may
# redistribute copies of this file provided that all such copies are subject
# to this license.
#
# FindRTIConnextDDS
# -----------------
#
# Find RTI Connext DDS libraries.
#
# Components
# ^^^^^^^^^^
# This module will set variables for the following components that are part
# of RTI Connext DDS:
# - core (default, always provided)
# - messaging_api
# - security_plugins
# - routing_service
# - monitoring_libraries
# - distributed_logger
#
# Core is always selected, because the rest of components depend on it.
# However, the rest of components must be explicitly selected in the
# find_package invocation for this module to set variables for the different
# libraries associated with them.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
# This module will set the following variables in your project:
#
# - ``CONNEXTDDS_DEFINITIONS``
#   RTI Connext DDS Compiler definitions.
# - ``CONNEXTDDS_EXTERNAL_LIBS``
#   RTI Connext DDS external dependencies.
# - ``CONNEXTDDS_INCLUDE_DIRS``
#   RTI Connext DDS include directories.
# - ``CONNEXTDDS_DLL_EXPORT_MACRO``
#   Macros to compile against RTI Connext DDS shared libraries on Windows.
#
# This module will set the following variables for all the different libraries
# that are part of the components selected at configuration time.
#
# - ``<LIBRARY_NAME>_LIBRARIES_RELEASE_STATIC``
#   Release static libraries for <LIBRARY_NAME>.
#   (e.g., ``CONNEXTDDS_C_API_LIBRARIES_RELEASE_STATIC``).
# - ``<LIBRARY_NAME>_LIBRARIES_RELEASE_SHARED``
#   Release shared libraries for <LIBRARY_NAME>.
#   (e.g., ``CONNEXTDDS_C_API_LIBRARIES_RELEASE_SHARED``).
# - ``<LIBRARY_NAME>_LIBRARIES_DEBUG_STATIC``
#   Debug static libraries for <LIBRARY_NAME>.
#   (e.g., ``CONNEXTDDS_C_API_LIBRARIES_DEBUG_STATIC``).
# - ``<LIBRARY_NAME>_LIBRARIES_DEBUG_SHARED``
#   Debug shared libraries for <LIBRARY_NAME>.
#   (e.g., ``CONNEXTDDS_C_API_LIBRARIES_DEBUG_SHARED``).
#
# The list of libraries for each component is the following:
#
# - ``core`` component (always set by this module):
#   - ``CONNEXTDDS_C_API``
#     (e.g., ``CONNEXTDDS_C_API_LIBRARIES_RELEASE_STATIC``)
#   - ``CONNEXTDDS_CPP_API``
#     (e.g., ``CONNEXTDDS_CPP_API_LIBRARIES_RELEASE_STATIC``)
#   - ``CONNEXTDDS_CPP2_API``
#     (e.g., ``CONNEXTDDS_CPP2_API_LIBRARIES_RELEASE_STATIC``)

# - ``messaging_api`` component:
#   - ``MESSAGING_C``
#     (e.g., ``MESSAGING_C_API_LIBRARIES_RELEASE_STATIC``)
#   - ``MESSAGING_CPP``
#     (e.g., ``MESSAGING_CPP_API_LIBRARIES_RELEASE_STATIC``)
#   - ``MESSAGING_CPP2``
#     (e.g, ``MESSAGING_CPP2_API_LIBRARIES_RELEASE_STATIC``)
#
# - ``routing_service``:
#   - ``ROUTING_SERVICE_API``
#     (e.g., ``ROUTING_SERVICE_API_LIBRARIES_RELEASE_STATIC``)
#
# - ``security_plugins``:
#   - ``SECURITY_PLUGINS``
#     (e.g., ``SECURITY_PLUGINS_LIBRARIES_RELEASE_STATIC``)
#
# - ``monitoring_libraries``:
#   - ``MONITORING_LIBRARIES``
#     (e.g., ``MONITORING_LIBRARIES_RELEASE_STATIC``)
#
# If you are building a simple ConnextDDS application (you are only using
# the  core libraries), use the following variables:
#  - For a C application: CONNEXTDDS_C_API
#  - For a Traditional C++ application: CONNEXTDDS_CPP_API
#  - For a Modern C++ application: CONNEXTDDS_CPP2_API
#
# If you are building a Routing Service Adapter, or Routing Service as a
# Libary use the following:
#  - MONITORING_LIBRARIES
#  - MESSAGING
#  - ROUTING_SERVICE_API
#  - CONNEXTDDS_C_API
#
# Lastly, if you want to use the security plugins (or any other component),
# add the appropriate variables to your CMake script.
#
# Hints
# ^^^^^
# If the find_package invocation specifies a version, this module will try
# to find your Connext DDS installation in the default installation
# directories. Likewise, the module will try to guess the name of the
# architecture you are trying to build against, by looking for it under the
# rti_connext_dds-x.y.z/lib.
#
# However, in some cases you must provide the following hints by defining some
# variables in your cmake invocation:
#
# - If you don't specify a version or you have installed Connext DDS in a
#   non-default location, you must set the ``CONNEXTDDS_DIR`` pointing to your
#   RTI Connext DDS installation folder. For example:
#       cmake -DCONNEXTDDS_DIR=/home/rti/rti_connext_dds-x.y.z
#
# - If you have installed more than one architecture on your system (i.e., more
#   than one target rtipkg), you must set the ``CONNEXTDDS_ARCH`` to provide
#   the name of the architecture. For example:
#       cmake -DCONNEXTDDS_ARCH=x64Linux3gcc5.4.0
#
# - If you are building against the security_plugins compoment, this module will
#   try to find OpenSSL in your system using find_package(OpenSSL). If you want
#   to build against a specific installation of OpenSSL, you must set the
#   ``CONNEXTDDS_OPENSSL_DIR`` to provide this module with the path to your
#   installation of OpenSSL.
#
# - Likewise, if you are building against the security_plugins compoment and
#   you want to ensure that you are using a specific OpenSSL version, you must
#   set the ``CONNEXTDDS_OPENSSL_VERSION`` so that it can be added to the
#   find_package(OpenSSL) invocation.
#
#
# Examples
# ^^^^^
# Simple Connext DDS application
# ::
#   cmake_minimum_required(VERSION 3.3.2)
#   project (example)
#   set(CMAKE_MODULE_PATH
#       ${CMAKE_MODULE_PATH}
#       "/home/rti/rti_connext_dds-5.3.0/resource/cmake")
#
#   find_package(RTIConnextDDS EXACT "5.3.0" REQUIRED)
#   add_definitions("${CONNEXTDDS_DEFINITIONS}")
#   include_directories("src" ${CONNEXTDDS_INCLUDE_DIRS})
#
#   set(SOURCES_PUB
#       src/HelloWorld_publisher.c
#       src/HelloWorld.c
#       src/HelloWorldPlugin.c
#       src/HelloWorldSupport.c)
#
#   add_executable(HelloWorld_publisher ${SOURCES_PUB})
#   TARGET_LINK_LIBRARIES(HelloWorld_publisher
#       ${CONNEXTDDS_C_API_LIBRARIES_DEBUG_STATIC}
#       ${CONNEXTDDS_EXTERNAL_LIBS})
#
#
# Simple Routing Service adapter
# ::
#   cmake_minimum_required(VERSION 3.3.0)
#   project (example)
#   set(CMAKE_MODULE_PATH
#       ${CMAKE_MODULE_PATH}
#       "/home/rti/rti_connext_dds-5.3.0/resource/cmake")
#
#   find_package(RTIConnextDDS EXACT "5.3.0" REQUIRED COMPONENTS routing_service)
#   add_definitions("${CONNEXTDDS_DEFINITIONS}")
#   include_directories("src" ${CONNEXTDDS_INCLUDE_DIRS})
#
#   set(LIBRARIES
#       ${ROUTING_SERVICE_API_LIBRARIES_RELEASE_STATIC}
#       ${CONNEXTDDS_C_API_LIBRARIES_DEBUG_STATIC}
#       ${CONNEXTDDS_EXTERNAL_LIBS})
#
#   set(SOURCES_LIB src/FileAdapter.c src/LineConversion.c src/osapi.c)
#
#   add_library(fileadapter SHARED ${SOURCES_LIB})
#   TARGET_LINK_LIBRARIES(fileadapter ${LIBRARIES})
#
# Supported platforms
# ^^^^^
# - Linux i86 and x64
# - armv6vfphLinux3.xgcc4.7.2
# - armv7aAndroid2.3gcc4.8 and armv7aAndroid5.0gcc4.9ndkr10e
# - Windows i86 and x64
# - Darwin
#

#####################################################################
# Preconditions Check                                               #
#####################################################################
#
# Find RTI Connext DDS installation. We provide some hints that include the
# CONNEXTDDS_DIR variable, the $NDDSHOME environment variable, and the
# default installation directories.
if (NOT DEFINED CONNEXTDDS_DIR)
    if (RTIConnextDDS_FIND_VERSION)
        if (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
            set(connextdds_root_hints
                "$ENV{HOME}/rti_connext_dds-${RTIConnextDDS_FIND_VERSION}")
        elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
            set(connextdds_root_hints
                "C:/Program Files (x86)/rti_connext_dds-${RTIConnextDDS_FIND_VERSION}"
                "C:/Program Files/rti_connext_dds-${RTIConnextDDS_FIND_VERSION}"
                "C:/rti_connext_dds-${RTIConnextDDS_FIND_VERSION}")
        elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
            set(connextdds_root_hints
                "/Applications/rti_connext_dds-${RTIConnextDDS_FIND_VERSION}")
        else()
            MESSAGE("CMAKE_HOST_SYSTEM_NAME = ${CMAKE_HOST_SYSTEM_NAME}")
        endif()
    else()

    endif()

    set(connextdds_root_hints
        "$ENV{NDDSHOME}"
        "${connextdds_root_hints}")
    find_path(CONNEXTDDS_DIR NAMES rti_versions.xml PATHS ${connextdds_root_hints})
endif()

# We require having an rti_versions file under the installation directory
# as we will use it to verify that the version is appropriate.

if(NOT CONNEXTDDS_DIR)
    string(CONCAT
        error
        "CONNEXTDDS_DIR not specified. Please set -DCONNEXTDDS_DIR= to "
        "your RTI Connext DDS installation directory")
    message(FATAL_ERROR ${error})
endif()

message(STATUS "RTI Connext DDS installation directory... ${CONNEXTDDS_DIR}")

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

# Find RTI Connext DDS architecture if CONNEXTDDS_ARCH is unset
if(NOT DEFINED CONNEXTDDS_ARCH)
    if(DEFINED ENV{CONNEXTDDS_ARCH})
        set(CONNEXTDDS_ARCH $ENV{CONNEXTDDS_ARCH})
    else()
        # If CONNEXTDDS_ARCH is unspecified, the module tries uses the first
        # architecture installed by looking under $CONNEXTDDS_DIR/lib.
        SUBDIRLIST(architectures_installed ${CONNEXTDDS_DIR}/lib)
        list(GET architectures_installed 0 architecture_name)

        if(architecture_name)
            if(architecture_name MATCHES "[Jj]ava")
                # Because the lib folder contains both target libraries and
                # java jar files, here we exclude the "java" in our algorithm
                # to guess the appropriate CONNEXTDDS_ARCH variable.
                list(GET architectures_installed 1 architecture_name)
                if(architecture_name)
                    set(CONNEXTDDS_ARCH ${architecture_name})
                else()
                    string(CONCAT
                        error
                        "Could not find architecture to build against. "
                        "Please set -DCONNEXTDDS_ARCH= to specify your RTI "
                        "Connext DDS architecture.")
                    message(FATAL_ERROR ${error})
                endif()
            else()
                set(CONNEXTDDS_ARCH ${architecture_name})
            endif()
        else()
            string(CONCAT
                error
                "CONNEXTDDS_ARCH not specified. Please set "
                "-DCONNEXTDDS_ARCH= to specify your RTI Connext DDS "
                " architecture")
            message(FATAL_ERROR ${error})
        endif()
    endif()
endif()

message(STATUS "RTI Connext DDS architecture... ${CONNEXTDDS_ARCH}")

#####################################################################
# Helper Functions                                                  #
#####################################################################

# Checks that a certain field associated with a component is installed under
# CONNEXTDDS_DIR and that its version is consistent with the version required
# for the rest of modules. If RTICONNEXTDDS_VERSION, the version variable, is
# specified, then it checks that the value is consistent. Otherwise,
# it sets the value of RTICONNEXTDDS_VERSION so that it can be checked in
# future calls of this function to validate that its value is consistent
# accross fields and components.
#
# At the end of the script CMake will check that RTICONNEXTDDS_VERSION is
# consistent with the version specified by the user in the find_package
# invocation, acording to the matching rules defined by CMake.
#
# Arguments:
# - field_name: provides the name of the
# Returns:
# - Throws an error if the component field is not present in rti_versions.xml
#   or the version does not match RTICONNEXTDDS_VERSION.
function(connextdds_check_component_field_version
        field_name
        rti_versions_file
        rti_architectures)
    # Get the field info
    set(field_section_regex "<${field_name}>.*</${field_name}>")
    string(REGEX MATCH ${field_section_regex} field_xml "${rti_versions_file}")

    foreach(architecture IN LISTS ${rti_architectures})
        string(CONCAT field_arch_regex
                "<installation>[\n\r\t ]*"
                "<architecture>${architecture}.*</architecture>"
                "[\n\r\t ]*<version>[0-9]+\\.[0-9]+\\.[0-9]+(\\.[0-9]+(\\.[0-9]+)?)?"
                "</version>")
        string(REGEX MATCH ${field_arch_regex} field_arch "${field_xml}")
        if(field_arch)
            break()
        endif()
    endforeach()

    if(NOT field_arch)
        string(CONCAT error
                "${field_name} is not installed for ${CONNEXTDDS_ARCH} "
                "under ${CONNEXTDDS_DIR}")
        message(WARNING ${error})
        set(rtiversion_error TRUE PARENT_SCOPE)
    endif()

    # Get the <version> tag from a field in CONNEXTDDS_DIR/rti_versions.xml
    set(field_version_regex
            "<version>[0-9]+\\.[0-9]+\\.[0-9]+(\\.[0-9]+(\\.[0-9]+)?)?</version>")
    string(REGEX MATCH ${field_version_regex} field_version "${field_arch}")

    if(NOT field_version)
        set(rtiversion_error TRUE PARENT_SCOPE)
        string(CONCAT
            error
            "The version tag was not extracted from ${field_name} and "
            "${CONNEXTDDS_ARCH} under ${CONNEXTDDS_DIR}")
        return()
    endif()

    # Extract the version string from the tag
    set(field_version_number_regex "[0-9]+\\.[0-9]+\\.[0-9]+(\\.[0-9]+(\\.[0-9]+)?)?")
    string(REGEX MATCH
        ${field_version_number_regex}
        field_version_number
        ${field_version})

    if(NOT field_version_number)
        set(rtiversion_error TRUE PARENT_SCOPE)
        string(CONCAT
            error
            "The version number was not extracted from ${field_name} and "
            "${CONNEXTDDS_ARCH} under ${CONNEXTDDS_DIR}")
        return()
    endif()

    if(NOT DEFINED RTICONNEXTDDS_VERSION)
        # If the variable version RTICONNEXTDDS_VERSION has not been set, we
        # set it here in the scope of the caller to store the value that has
        # been detected.
        set(RTICONNEXTDDS_VERSION ${field_version_number} PARENT_SCOPE)
    elseif(NOT RTICONNEXTDDS_VERSION STREQUAL field_version_number)
        # Otherwise, if the detected version is inconsistent with the previous
        # value of RTICONNEXTDDS_VERSION, we throw an error.
        string(CONCAT
            error
            "The version of ${field} is ${field_version_number}, which "
            "is incosistent with the expected version... "
            "${RTICONNEXTDDS_VERSION}")
        message(WARNING ${error})
        set(rtiversion_error TRUE PARENT_SCOPE)
    endif()
endfunction()

#####################################################################
# Platform-specific Definitions                                     #
#####################################################################

set(connextdds_host_arch "java")

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
    # connextdds_host_arch is usually a prefix of the CONNEXTDDS_ARCH (e.g.,
    # "x64Linux" is a prefix  of "x64Linux3gcc5.4.0"). However, in the case
    # of Darwin, connextdds_host_arch is simply "darwin".
    # To be able to match the CONNEXTDDS_ARCH in some parts of the script,
    # we need to be able to consider connextdds_host_arch with both names
    # "darwin" and the more natural "x64Darwin". Consequently, we assign a
    # list of names to CONNEXT_HOST_ARCH in the case of darwin.
    set(connextdds_host_arch ${connextdds_host_arch} "darwin" "x64Darwin")
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86")
        set(connextdds_host_arch ${connextdds_host_arch} "i86Win32")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "AMD64")
        set(connextdds_host_arch ${connextdds_host_arch} "x64Win64")
    else()
        message(FATAL_ERROR
            "${CMAKE_HOST_SYSTEM} is not supported as host architecture")
    endif()
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
        set(connextdds_host_arch ${connextdds_host_arch} "x64Linux")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86")
        set(connextdds_host_arch ${connextdds_host_arch} "i86Linux")
    else()
        message(FATAL_ERROR
            "${CMAKE_HOST_SYSTEM} is not supported as host architecture")
    endif()
else()
    message(FATAL_ERROR
        "${CMAKE_HOST_SYSTEM} is not supported as host architecture")
endif()

# Platform-Specific Definitions
if(CONNEXTDDS_ARCH MATCHES "Linux")
    # Linux Platforms
    set(CONNEXTDDS_EXTERNAL_LIBS -ldl -lnsl -lm -lpthread -lrt)
    set(CONNEXTDDS_DEFINITIONS "-DRTI_UNIX" "-DRTI_LINUX")

    if(CONNEXTDDS_ARCH MATCHES "i86Linux")
        set(CONNEXTDDS_DEFINITIONS ${CONNEXTDDS_DEFINITIONS})
    elseif(CONNEXTDDS_ARCH MATCHES "x64Linux")
        set(CONNEXTDDS_DEFINITIONS ${CONNEXTDDS_DEFINITIONS} -DRTI_64BIT)
    elseif(CONNEXTDDS_ARCH MATCHES "armv6vfph")
        set(CONNEXTDDS_DEFINITIONS ${CONNEXTDDS_DEFINITIONS})
    else()
        message(FATAL_ERROR
            "${CONNEXTDDS_ARCH} architecture is unsupported by this module")
    endif()
elseif(CONNEXTDDS_ARCH MATCHES "Win")
    # Windows Platforms
    set(CONNEXTDDS_EXTERNAL_LIBS ws2_32 netapi32 version)

    LIST(APPEND CONNEXTDDS_DEFINITIONS
            "-DWIN32_LEAN_AND_MEAN"
            "-DWIN32"
            "-D_WINDOWS"
            "-DRTI_WIN32"
            "-D_BIND_TO_CURRENT_MFC_VERSION=1"
            "-D_BIND_TO_CURRENT_CRT_VERSION=1"
            "-D_CRT_SECURE_NO_WARNING")

    # When building against ConnextDDS's shared libraries, users need to also
    # add the CONNEXTDDS_DLL_EXPORT_MACRO to their definitions.
    string(CONCAT CONNEXTDDS_DLL_EXPORT_MACRO
            "-DNDDS_DLL_VARIABLE")

elseif(CONNEXTDDS_ARCH MATCHES "Darwin")
    # Darwin Platforms
    set(CONNEXTDDS_EXTERNAL_LIBS "")

    LIST(APPEND CONNEXTDDS_DEFINITIONS
            "-DRTI_UNIX"
            "-DRTI_DARWIN"
            "-DRTI_DARWIN10"
            "-DRTI_64BIT")
elseif(CONNEXTDDS_ARCH MATCHES "Android")
    set(CONNEXTDDS_EXTERNAL_LIBS -llog -lc -lm)
    set(CONNEXTDDS_DEFINITIONS -DRTI_UNIX -DLINUX -DRTI_ANDROID)
else()
    message(FATAL_ERROR
        "${CONNEXTDDS_ARCH} architecture is unsupported by this module")
endif()


#####################################################################
# Core Component Variables                                          #
#####################################################################

# This script verifies the version of each of the following fields in
# ${CONNEXTDDS_DIR}/rti_versions.xml, as they are the basic components of the
# RTI Connext DDS core libraries, which are always part of an installation.
list(APPEND rti_versions_field_names_host
        "header_files"
        "host_files"
        "core_release_docs"
        "core_api_docs"
        "core_jars"
        "target_libraries")

list(APPEND rti_versions_field_names_target
        "target_libraries")

# Define CONNEXTDDS_INCLUDE_DIRS
find_path(CONNEXTDDS_INCLUDE_DIRS
        NAMES ndds_c.h
        PATHS "${CONNEXTDDS_DIR}/include/ndds")
set(CONNEXTDDS_INCLUDE_DIRS
        "${CONNEXTDDS_DIR}/include"
        ${CONNEXTDDS_INCLUDE_DIRS}
        "${CONNEXTDDS_DIR}/include/ndds/hpp")

# Find all flavors of libnddscore
find_library(libnddscore_release_static
        NAMES nddscorez
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
find_library(libnddscore_release_shared
        NAMES nddscore
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
find_library(libnddscore_debug_static
        NAMES nddscorezd
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
find_library(libnddscore_debug_shared
        NAMES nddscored
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")

# Find all flavors of libnddsc
find_library(libnddsc_release_static
        NAMES nddscz
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
find_library(libnddsc_release_shared
        NAMES nddsc
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
find_library(libnddsc_debug_static
        NAMES nddsczd
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
find_library(libnddsc_debug_shared
        NAMES nddscd
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")

# Find all flavors of libnddscpp
find_library(libnddscpp_release_static
        NAMES nddscppz
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
find_library(libnddscpp_release_shared
        NAMES nddscpp
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
find_library(libnddscpp_debug_static
        NAMES nddscppzd
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
find_library(libnddscpp_debug_shared
        NAMES nddscppd
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")

# Find all flavors of libnddscpp2
find_library(libnddscpp2_release_static
        NAMES nddscpp2z
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
find_library(libnddscpp2_release_shared
        NAMES nddscpp2
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
find_library(libnddscpp2_debug_static
        NAMES nddscpp2zd
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
find_library(libnddscpp2_debug_shared
        NAMES nddscpp2d
        PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")

# Define CONNEXTDDS_C_API
set(CONNEXTDDS_C_API_LIBRARIES_RELEASE_STATIC
        ${libnddsc_release_static}
        ${libnddscore_release_static})
set(CONNEXTDDS_C_API_LIBRARIES_RELEASE_SHARED
        ${libnddsc_release_shared}
        ${libnddscore_release_shared})
set(CONNEXTDDS_C_API_LIBRARIES_DEBUG_STATIC
        ${libnddsc_debug_static}
        ${libnddscore_debug_static})
set(CONNEXTDDS_C_API_LIBRARIES_DEBUG_SHARED
        ${libnddsc_debug_shared}
        ${libnddscore_debug_shared})

# Define CONNEXTDDS_CPP_API
set(CONNEXTDDS_CPP_API_LIBRARIES_RELEASE_STATIC
        ${libnddscpp_release_static}
        ${CONNEXTDDS_C_API_LIBRARIES_RELEASE_STATIC})
set(CONNEXTDDS_CPP_API_LIBRARIES_RELEASE_SHARED
        ${libnddscpp_release_shared}
        ${CONNEXTDDS_C_API_LIBRARIES_RELEASE_SHARED})
set(CONNEXTDDS_CPP_API_LIBRARIES_DEBUG_STATIC
        ${libnddscpp_debug_static}
        ${CONNEXTDDS_C_API_LIBRARIES_DEBUG_STATIC})
set(CONNEXTDDS_CPP_API_LIBRARIES_DEBUG_SHARED
        ${libnddscpp_debug_shared}
        ${CONNEXTDDS_C_API_LIBRARIES_DEBUG_SHARED})

# Define CONNEXTDDS_CPP2_API
set(CONNEXTDDS_CPP2_API_LIBRARIES_RELEASE_STATIC
        ${libnddscpp2_release_static}
        ${CONNEXTDDS_C_API_LIBRARIES_RELEASE_STATIC})

set(CONNEXTDDS_CPP2_API_LIBRARIES_RELEASE_SHARED
        ${libnddscpp2_release_shared}
        ${CONNEXTDDS_C_API_LIBRARIES_RELEASE_SHARED})

set(CONNEXTDDS_CPP2_API_LIBRARIES_DEBUG_STATIC
        ${libnddscpp2_debug_static}
        ${CONNEXTDDS_C_API_LIBRARIES_DEBUG_STATIC})

set(CONNEXTDDS_CPP2_API_LIBRARIES_DEBUG_SHARED
        ${libnddscpp2_debug_shared}
        ${CONNEXTDDS_C_API_LIBRARIES_DEBUG_SHARED})

#####################################################################
# Distributed Logger Component Variables                                 #
#####################################################################
if(distributed_logger IN_LIST RTIConnextDDS_FIND_COMPONENTS)

    # Find all flavors of rtidlc
    find_library(librtidlc_release_static
            NAMES rtidlcz
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtidlc_release_shared
            NAMES rtidlc
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtidlc_debug_static
            NAMES rtidlczd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtidlc_debug_shared
            NAMES rtidlcd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")

    # Find all flavors of rtidlcpp
    find_library(librtidlcpp_release_static
            NAMES rtidlcppz
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtidlcpp_release_shared
            NAMES rtidlcpp
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtidlcpp_debug_static
            NAMES rtidlcppzd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtidlcpp_debug_shared
            NAMES rtidlcppd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")

    # Libraries for Distributed Logger for C
    set(DISTRIBUTED_LOGGER_C_LIBRARIES_RELEASE_STATIC
            ${librtidlc_release_static})
    set(DISTRIBUTED_LOGGER_C_LIBRARIES_RELEASE_SHARED
            ${librtidlc_release_shared})
    set(DISTRIBUTED_LOGGER_C_LIBRARIES_DEBUG_STATIC
            ${librtidlc_debug_static})
    set(DISTRIBUTED_LOGGER_C_LIBRARIES_DEBUG_SHARED
            ${librtidlc_debug_shared})

    # Libraries for Distributed Logger for C++
    set(DISTRIBUTED_LOGGER_CPP_LIBRARIES_RELEASE_STATIC
            ${librtidlcpp_release_static})
    set(DISTRIBUTED_LOGGER_CPP_LIBRARIES_RELEASE_SHARED
            ${librtidlcpp_release_shared})
    set(DISTRIBUTED_LOGGER_CPP_LIBRARIES_DEBUG_STATIC
            ${librtidlcpp_debug_static})
    set(DISTRIBUTED_LOGGER_CPP_LIBRARIES_DEBUG_SHARED
            ${librtidlcpp_debug_shared})

    if (CMAKE_SYSTEM_NAME MATCHES "Linux")
        set(CONNEXTDDS_EXTERNAL_LIBS "-Wl,--no-as-needed  ${CONNEXTDDS_EXTERNAL_LIBS}")
    endif()
endif()

#####################################################################
# Routing Service Component Variables                               #
#####################################################################
if(routing_service IN_LIST RTIConnextDDS_FIND_COMPONENTS)
    # Add fields associated with the routing_service component
    list(APPEND rti_versions_field_names_host
            "routing_service_host"
            "routing_service_sdk_jars")
    list(APPEND rti_versions_field_names_target
            "routing_service"
            "routing_service_sdk")

    # Find all flavors of librtiroutingservice
    find_library(librtiroutingservice_release_static
            NAMES rtiroutingservicez
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtiroutingservice_release_shared
            NAMES rtiroutingservice
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtiroutingservice_debug_static
            NAMES rtiroutingservicezd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtiroutingservice_debug_shared
            NAMES rtiroutingserviced
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")

    # Find all flavors of librtirsinfrastructure
    find_library(librtirsinfrastructure_release_static
            NAMES rtirsinfrastructurez
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtirsinfrastructure_release_shared
            NAMES rtirsinfrastructure
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtirsinfrastructure_debug_static
            NAMES rtirsinfrastructurezd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtirsinfrastructure_debug_shared
            NAMES rtirsinfrastructured
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")

    # Libraries for Routing Service
    set(ROUTING_SERVICE_API_LIBRARIES_RELEASE_STATIC
            ${librtiroutingservice_release_static}
            ${librtirsinfrastructure_release_static})
    set(ROUTING_SERVICE_API_LIBRARIES_RELEASE_SHARED
            ${librtiroutingservice_release_shared}
            ${librtirsinfrastructure_release_shared})
    set(ROUTING_SERVICE_API_LIBRARIES_DEBUG_STATIC
            ${librtiroutingservice_debug_static}
            ${librtirsinfrastructure_debug_static})
    set(ROUTING_SERVICE_API_LIBRARIES_DEBUG_SHARED
            ${librtiroutingservice_debug_shared}
            ${librtirsinfrastructure_debug_shared})
endif()

#####################################################################
# Messaging Component Variables                                 #
#####################################################################
if(messaging_api IN_LIST RTIConnextDDS_FIND_COMPONENTS)
    list(APPEND rti_versions_field_names_host
            "request_reply_host")
    list(APPEND rti_versions_field_names_target
            "request_reply")

    # Find all flavors of librticonnextmsgc
    find_library(librticonnextmsgc_release_static
            NAMES rticonnextmsgcz
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librticonnextmsgc_release_shared
            NAMES rticonnextmsgc
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librticonnextmsgc_debug_static
            NAMES rticonnextmsgczd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librticonnextmsgc_debug_shared
            NAMES rticonnextmsgcd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")

    # Find all flavors of librticonnextmsgcpp
    find_library(librticonnextmsgcpp_release_static
            NAMES rticonnextmsgcppz
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librticonnextmsgcpp_release_shared
            NAMES rticonnextmsgcpp
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librticonnextmsgcpp_debug_static
            NAMES rticonnextmsgcppzd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librticonnextmsgcpp_debug_shared
            NAMES rticonnextmsgcppd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")

    # Find all flavors of librticonnextmsgcpp2
    find_library(librticonnextmsgcpp2_release_static
            NAMES rticonnextmsgcpp2z
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librticonnextmsgcpp2_release_shared
            NAMES rticonnextmsgcpp2
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librticonnextmsgcpp2_debug_static
            NAMES rticonnextmsgcpp2zd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librticonnextmsgcpp2_debug_shared
            NAMES rticonnextmsgcpp2d
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")

    # Libraries for Messaging for C
    set(MESSAGING_C_API_LIBRARIES_RELEASE_STATIC
            ${librticonnextmsgc_release_static})
    set(MESSAGING_C_API_LIBRARIES_RELEASE_SHARED
            ${librticonnextmsgc_release_shared})
    set(MESSAGING_C_API_LIBRARIES_DEBUG_STATIC
            ${librticonnextmsgc_debug_static})
    set(MESSAGING_C_API_LIBRARIES_DEBUG_SHARED
            ${librticonnextmsgc_debug_shared})

    # Libraries for Messaging for C++
    set(MESSAGING_CPP_API_LIBRARIES_RELEASE_STATIC
            ${librticonnextmsgcpp_release_static})
    set(MESSAGING_CPP_API_LIBRARIES_RELEASE_SHARED
            ${librticonnextmsgcpp_release_shared})
    set(MESSAGING_CPP_API_LIBRARIES_DEBUG_STATIC
            ${librticonnextmsgcpp_debug_static})
    set(MESSAGING_CPP_API_LIBRARIES_DEBUG_SHARED
            ${librticonnextmsgcpp_debug_shared})

    # Libraries for Messaging for Modern C++
    set(MESSAGING_CPP2_API_LIBRARIES_RELEASE_STATIC
            ${librticonnextmsgcpp2_release_static})
    set(MESSAGING_CPP2_API_LIBRARIES_RELEASE_SHARED
            ${librticonnextmsgcpp2_release_shared})
    set(MESSAGING_CPP2_API_LIBRARIES_DEBUG_STATIC
            ${librticonnextmsgcpp2_debug_static})
    set(MESSAGING_CPP2_API_LIBRARIES_DEBUG_SHARED
            ${librticonnextmsgcpp2_debug_shared})
endif()

#####################################################################
# Security Plugins Component Variables                              #
#####################################################################
if(security_plugins IN_LIST RTIConnextDDS_FIND_COMPONENTS)
    list(APPEND rti_versions_field_names_host
            "secure_base"
            "secure_host")

    list(APPEND rti_versions_field_names_target
        "secure_target_libraries")

    if(DEFINED CONNEXTDDS_OPENSSL_DIR)
        find_package(OpenSSL
                REQUIRED ${CONNEXTDDS_OPENSSL_VERSION}
                PATHS "${CONNEXTDDS_OPENSSL_DIR}")
    elseif(DEFINED CONNEXTDDS_OPENSSL_VERSION)
        find_package(OpenSSL REQUIRED ${CONNEXTDDS_OPENSSL_VERSION})
    else()
        find_package(OpenSSL REQUIRED)
    endif()

    # Add OpenSSL include directories to the list of CONNEXTDDS_INCLUDE_DIRS
    set(CONNEXTDDS_INCLUDE_DIRS
            "${OPENSSL_INCLUDE_DIR}"
            ${CONNEXTDDS_INCLUDE_DIRS})

    # Add OpenSSL libraries to the list of CONNEXTDDS_EXTERNAL_LIBS
    set(CONNEXTDDS_EXTERNAL_LIBS
            ${OPENSSL_SSL_LIBRARY}
            ${OPENSSL_CRYPTO_LIBRARY}
            ${CONNEXTDDS_EXTERNAL_LIBS})

    # Find all flavors of libnddssecurity
    find_library(libnddssecurity_release_static
                NAMES nddssecurityz
                PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(libnddssecurity_release_shared
                NAMES nddssecurity
                PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(libnddssecurity_debug_static
                NAMES nddssecurityzd
                PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(libnddssecurity_debug_shared
                NAMES nddssecurityd
                PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")

    # Libraries for the Security Plugins
    set(SECURITY_PLUGINS_LIBRARIES_RELEASE_STATIC
            ${libnddssecurity_release_static})
    set(SECURITY_PLUGINS_LIBRARIES_RELEASE_SHARED
            ${libnddssecurity_release_shared})
    set(SECURITY_PLUGINS_LIBRARIES_DEBUG_STATIC
            ${libnddssecurity_debug_static})
    set(SECURITY_PLUGINS_LIBRARIES_DEBUG_SHARED
            ${libnddssecurity_debug_shared})
endif()

#####################################################################
# Monitoring Liraries Component Variables                           #
#####################################################################
if(monitoring_libraries IN_LIST RTIConnextDDS_FIND_COMPONENTS)
    # Find all flavors of librtimonitoring
    find_library(librtimonitoring_release_static
            NAMES rtimonitoringz
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtimonitoring_release_shared
            NAMES rtimonitoring
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtimonitoring_debug_static
            NAMES rtimonitoringzd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(librtimonitoring_debug_shared
            NAMES rtimonitoringd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")

    # Libraries for the Monitoring Libraries
    set(MONITORING_LIBRARIES_RELEASE_STATIC
            ${librtimonitoring_release_static})
    set(MONITORING_LIBRARIES_RELEASE_SHARED
            ${librtimonitoring_release_shared})
    set(MONITORING_LIBRARIES_DEBUG_STATIC
            ${librtimonitoring_debug_static})
    set(MONITORING_LIBRARIES_DEBUG_SHARED
            ${librtimonitoring_debug_shared})

    if(CONNEXTDDS_ARCH MATCHES "Win")
        set(CONNEXTDDS_EXTERNAL_LIBS psapi ${CONNEXTDDS_EXTERNAL_LIBS})
    endif()
endif()

if(zero_copy IN_LIST RTIConnextDDS_FIND_COMPONENTS)
    find_library(ZERO_COPY_LIBRARIES_RELEASE_STATIC
            NAMES nddsmetpz
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(ZERO_COPY_LIBRARIES_RELEASE_SHARED
            NAMES nddsmetp
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(ZERO_COPY_LIBRARIES_DEBUG_STATIC
            NAMES nddsmetpzd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
    find_library(ZERO_COPY_LIBRARIES_DEBUG_SHARED
            NAMES nddsmetpd
            PATHS "${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}")
endif()

#####################################################################
# Version Variables                                                 #
#####################################################################
# Verify that all the components specified have the same version
file(READ "${CONNEXTDDS_DIR}/rti_versions.xml" xml_file)

# Verify host components
foreach(field IN LISTS rti_versions_field_names_host)
    connextdds_check_component_field_version(
            ${field}
            ${xml_file}
            connextdds_host_arch)
endforeach()

# Verify target components
foreach(field IN LISTS rti_versions_field_names_target)
    connextdds_check_component_field_version(
            ${field}
            ${xml_file}
            CONNEXTDDS_ARCH)
endforeach()

if(NOT rtiversion_error)
    set(RTICONNEXTDDS_VERSION_OK TRUE)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RTIConnextDDS
        REQUIRED_VARS
            CONNEXTDDS_DIR
            RTICONNEXTDDS_VERSION
            RTICONNEXTDDS_VERSION_OK
        VERSION_VAR
            RTICONNEXTDDS_VERSION)
