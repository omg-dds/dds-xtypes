//
//  DDS_Variants.h
//  xtypes-interop
//
//  Created by Kelvin Zhang on 6/11/18.
//  Copyright © 2018 Kelvin Zhang. All rights reserved.
//

#ifndef __DDS_VARIANTS_HEADER
#define __DDS_VARIANTS_HEADER
#include <Conformance.h>

enum LogVerbosity{
    SILENT,
    FATAL,
    WARNING,
    DEBUG
};


#if defined(RTI_DDS_PRO)

#include <ndds/ndds_namespace_cpp.h>
#include <shapes.h>
#include <shapesSupport.h>
#include <commands.h>
#include <commandsSupport.h>


static inline void set_log_verbosity(LogVerbosity verbosity)
{
    switch (verbosity) {
        case SILENT:
            NDDSConfigLogger::get_instance()->set_verbosity(NDDS_CONFIG_LOG_VERBOSITY_SILENT);
            break;
        case FATAL:
            NDDSConfigLogger::get_instance()->set_verbosity(NDDS_CONFIG_LOG_VERBOSITY_ERROR);
            break;
        case WARNING:
            NDDSConfigLogger::get_instance()->set_verbosity(NDDS_CONFIG_LOG_VERBOSITY_WARNING);
            break;
        case DEBUG:
            NDDSConfigLogger::get_instance()->set_verbosity(NDDS_CONFIG_LOG_VERBOSITY_STATUS_LOCAL);
            break;
    }
}

static inline void sleep_ms(unsigned int milliseconds)
{
    NDDSUtility::sleep({0, milliseconds});
}

#define DEFINE_TOPIC_TRAITS(TYPE) \
template<>\
struct topic_traits<TYPE> {\
typedef struct TYPE##Seq Seq;\
typedef TYPE##TypeSupport TypeSupport;\
typedef TYPE##DataWriter DataWriter;\
typedef TYPE##DataReader DataReader;\
};

#elif defined(RTI_DDS_MICRO)

#include <rti_me_cpp.hxx>
#include <shapes.h>
#include <shapesSupport.h>
#include <commands.h>
#include <commandsSupport.h>



static inline void sleep_ms(unsigned int milliseconds)
{
    OSAPI_Thread_sleep(milliseconds);
}

static inline void set_log_verbosity(LogVerbosity verbosity)
{
    switch (verbosity) {
        case SILENT:
            OSAPI_Log_set_verbosity(OSAPI_LOG_VERBOSITY_SILENT);
            break;
        case FATAL:
            OSAPI_Log_set_verbosity(OSAPI_LOG_VERBOSITY_ERROR);
            break;
        case WARNING:
            OSAPI_Log_set_verbosity(OSAPI_LOG_VERBOSITY_WARNING);
            break;
        case DEBUG:
            OSAPI_Log_set_verbosity(OSAPI_LOG_VERBOSITY_DEBUG);
            break;
    }
}

#define DEFINE_TOPIC_TRAITS(TYPE) \
template<>\
struct topic_traits<TYPE> {\
typedef struct TYPE##Seq Seq;\
typedef TYPE##TypeSupport TypeSupport;\
typedef TYPE##DataWriter DataWriter;\
typedef TYPE##DataReader DataReader;\
};

#elif defined(TWINOAKS_COREDX)
static inline void set_log_verbosity(LogVerbosity verbosity)
{
    //implement this :)
}
#include <dds/dds.hh>
#include <shapes.hh>
#include <shapesTypeSupport.hh>
#include <commands.hh>
#include <commandsTypeSupport.hh>

#else
#   error No DDS vendor -define was provided. No DDS header files included.
#   Compilation will fail
#   error Please configure the makefile to define one for the following
#  variables: RTI_DDS_PRO, RTI_DDS_MICRO, TWINOAKS_COREDX
#endif

template <typename T>
struct topic_traits{
};
DEFINE_TOPIC_TRAITS(Point2DFinal)
DEFINE_TOPIC_TRAITS(Point2DAppendable)
DEFINE_TOPIC_TRAITS(Point3DFinal)
DEFINE_TOPIC_TRAITS(Point3DAppendable)
DEFINE_TOPIC_TRAITS(Shape1Default)
DEFINE_TOPIC_TRAITS(Shape1Final)
DEFINE_TOPIC_TRAITS(Shape1Appendable)
DEFINE_TOPIC_TRAITS(Shape1Mutable)
DEFINE_TOPIC_TRAITS(Shape1MutableExplicitID)
DEFINE_TOPIC_TRAITS(Shape2Final)
DEFINE_TOPIC_TRAITS(Shape2Appendable)
DEFINE_TOPIC_TRAITS(Shape2Mutable)
DEFINE_TOPIC_TRAITS(Shape3Final)
DEFINE_TOPIC_TRAITS(Shape3Appendable)
DEFINE_TOPIC_TRAITS(Shape3Mutable)
DEFINE_TOPIC_TRAITS(Shape4Final)
DEFINE_TOPIC_TRAITS(Shape4Appendable)
DEFINE_TOPIC_TRAITS(Shape4Mutable)
DEFINE_TOPIC_TRAITS(Shape4MutableExplicitID)
DEFINE_TOPIC_TRAITS(Shape5Final)
DEFINE_TOPIC_TRAITS(Shape5Appendable)
DEFINE_TOPIC_TRAITS(Shape5Mutable)
DEFINE_TOPIC_TRAITS(Shape6Final)
DEFINE_TOPIC_TRAITS(Shape6Appendable)
DEFINE_TOPIC_TRAITS(Shape6Mutable)
DEFINE_TOPIC_TRAITS(Shape7Final)
DEFINE_TOPIC_TRAITS(Shape7Appendable)
DEFINE_TOPIC_TRAITS(Shape7Mutable)
DEFINE_TOPIC_TRAITS(Shape8Final)
DEFINE_TOPIC_TRAITS(Shape8Appendable)
DEFINE_TOPIC_TRAITS(Shape8Mutable)
DEFINE_TOPIC_TRAITS(Shape9Final)
DEFINE_TOPIC_TRAITS(Shape9Appendable)
DEFINE_TOPIC_TRAITS(Shape9Mutable)
DEFINE_TOPIC_TRAITS(Shape10Final)
DEFINE_TOPIC_TRAITS(Shape10Appendable)
DEFINE_TOPIC_TRAITS(Shape10Mutable)
DEFINE_TOPIC_TRAITS(Shape11Final)
DEFINE_TOPIC_TRAITS(Shape11Appendable)
DEFINE_TOPIC_TRAITS(Shape11Mutable)
DEFINE_TOPIC_TRAITS(Parameter1Final)
DEFINE_TOPIC_TRAITS(Parameter1Appendable)
DEFINE_TOPIC_TRAITS(Parameter1Mutable)
DEFINE_TOPIC_TRAITS(Parameter2Final)
DEFINE_TOPIC_TRAITS(Parameter2Appendable)
DEFINE_TOPIC_TRAITS(Parameter2Mutable)
DEFINE_TOPIC_TRAITS(Parameter3Final)
DEFINE_TOPIC_TRAITS(Parameter3Appendable)
DEFINE_TOPIC_TRAITS(Parameter3Mutable)
DEFINE_TOPIC_TRAITS(Parameter4Final)
DEFINE_TOPIC_TRAITS(Parameter4Appendable)
DEFINE_TOPIC_TRAITS(Parameter4Mutable)
DEFINE_TOPIC_TRAITS(Parameter5Final)
DEFINE_TOPIC_TRAITS(Parameter5Appendable)
DEFINE_TOPIC_TRAITS(Parameter5Mutable)
DEFINE_TOPIC_TRAITS(Parameter6Final)
DEFINE_TOPIC_TRAITS(Parameter6Appendable)
DEFINE_TOPIC_TRAITS(Parameter6Mutable)
DEFINE_TOPIC_TRAITS(Parameter7Final)
DEFINE_TOPIC_TRAITS(Parameter7Appendable)
DEFINE_TOPIC_TRAITS(Parameter7Mutable)
DEFINE_TOPIC_TRAITS(ControlMessage)

template <typename T>
T *create_data()
{
    return topic_traits<T>::TypeSupport::create_data();
}

template <typename T>
void delete_data(T *data)
{
    topic_traits<T>::TypeSupport::delete_data(data);
}

template <typename TReader, typename T>
DDS_ReturnCode_t take_data(TReader *reader, T *data, DDS::SampleInfo *info)
{
#if defined  RTI_DDS_PRO
    return reader->take_next_sample(*data, *info);
#elif defined RTI_DDS_MICRO
    return reader->take_next_sample(*data, *info);
#elif defined TWINOAKS_COREDX
    return reader->take_next_sample(data, info);
#endif
}

template <typename TWriter, typename T>
DDS_ReturnCode_t write_data(TWriter *writer, T *data)
{
#if defined  RTI_DDS_PRO
    return writer->write(*data, DDS_HANDLE_NIL);
#elif defined RTI_DDS_MICRO
    return writer->write(*data, DDS_HANDLE_NIL);
#elif defined TWINOAKS_COREDX
    return writer->write(data, DDS_HANDLE_NIL);
#endif
}

#endif
