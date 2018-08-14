# xtypes-interop
## What is xtype-interop
`xtype-interop` is an interoperability test between DDS applications, with a primary focus on testing [xtype](https://www.omg.org/spec/DDS-XTypes/About-DDS-XTypes/) features.

This repository contains all the feature tests that shall be implemented for the following DDS-XTYPES 1.2 capabilities:

  * DataRepresentationQosPolicy matching rules
  * Type compatibility rules
  * DataRepresentation serialization/deserialization rules

These tests can be used to validate compliance with OMG DDS-XTYPES 1.2 of each DDS implementation separately. The same tests can also be used to test interoperability between different DDS implementations.

`xtype-interop` has 4 modules, and every module might contain dozens of tests. More detailed README about each module is available in each module's subdirectory.

* `TestHelloWorld` Simple Hello World test that just test if a pair of Reader/Writer can discover each other.
* `TestDataRepresentationQos` Tests about compatibility between different Data Representation Qos Policies (XCDR and XCDR2).
* `TestExtensibility` Tests whether readers and writers with different types can discover each other. "Different" means that members of a type might be added, removed and/or shuffled. This tests xtype features.
* `TestSerialization` Tests whether data is serialized correctly across different types.

## How does it work
`xtype-interop` contains a **single master process** that's shared by all modules. But each module has its own slave process that's only shared in that module. 

Since the master process and slave processes are from different executables, it is possible to build them with different DDS implementations. Therefore testing interoperability between different DDS implementations.

When running tests of a particular module, the master process will spawn launch the corresponding slave process and tell the slave process which test is currently running. The master process will create a `DataReader` and the slave process will create a `DataWriter`. The master process will determine if the `DataReader/DataWriter` pair can discover each other, or timeout after a while.

Since each module can contain any number of tests and all these tests share 1 slave process, the slave process must be aware of which test is currently running and take respective actions. Communication between master and slave processes is achieved via DDS. Both master and slave process will create a `TestController` object that handles this communication.

## Source Tree Structure

Every module is in its own subdirectory in the repository. Module `Foo` will be in `src/TestFoo` subdirectory. The code for master process of a module is in `src/TestFoo/TestFoo.cxx` while the code for slave process is in `src/TestFoo/TestFooHelper.cxx`. Since there is only 1 master process, each `TestXXX.cxx` file contains no main function. Instead, each `TestXXX.cxx` file only contains test code, while `src/main.cxx` contains code to run start running these tests.

* [src](src) contains common source files shared among all modules. Such as TestController.
	* [TestController.cxx](src/TestController.cxx) contains code responsible for communication between the master and slave process. This is done via DDS.
	* [Utils.cxx](src/Utils.cxx) contains code for initializing and destroying DDS entities.

### To build the project

##### Instructions to Get [Google Test](https://github.com/google/googletest)
* Clone google test repo and `cd` into the repo:

```	
git clone https://github.com/google/googletest.git
cd googletest
```
* Generate build systems

```
cmake -G "Unix Makefiles" .
```
or, if you want to install google test in a directory other than `/usr`:
	
```
cmake -G "Unix Makefiles" . -DCMAKE_INSTALL_PREFIX=<desired install dir>
```
* Build & install google test

```
make
sudo make install
```

##### Build the project
###### Generate build system
Clone the project and `cd` into project's directory. Then execute the following lines to generate a build system:

```
cmake -G "Unix Makefiles" . -DVENDOR_NAME_MASTER=ConnextDDSPro
```


If you installed Google Test at a non-default location, you must tell `cmake` where is Google Test by passing `-DGTEST_ROOT=<where gtest is installed>`. The full cmake command should look like

```
cmake -G "Unix Makefiles" . -DVENDOR_NAME_MASTER=ConnextDDSPro -DGTEST_ROOT=<where gtest is installed>
```

You can also generate other build systems. It's up to you to use your favourite build system.

`-DVENDOR_NAME_MASTER` is a **required** flag that tells CMake what DDS implementation to use for the master process. `-DVENDOR_NAME_SLAVE` specifies DDS implementation for slave processes, which is optional. If `-DVENDOR_NAME_SLAVE` is not specified the same DDS implementation will be used for master and slave processes.

##### Additional Flags
Depending on which DDS vendor is used, their might be additional flags required.

`ConnextDDSPro`:

1. `optional` You may specify which installation to use by pass `-DCONNEXTDDS_DIR=<install directory>`.
2. `optional` You may specify which target package to use by pass `-DCONNEXTDDS_ARCH=<target arch>`

`ConnextDDSMicro`: Since micro depends on `OpenSSL` and `libxml2`, there are more flags.

1. You may specify which OpenSSL installation to use by passing `-DOPENSSL_ROOT_DIR=<where is openssl>`. 
2. `required` You must specify where ConnextDDSMicro is installed to use by passing `-CONNEXTDDSME_DIR=<dir>` 
3. `optional` You may specify which installation to use by pass `-DCONNEXTDDSME_DIR=<install directory>`. When this flag is missing, value of `-DCONNEXTDDS_DIR` will be used.

###### Build the tests using generated build system:
* `make` if `-G "Unix Makefiles"`
* `Ninja` if `-G "Ninja"`

### How to build the project with other DDS implementations

1. Create a CMake script with name `<your_dds_vendor_name>.cmake` and put it in `cmake` directory. This script will contain everything someone need to know in order to compile an application with your dds vendor. It must implement 6 functions below:

```
FUNCTION(GET_DDS_INFO INCLUDE_DIRS LINK_LIBS COMPILE_DEFINITIONS)
	#Tell us what include paths are needed, what libraries we need to link against, and what preprocess macros are required.
	#SET(${INCLUDE_DIRS} 
	#		/usr/include 
	#		/usr/local/include 
	#		PARENT_SCOPE)
	#SET(${LINK_LIBS} 
	#		/usr/lib/libfoo.a 
	#		/usr/lib/libbar.a} 
	#		PARENT_SCOPE)
	#IF (${CMAKE_BUILD_TYPE} MATCHES "[Rr]elease")
	#	SET(${COMPILE_DEFINITIONS} 
	#			RELEASE HelloWorld 
	#			PARENT_SCOPE)
	ENDIF()
ENDFUNCTION()

FUNCTION(GET_DDS_GEN_FILES basename ret)
	# ${basename} holds the name of an 
	# .idl file(without .idl extension)
	# return what source files and header
	# files will be generated from this idl file
	#SET(${ret} 
	#		${basename}.cxx 
	#		${basename}Plugin.cxx 
	#		${basename}Support.cxx 
	#		PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(GET_DDS_GEN_COMMAND file output_dir cmd)
	# ${file} holds the full path to an idl file
	# with .idl extension.
	# ${output_dir} holds the path to a directory 
	# which generated source & header files
	# should be stored.
	# return the command to regenerate source &
	# header files.
	#SET(${cmd} SOME_ENV="Some_Value"  
	#           -d ${output_dir} ${file}
	#            PARENT_SCOPE)
ENDFUNCTION()
```
To start, paste above code into `<your_dds_vendor_name>.cmake` and start implement them. You can also read `cmake/ConnextDDSPro.cmake` to get an idea.

### How to run the tests
```
cd objs/
./xtype-interop
```
Where `arch` must be the same string you give to `cmake` when building the project.

It is important that you `cd` into the `xtype-interop`'s directory first. Otherwise `xtype-interop` will not be able to find its helper executables.


## References <a id="2"></a>

  * [Extensible and Dynamic Topic Types for DDS Specification version 1.2](https://www.google.com/url?q=https://www.omg.org/spec/DDS-XTypes/1.2/PDF&sa=D&ust=1533927289304000)
  * [OMG Interoperability examples for the ](https://www.google.com/url?q=https://github.com/omg-dds&sa=D&ust=1533927289304000)[Extensible and Dynamic Topic Types for DDS Specification](https://www.google.com/url?q=https://github.com/omg-dds&sa=D&ust=1533927289305000)
  * [shapes.idl](idl/shapes.idl)  (Note: The file might not be up to date, it will be updated periodically as we finish creating all the scenarios)