FIND_PACKAGE(RTIConnextDDS 5.3.1 REQUIRED)
FIND_PACKAGE(RTICodeGenerator REQUIRED)


FUNCTION(GET_DDS_INFO INCLUDE_DIRS LINK_LIBS COMPILE_DEFINITIONS)
	SET(${INCLUDE_DIRS} ${CONNEXTDDS_INCLUDE_DIRS} PARENT_SCOPE)
	IF (${CMAKE_BUILD_TYPE} MATCHES "[Rr]elease")
    	SET(${LINK_LIBS} 
    		${CONNEXTDDS_CPP_API_LIBRARIES_RELEASE_STATIC} 
    		${CONNEXTDDS_EXTERNAL_LIBS} PARENT_SCOPE)
	ELSE ()
	    SET(${LINK_LIBS} 
	    	${CONNEXTDDS_CPP_API_LIBRARIES_DEBUG_STATIC}
	    	${CONNEXTDDS_EXTERNAL_LIBS} PARENT_SCOPE)
	ENDIF ()
	SET(${COMPILE_DEFINITIONS} ${CONNEXTDDS_DEFINITIONS} PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(GET_DDS_GEN_FILES basename ret)
	SET(${ret} ${basename}.cxx ${basename}Plugin.cxx ${basename}Support.cxx ${basename}.h ${basename}Plugin.h ${basename}Support.h PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(GET_DDS_GEN_COMMAND file output_dir cmd)
	SET(${cmd} 					${CMAKE_COMMAND} -E env JREHOME=$ENV{JREHOME} ${RTICODEGEN}
                                -verbosity 1 -namespace -language C++
                                -replace
                                -d ${output_dir} ${file}
               PARENT_SCOPE)
ENDFUNCTION()