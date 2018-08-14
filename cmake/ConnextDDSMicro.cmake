FIND_PACKAGE(ConnextDDSMicro)

FUNCTION(GET_DDS_INFO INCLUDE_DIRS LINK_LIBS COMPILE_DEFINITIONS)
	SET(${INCLUDE_DIRS} ${CONNEXTDDSME_INCLUDE_DIRS} PARENT_SCOPE)
	SET(${LINK_LIBS} ${CONNEXTDDSME_CPP_API_LIBRARIES} PARENT_SCOPE)
	# SET(${COMPILE_DEFINITIONS} ${CONNEXTDDS_DEFINITIONS} PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(GET_DDS_GEN_FILES basename ret)
	SET(${ret} ${basename}.cxx ${basename}Plugin.cxx ${basename}Support.cxx ${basename}.h ${basename}Plugin.h ${basename}Support.h PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(GET_DDS_GEN_COMMAND file output_dir cmd)
	SET(${cmd} 					${CMAKE_COMMAND} -E env JREHOME=$ENV{JREHOME} ${RTIMECODEGEN}
                                -verbosity 1 -namespace -language C++
                                -micro -replace
                                -d ${output_dir} ${file}
               PARENT_SCOPE)
ENDFUNCTION()