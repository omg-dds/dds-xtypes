# This module will set the following variables:
# CONNEXTDDSME_CPP_API_LIBRARIES: link libraries for ConnextDDS Micro C++'s API.
# CONNEXTDDSME_INCLUDE_DIRS: include directories for ConnextDDS Micro

# You must tell this module where micro is installed by 
# passing -DCONNEXTDDSME_DIR=<where micro is installed>
# This module will try to detect the architecture. 
# To specify a architecture, pass -DCONNEXTDDSME_ARCH=<arch-os-compiler-triple>
# Codegenerator requires Java

FIND_PACKAGE(Java REQUIRED COMPONENTS Runtime)
FIND_PACKAGE(Threads REQUIRED)

IF (NOT DEFINED CONNEXTDDSME_DIR)
	IF (NOT DEFINED $ENV{CONNEXTDDSME_DIR})
		STRING(CONCAT ERR_MSG 
					"Please tell me where micro is installed by passing -DCONNEXTDDSME_DIR=<install_dir>"
					" or set the environment variable CONNEXTDDSME_DIR")
		MESSAGE(FATAL_ERROR ${ERR_MSG})
	ELSE()
		SET(CONNEXTDDSME_DIR $ENV{CONNEXTDDSME_DIR})
	ENDIF()
ENDIF()

IF (NOT EXISTS ${CONNEXTDDSME_DIR})
    MESSAGE(FATAL_ERROR "Directory ${CONNEXTDDSME_DIR} does not exists. Try using absolute path.")
ENDIF()

MESSAGE(STATUS "RTI Connext DDS Micro installation directory... ${CONNEXTDDSME_DIR}")
IF (NOT DEFINED CONNEXTDDSME_ARCH)
	IF(DEFINED CONNEXTDDS_ARCH)
		SET(CONNEXTDDSME_ARCH ${CONNEXTDDS_ARCH})
	ELSEIF(DEFINED $ENV{CONNEXTDDS_ARCH})
        MESSAGE("")
		SET(CONNEXTDDSME_ARCH $ENV{CONNEXTDDS_ARCH})
	ENDIF()
ENDIF()

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

IF (NOT DEFINED CONNEXTDDSME_ARCH)
    SUBDIRLIST(architectures_installed ${CONNEXTDDSME_DIR}/lib)
    LIST(GET architectures_installed 0 architecture_name)
    IF(${architecture_name} MATCHES "java")
    	LIST(GET architectures_installed 1 architecture_name)
    ENDIF()

    IF(NOT architecture_name)
        STRING(CONCAT
            error
            "Could not find architecture to build against. "
            "Please set -DCONNEXTDDSME_ARCH= or -DCONNEXTDDS_ARCH= to specify your RTI "
            "Connext DDS architecture.")
        MESSAGE(FATAL_ERROR ${error})
    ENDIF()
    SET(CONNEXTDDSME_ARCH ${architecture_name})
ENDIF()

MESSAGE(STATUS "RTI Connext DDS Micro architecture... ${CONNEXTDDSME_ARCH}")


IF(NOT EXISTS "${CONNEXTDDSME_DIR}/lib/${CONNEXTDDSME_ARCH}")
    MESSAGE(FATAL_ERROR "Could not find library directory under architecture ${CONNEXTDDSME_ARCH}")
ENDIF()


MACRO(FIND_RTI_LIBRARY LIB_NAME)
    FIND_LIBRARY(${LIB_NAME}_release_static
            NAMES "${LIB_NAME}z"
            PATHS "${CONNEXTDDSME_DIR}/lib/${CONNEXTDDSME_ARCH}")
    FIND_LIBRARY(${LIB_NAME}_release_shared
            NAMES "${LIB_NAME}"
            PATHS "${CONNEXTDDSME_DIR}/lib/${CONNEXTDDSME_ARCH}")
    FIND_LIBRARY(${LIB_NAME}_debug_static
            NAMES "${LIB_NAME}zd"
            PATHS "${CONNEXTDDSME_DIR}/lib/${CONNEXTDDSME_ARCH}")
    FIND_LIBRARY(${LIB_NAME}_debug_shared
            NAMES "${LIB_NAME}d"
            PATHS "${CONNEXTDDSME_DIR}/lib/${CONNEXTDDSME_ARCH}")
ENDMACRO()



FIND_RTI_LIBRARY(rti_me_cpp)
FIND_RTI_LIBRARY(rti_me_rhsm)
FIND_RTI_LIBRARY(rti_me_whsm)
FIND_RTI_LIBRARY(rti_me_discdpse)
FIND_RTI_LIBRARY(rti_me_discdpde)
IF("security_plugins" IN_LIST ConnextDDSMicro_FIND_COMPONENTS)
    FIND_RTI_LIBRARY(rti_me_seccore)
    FIND_RTI_LIBRARY(rti_me_ddssecurity)
    FIND_PACKAGE(OpenSSL REQUIRED)
    FIND_PACKAGE(LibXml2 REQUIRED)
ENDIF()
FIND_RTI_LIBRARY(rti_me)
IF("shared_memory" IN_LIST ConnextDDSMicro_FIND_COMPONENTS)
    FIND_RTI_LIBRARY(rti_me_netioshmem)
ENDIF()

SET(CONNEXTDDSME_INCLUDE_DIRS 
    ${CONNEXTDDSME_DIR}/include 
    ${CONNEXTDDSME_DIR}/include/rti_me 
    ${OPENSSL_INCLUDE_DIR}
    ${LIBXML2_INCLUDE_DIR})


SET(CONNEXTDDSME_CPP_API_LIBRARIES_RELEASE_STATIC 
        ${rti_me_cpp_release_static}
        ${rti_me_rhsm_release_static}
        ${rti_me_whsm_release_static}
        ${rti_me_discdpse_release_static}
        ${rti_me_discdpde_release_static}
        ${rti_me_release_static}
        ${rti_me_netioshmem_release_static})

SET(CONNEXTDDSME_CPP_API_LIBRARIES_RELEASE_SHARED
        ${rti_me_cpp_release_shared}
        ${rti_me_rhsm_release_shared}
        ${rti_me_whsm_release_shared}
        ${rti_me_discdpse_release_shared}
        ${rti_me_discdpde_release_shared}
        ${rti_me_release_shared}
        ${rti_me_netioshmem_release_shared})

SET(CONNEXTDDSME_CPP_API_LIBRARIES_DEBUG_STATIC
        ${rti_me_cpp_debug_static}
        ${rti_me_rhsm_debug_static}
        ${rti_me_whsm_debug_static}
        ${rti_me_discdpse_debug_static}
        ${rti_me_discdpde_debug_static}
        ${rti_me_debug_static}
        ${rti_me_netioshmem_debug_static})

SET(CONNEXTDDSME_CPP_API_LIBRARIES_DEBUG_SHARED
        ${rti_me_cpp_debug_shared} 
        ${rti_me_rhsm_debug_shared}
        ${rti_me_whsm_debug_shared}
        ${rti_me_discdpse_debug_shared}
        ${rti_me_discdpde_debug_shared}
        ${rti_me_debug_shared}
        ${rti_me_netioshmem_debug_shared})

IF("security_plugins" IN_LIST ConnextDDSMicro_FIND_COMPONENTS)
    SET(SECURITY_LIBRARIES_RELEASE_STATIC ${rti_me_seccore_release_static} ${rti_me_ddssecurity_release_static})
    SET(SECURITY_LIBRARIES_RELEASE_SHARED ${rti_me_seccore_release_shared} ${rti_me_ddssecurity_release_shared})
    SET(SECURITY_LIBRARIES_DEBUG_STATIC ${rti_me_seccore_debug_static} ${rti_me_ddssecurity_debug_static})
    SET(SECURITY_LIBRARIES_DEBUG_SHARED ${rti_me_seccore_debug_shared} ${rti_me_ddssecurity_debug_shared})
ENDIF()


IF (${CMAKE_BUILD_TYPE} MATCHES "[Rr]elease")
    SET(CONNEXTDDSME_CPP_API_LIBRARIES_STATIC ${CONNEXTDDSME_CPP_API_LIBRARIES_RELEASE_STATIC})
    SET(CONNEXTDDSME_CPP_API_LIBRARIES_SHARED ${CONNEXTDDSME_CPP_API_LIBRARIES_RELEASE_SHARED})
    SET(SECURITY_PLUGIN_LIBRARIES_STATIC ${SECURITY_LIBRARIES_RELEASE_STATIC})
    SET(SECURITY_PLUGIN_LIBRARIES_SHARED ${SECURITY_LIBRARIES_RELEASE_SHARED})
    SET(SHARED_MEMORY_LIBRARIES_STATIC ${rti_me_netioshmem_release_static})
    SET(SHARED_MEMORY_LIBRARIES_SHARED ${rti_me_netioshmem_release_shared})
ELSE()
    SET(CONNEXTDDSME_CPP_API_LIBRARIES_STATIC ${CONNEXTDDSME_CPP_API_LIBRARIES_DEBUG_STATIC})
    SET(CONNEXTDDSME_CPP_API_LIBRARIES_SHARED ${CONNEXTDDSME_CPP_API_LIBRARIES_DEBUG_SHARED})
    SET(SECURITY_PLUGIN_LIBRARIES_STATIC ${SECURITY_LIBRARIES_DEBUG_STATIC})
    SET(SECURITY_PLUGIN_LIBRARIES_SHARED ${SECURITY_LIBRARIES_DEBUG_SHARED})
    SET(SHARED_MEMORY_LIBRARIES_STATIC ${rti_me_netioshmem_debug_static})
    SET(SHARED_MEMORY_LIBRARIES_SHARED ${rti_me_netioshmem_debug_shared})
ENDIF()

SET(CONNEXTDDSME_CPP_API_LIBRARIES ${CONNEXTDDSME_CPP_API_LIBRARIES_STATIC} Threads::Threads)

SET(SECURITY_PLUGIN_LIBRARIES ${SECURITY_PLUGIN_LIBRARIES_STATIC} ${OPENSSL_LIBRARIES} ${LIBXML2_LIBRARIES})
SET(SHARED_MEMORY_LIBRARIES ${SHARED_MEMORY_LIBRARIES_STATIC})


SET(RTIMECODEGEN ${CONNEXTDDSME_DIR}/rtiddsgen/scripts/rtiddsgen)

IF(NOT EXISTS ${RTIMECODEGEN})
    MESSAGE(FATAL_ERROR "codegen not found in ${CONNEXTDDSME_DIR}")
ENDIF()

IF("${CONNEXTDDSME_CPP_API_LIBRARIES}" MATCHES "NOTFOUND")
    FOREACH(LIB ${CONNEXTDDSME_CPP_API_LIBRARIES})
        IF(LIB MATCHES "NOTFOUND")
            MESSAGE(WARNING LIB)
        ENDIF()
    ENDFOREACH()
    MESSAGE(FATAL_ERROR "Could not find required libraries for architectures ${CONNEXTDDSME_ARCH}")
ENDIF()