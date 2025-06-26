

#include <stdio.h>
#include <stdlib.h>

#if defined(RTI_CONNEXT_DDS)
#  include "ndds/ndds_namespace_cpp.h"
#  include "ShapeType.h"
#  include "ShapeTypeSupport.h"

#elif defined(TWINOAKS_COREDX)
#  include <dds/dds.hh>
#  include "ShapeType.hh"
#  include "ShapeTypeTypeSupport.hh"
#  if (COREDX_DDS_VERSION_MAJOR < 4)
#    error X-Types support requires CoreDX DDS v4.0 or newer
#  endif

#elif defined(OCI_OPENDDS)
#  include "ShapeTypeTypeSupportImpl.h"
#  include <dds/DdsDcpsC.h>
#  include <dds/DCPS/Marked_Default_Qos.h>
#  include <dds/DCPS/PrinterValueWriter.h>
#  include <dds/DCPS/Service_Participant.h>
#  include <dds/DCPS/WaitSet.h>
#  include <dds/DCPS/transport/framework/TransportConfig.h>
#  include <dds/DCPS/transport/framework/TransportInst.h>
#  include <dds/DCPS/transport/framework/TransportRegistry.h>
#  include <iostream>

#else
#  error No DDS vendor -define was provided. No DDS header files included.  Compilation will fail
#  error Please configure the makefile to define one for the following variables: RTI_CONNEXT_DDS, TWINOAKS_COREDX, or OCI_OPENDDS
#endif

#ifdef OCI_OPENDDS
#  include <tao/idl_features.h>
#  define HAS_SHAPE5_IDL TAO_IDL_HAS_STRUCT_INHERITANCE
#else
#  define HAS_SHAPE5_IDL 1
#endif

using namespace DDS;

#ifdef OCI_OPENDDS
const StatusMask STATUS_MASK_NONE = OpenDDS::DCPS::NO_STATUS_MASK;

template <typename T>
void print(std::ostream& out, const T& data)
{
    OpenDDS::DCPS::PrinterValueWriter pvw;
    vwrite(pvw, data);
    out << '\n' << pvw.str() << '\n';
}

template <typename T>
class StaticTypeSupport {
public:
    static ReturnCode_t register_type(DomainParticipant* participant, const char* type_name)
    {
        return instance()->register_type(participant, type_name);
    }

    static const char* get_type_name()
    {
        return instance()->get_type_name();
    }

private:
    typedef typename OpenDDS::DCPS::DDSTraits<T> Traits;
    typedef typename Traits::TypeSupportType TypeSupport;
    typedef typename Traits::TypeSupportImplType TypeSupportImpl;

    static TypeSupport* instance() {
        static TypeSupportImpl tsimpl;
        return dynamic_cast<TypeSupport*>(&tsimpl);
    }
};

void setup_rtps()
{
    TheServiceParticipant->set_default_discovery(
        OpenDDS::DCPS::Discovery::DEFAULT_RTPS);
    OpenDDS::DCPS::TransportConfig_rch transport_config =
        TheTransportRegistry->create_config("default_rtps_transport_config");
    OpenDDS::DCPS::TransportInst_rch transport_inst =
        TheTransportRegistry->create_inst("default_rtps_transport", "rtps_udp");
    transport_config->instances_.push_back(transport_inst);
    TheTransportRegistry->global_config(transport_config);
    OpenDDS::DCPS::DCPS_debug_level = 4;
}

void strcpy(TAO::String_Manager_T<char>& dst, const char* src)
{
    dst = src;
}

#else
const StatusMask STATUS_MASK_NONE = DDS_STATUS_MASK_NONE;
#endif

DomainParticipantFactory* get_domain_participant_factory()
{
#ifdef OCI_OPENDDS
    return TheParticipantFactory;
#else
    return DomainParticipantFactory::get_instance();
#endif
}

class TheTopicListener : public TopicListener {
public:
    virtual void on_inconsistent_topic(
        Topic *topic,
        const InconsistentTopicStatus & status) {
            fprintf(stderr, "on_inconsistent_topic: topic \"%s\", type \"%s\" count: %d, change: %d\n",
                    topic->get_name(),
                    topic->get_type_name(),
                    status.total_count, status.total_count_change);
    }
};

/*  Default types   */
template <typename T> class Shape1Filler {
public:
    static void fill_data(T *data, const char *color, int count) {
        strcpy(data->color, color);
        data->x = count % 250;
        data->y = 2*count %250;
        data->shapesize = 30;
     }
};

template <typename T> class Shape2Filler {
public:
    static void fill_data(T *data, const char *color, int count) {
        strcpy(data->color, color);
        data->x = count % 250;
        data->y = 2*count %250;
        data->shapesize = 30;
        data->angle = (float)((5*count)%360);
    };
};

template <typename T> class Shape3Filler {
public:
    static void fill_data(T *data, const char *color, int count) {
        strcpy(data->color, color);
        data->x = count % 250;
        data->y = 2*count %250;
        data->shapesize = 30;
        data->z = 3*count %250;
    }
};

template <typename T> class Shape4Filler {
public:
    static void fill_data(T *data, const char *color, int count) {
        strcpy(data->color, color);
        data->x = count % 250;
        data->y = 2*count %250;
        data->shapesize = 30;
    }
};

#if HAS_SHAPE5_IDL
template <typename T> class Shape5Filler {
public:
    static void fill_data(T *data, const char *color, int count) {
        strcpy(data->color, color);
        data->x = count % 250;
        data->y = 2*count %250;
        data->shapesize = 30;
        data->angle = (float)((5*count)%360);
    }
};
#endif

template <typename TData, typename TDataFiller> class GenericFiller {
public:
    static void fill_data(TData *data, const char *color, int count) {
        TDataFiller::fill_data(data, color, count);
    }
};

class WriterBase {
public:
    virtual ~WriterBase() {}
    virtual bool initialize(DomainParticipant *participant, const char *topic_name) = 0;
    virtual bool write_data(const char *color, int count) = 0;
    virtual Topic *get_topic() = 0;
};

template <typename T, typename TFiller, typename TSupport, typename TDataWriter>
class Writer : public WriterBase {
public:
#ifdef OCI_OPENDDS
    typedef StaticTypeSupport<T> TypeSupport;
#else
    typedef TSupport TypeSupport;
#endif

    virtual ~Writer() {
      _topic->set_listener(NULL, 0);
#if defined(TWINOAKS_COREDX) || defined(OCI_OPENDDS)
      delete _data;
#endif
#ifdef TWINOAKS_COREDX
      delete _topic_listener;
#elif defined OCI_OPENDDS
      TopicListener_var tl = _topic_listener;
      tl = 0;
#endif
    }

    virtual bool initialize(DomainParticipant *participant, const char *topic_name) {
        ReturnCode_t retcode = TypeSupport::register_type(
            participant, TypeSupport::get_type_name());
        if ( retcode != RETCODE_OK) {
            fprintf(stderr, "Failed to register type for topic \"%s\" retcode=%d\n", topic_name, (int)retcode);
            return false;
        }

        Publisher *publisher = participant->create_publisher(
            PUBLISHER_QOS_DEFAULT, NULL /* listener */, STATUS_MASK_NONE);
        if (publisher == NULL) {
            printf("create_publisher error for topic \"%s\"\n", topic_name);
            return false;
        }

        _topic_listener = new TheTopicListener();

        _topic = participant->create_topic(
            topic_name, TypeSupport::get_type_name(),
            TOPIC_QOS_DEFAULT, _topic_listener,
            INCONSISTENT_TOPIC_STATUS);
        if (_topic == NULL) {
            printf("create_topic error for topic \"%s\"\n", topic_name);
            return false;
        }

        DataWriter *writer = publisher->create_datawriter(
            _topic, DATAWRITER_QOS_DEFAULT, NULL /* listener */,
            STATUS_MASK_NONE);
        if (writer == NULL) {
            printf("create_datawriter error for topic \"%s\"\n", topic_name);
            return false;
        }

#ifdef OCI_OPENDDS
        _writer = TDataWriter::_narrow(writer);
#else
        _writer = TDataWriter::narrow(writer);
#endif
        if (_writer == NULL) {
            printf("DataWriter narrow error for topic \"%s\"\n", topic_name);
            return false;
        }

#if defined   RTI_CONNEXT_DDS
        _data = TSupport::create_data();
#elif defined TWINOAKS_COREDX || defined OCI_OPENDDS
        _data = new T();
#endif

        return true;
    }

    virtual bool write_data(const char *color, int count) {
        _shapeFiller.fill_data(_data, color, count);
        printf("Writing Topic \"%s\", type \"%s\", count %d, data:",
               _writer->get_topic()->get_name(),
               _writer->get_topic()->get_type_name(), count);

#if   defined RTI_CONNEXT_DDS
        TSupport::print_data(_data);
#elif defined TWINOAKS_COREDX
        T::print(stdout, _data);
#elif defined OCI_OPENDDS
        print(std::cout, *_data);
#endif

#ifdef TWINOAKS_COREDX
        return _writer->write(_data, HANDLE_NIL) == RETCODE_OK;
#else
        return _writer->write(*_data, HANDLE_NIL) == RETCODE_OK;
#endif
    }

    virtual Topic *get_topic() { return _topic;  }

private:
    TDataWriter   *_writer;
    Topic         *_topic;
    TopicListener *_topic_listener;
    T             *_data;
    GenericFiller<T, TFiller> _shapeFiller;
};

class ReaderBase {
public:
    virtual ~ReaderBase() {}
    virtual bool initialize(DomainParticipant *participant, const char *topic_name) = 0;
    virtual int take_data() = 0;
    virtual Topic *get_topic() = 0;
    virtual bool wait_for_data(Duration_t timeout) = 0;
};


class TheReaderListener : public DataReaderListener {
public:
    virtual void on_requested_incompatible_qos(
        DataReader* reader,
        const RequestedIncompatibleQosStatus& status) {
           fprintf(stderr, "on_requested_incompatible_qos: topic \"%s\", type \"%s\" last_policy: %d, count: %d, change: %d\n",
                    reader->get_topicdescription()->get_name(),
                    reader->get_topicdescription()->get_type_name(),
                    status.last_policy_id,
                    status.total_count, status.total_count_change);
    }

    virtual void on_subscription_matched(
        DataReader* reader,
        const SubscriptionMatchedStatus& status) {
             fprintf(stderr, "on_subscription_matched: topic \"%s\", type \"%s\", count: %d, change: %d\n",
                    reader->get_topicdescription()->get_name(),
                    reader->get_topicdescription()->get_type_name(),
                    status.current_count, status.current_count_change);
    }

    // Some implementations require all overrides to be defined (they are pure virtual in the base class).
    // For other implementations it won't hurt to have them here
    void on_requested_deadline_missed(
        DataReader*, const RequestedDeadlineMissedStatus&) {}
    void on_sample_rejected(
        DataReader*, const SampleRejectedStatus&) {}
    void on_liveliness_changed(
        DataReader*, const LivelinessChangedStatus&) {}
    void on_data_available(DataReader*) {}
    void on_sample_lost(DataReader*, const SampleLostStatus&) {}
};

template <typename T, typename TSupport, typename TDataReader>
class Reader : public ReaderBase {
public:
#ifdef OCI_OPENDDS
    typedef StaticTypeSupport<T> TypeSupport;
#else
    typedef TSupport TypeSupport;
#endif

    virtual ~Reader() {

      _reader->set_listener(NULL, 0);
      _topic->set_listener(NULL, 0);
#if defined(TWINOAKS_COREDX) || defined(OCI_OPENDDS)
      delete _data;
#endif
#ifdef TWINOAKS_COREDX
      delete _reader_listener;
      delete _topic_listener;
#elif defined OCI_OPENDDS
      TopicListener_var tl = _topic_listener;
      tl = 0;
      DataReaderListener_var rl = _reader_listener;
      rl = 0;
#endif
    }

    bool initialize(DomainParticipant *participant, const char *topic_name) {
        ReturnCode_t retcode;
        retcode = TypeSupport::register_type(participant, TypeSupport::get_type_name());
        if ( retcode != RETCODE_OK) {
            fprintf(stderr, "Failed to register type for topic \"%s\" retcode=%d\n", topic_name, (int)retcode);
            return false;
        }

        Subscriber *subscriber = participant->create_subscriber(
            SUBSCRIBER_QOS_DEFAULT, NULL /* listener */, STATUS_MASK_NONE);
        if (subscriber == NULL) {
            printf("create subscriber error for topic \"%s\"\n", topic_name);
            return false;
        }

        _topic_listener = new TheTopicListener();
        _topic = participant->create_topic(
            topic_name, TypeSupport::get_type_name(),
            TOPIC_QOS_DEFAULT, _topic_listener,
            INCONSISTENT_TOPIC_STATUS );
        if (_topic == NULL) {
            printf("create topic error for topic \"%s\"\n", topic_name);
            return false;
        }


        _reader_listener = new TheReaderListener();
        DataReader *reader = subscriber->create_datareader(
            _topic, DATAREADER_QOS_DEFAULT, _reader_listener,
            REQUESTED_INCOMPATIBLE_QOS_STATUS | SUBSCRIPTION_MATCHED_STATUS);
        if (reader == NULL) {
            printf("create datareader error for topic \"%s\"\n", topic_name);
            return false;
        }

#ifdef OCI_OPENDDS
        _reader = TDataReader::_narrow(reader);
#else
        _reader = TDataReader::narrow(reader);
#endif
        if (_reader == NULL) {
            printf("DataWriter narrow error for topic \"%s\"\n", topic_name);
            return false;
        }

        ReadCondition  *cond = _reader->create_readcondition( ANY_SAMPLE_STATE,
                                                              ANY_VIEW_STATE,
                                                              ANY_INSTANCE_STATE);
        _waitset.attach_condition(cond);

#if   defined(RTI_CONNEXT_DDS)
        _data = TSupport::create_data();
#elif defined(TWINOAKS_COREDX) || defined(OCI_OPENDDS)
        _data = new T();
#endif

        return true;
    }

    int take_data() {
        SampleInfo info;
        ReturnCode_t retcode;
        int sample_count = 0;

        do {

#if   defined(RTI_CONNEXT_DDS) || defined(OCI_OPENDDS)
            retcode = _reader->take_next_sample(*_data, info);
#elif defined(TWINOAKS_COREDX)
            retcode = _reader->take_next_sample(_data, &info);
#endif

            if ( (retcode == RETCODE_OK ) && info.valid_data ) {
              printf(
                "\n"
                "Reading Topic \"%s\", type \"%s\", data:",
                 _reader->get_topicdescription()->get_name(),
                 _reader->get_topicdescription()->get_type_name());
                ++sample_count;

#if   defined(RTI_CONNEXT_DDS)
                TSupport::print_data(_data);
#elif defined(TWINOAKS_COREDX)
                T::print(stdout, _data);
#elif defined(OCI_OPENDDS)
                print(std::cout, *_data);
#endif

            }
            else if (retcode == RETCODE_NO_DATA) {
                return sample_count; // all done
            }
        }
        while ( retcode == RETCODE_OK );

        fprintf(stderr, "Take error %d for Topic %s\n",(int)retcode,  _reader->get_topicdescription()->get_name());
        return sample_count; // done reading
    }

    Topic *get_topic() { return _topic;  }

    bool wait_for_data(Duration_t timeout) {
        ConditionSeq active_cond;
        return _waitset.wait(active_cond, timeout) == RETCODE_OK;
    }

private:
    TDataReader        * _reader;
    DataReaderListener * _reader_listener;
    Topic              * _topic;
    TopicListener      * _topic_listener;
    T                  * _data;
    WaitSet _waitset;
};


class ShapeTypeVariants {
private:
    static const int TYPE_EXTENSIBILITY_KIND_COUNT=5;
    static const int TYPE_VERSION_COUNT=5;

    static const char *extensibility_kind(int i) {
        static const char *TYPE_EXTENSIBILITY_KIND[5] = {
            "Default",
            "Final",
            "Extensible",
            "Mutable",
            "MutableExplicitID"

        };

        if ( i<0 || i >= 5) {
            return NULL;
        }
        return TYPE_EXTENSIBILITY_KIND[i];
    }

public:
    static void print_type_variants(FILE *fp) {
        for (int version=1; version<= TYPE_VERSION_COUNT; ++version) {
            for (int kind=0; kind<TYPE_EXTENSIBILITY_KIND_COUNT; ++kind) {
                fprintf(fp, "Shape%d%s, ", version, extensibility_kind(kind));
            }
            fprintf(fp, "\n");
        }
    }

    static bool check_type_variant(const char *type_name) {
        char typename_buffer[64];
        for (int version=1; version<= TYPE_VERSION_COUNT; ++version) {
            for (int kind=0; kind<TYPE_EXTENSIBILITY_KIND_COUNT; ++kind) {
                sprintf(typename_buffer, "Shape%d%s", version, extensibility_kind(kind));
                if ( strcmp(typename_buffer, type_name) == 0 ) {
                    return true;
                }
            }
        }
        return false;
    }


    static ReaderBase *create_reader(const char *type_name) {
        if ( strcmp(type_name, "Shape1Default") == 0 ) {
            return new Reader<Shape1Default, Shape1DefaultTypeSupport, Shape1DefaultDataReader>();
        }
        else if ( strcmp(type_name, "Shape1Final") == 0 ) {
            return new Reader<Shape1Final, Shape1FinalTypeSupport, Shape1FinalDataReader>();
        }
        else if ( strcmp(type_name, "Shape1Extensible") == 0 ) {
            return new Reader<Shape1Extensible, Shape1ExtensibleTypeSupport, Shape1ExtensibleDataReader>();
        }
        else if ( strcmp(type_name, "Shape1Mutable") == 0 ) {
            return new Reader<Shape1Mutable, Shape1MutableTypeSupport, Shape1MutableDataReader>();
        }
        else if ( strcmp(type_name, "Shape1MutableExplicitID") == 0 ) {
            return new Reader<Shape1MutableExplicitID, Shape1MutableExplicitIDTypeSupport, Shape1MutableExplicitIDDataReader>();
        }
        else if (strcmp(type_name, "Shape2Default") == 0 ) {
            return new Reader<Shape2Default, Shape2DefaultTypeSupport, Shape2DefaultDataReader>();
        }
        else if (strcmp(type_name, "Shape2Final") == 0 ) {
            return new Reader<Shape2Final, Shape2FinalTypeSupport, Shape2FinalDataReader>();
        }
        else if (strcmp(type_name, "Shape2Extensible") == 0 ) {
            return new Reader<Shape2Extensible, Shape2ExtensibleTypeSupport, Shape2ExtensibleDataReader>();
        }
        else if (strcmp(type_name, "Shape2Mutable") == 0 ) {
            return new Reader<Shape2Mutable, Shape2MutableTypeSupport, Shape2MutableDataReader>();
        }
        else if (strcmp(type_name, "Shape2MutableExplicitID") == 0 ) {
            return new Reader<Shape2MutableExplicitID, Shape2MutableExplicitIDTypeSupport, Shape2MutableExplicitIDDataReader>();
        }
        else if (strcmp(type_name, "Shape3Default") == 0 ) {
            return new Reader<Shape3Default, Shape3DefaultTypeSupport, Shape3DefaultDataReader>();
        }
        else if (strcmp(type_name, "Shape3Final") == 0 ) {
            return new Reader<Shape3Final, Shape3FinalTypeSupport, Shape3FinalDataReader>();
        }
        else if (strcmp(type_name, "Shape3Extensible") == 0 ) {
            return new Reader<Shape3Extensible, Shape3ExtensibleTypeSupport, Shape3ExtensibleDataReader>();
        }
        else if (strcmp(type_name, "Shape3Mutable") == 0 ) {
            return new Reader<Shape3Mutable, Shape3MutableTypeSupport, Shape3MutableDataReader>();
        }
        else if (strcmp(type_name, "Shape3MutableExplicitID") == 0 ) {
            return new Reader<Shape3MutableExplicitID, Shape3MutableExplicitIDTypeSupport, Shape3MutableExplicitIDDataReader>();
        }
        else if (strcmp(type_name, "Shape4Default") == 0 ) {
            return new Reader<Shape4Default, Shape4DefaultTypeSupport, Shape4DefaultDataReader>();
        }
        else if (strcmp(type_name, "Shape4Final") == 0 ) {
            return new Reader<Shape4Final, Shape4FinalTypeSupport, Shape4FinalDataReader>();
        }
        else if (strcmp(type_name, "Shape4Extensible") == 0 ) {
            return new Reader<Shape4Extensible, Shape4ExtensibleTypeSupport, Shape4ExtensibleDataReader>();
        }
        else if (strcmp(type_name, "Shape4Mutable") == 0 ) {
            return new Reader<Shape4Mutable, Shape4MutableTypeSupport, Shape4MutableDataReader>();
        }
        else if (strcmp(type_name, "Shape4MutableExplicitID") == 0 ) {
            return new Reader<Shape4MutableExplicitID, Shape4MutableExplicitIDTypeSupport, Shape4MutableExplicitIDDataReader>();
        }
#if HAS_SHAPE5_IDL
        else if (strcmp(type_name, "Shape5Default") == 0 ) {
            return new Reader<Shape5Default, Shape5DefaultTypeSupport, Shape5DefaultDataReader>();
        }
        else if (strcmp(type_name, "Shape5Final") == 0 ) {
            return new Reader<Shape5Final, Shape5FinalTypeSupport, Shape5FinalDataReader>();
        }
        else if (strcmp(type_name, "Shape5Extensible") == 0 ) {
            return new Reader<Shape5Extensible, Shape5ExtensibleTypeSupport, Shape5ExtensibleDataReader>();
        }
        else if (strcmp(type_name, "Shape5Mutable") == 0 ) {
            return new Reader<Shape5Mutable, Shape5MutableTypeSupport, Shape5MutableDataReader>();
        }
        else if (strcmp(type_name, "Shape5MutableExplicitID") == 0 ) {
            return new Reader<Shape5MutableExplicitID, Shape5MutableExplicitIDTypeSupport, Shape5MutableExplicitIDDataReader>();
        }
#endif

        //  else
        fprintf(stderr, "create_reader: Unrecognized type: \"%s\"\n", type_name);
        return NULL;
    }

    static WriterBase *create_writer(const char *type_name) {
        if ( strcmp(type_name, "Shape1Default") == 0 ) {
          return new Writer<Shape1Default, Shape1Filler<Shape1Default>, Shape1DefaultTypeSupport, Shape1DefaultDataWriter>();
        }
        else if ( strcmp(type_name, "Shape1Final") == 0 ) {
          return new Writer<Shape1Final, Shape1Filler<Shape1Final>, Shape1FinalTypeSupport, Shape1FinalDataWriter>();
        }
        else if ( strcmp(type_name, "Shape1Extensible") == 0 ) {
            return new Writer<Shape1Extensible, Shape1Filler<Shape1Extensible>, Shape1ExtensibleTypeSupport, Shape1ExtensibleDataWriter>();
        }
        else if ( strcmp(type_name, "Shape1Mutable") == 0 ) {
            return new Writer<Shape1Mutable, Shape1Filler<Shape1Mutable>, Shape1MutableTypeSupport, Shape1MutableDataWriter>();
        }
        else if ( strcmp(type_name, "Shape1MutableExplicitID") == 0 ) {
            return new Writer<Shape1MutableExplicitID, Shape1Filler<Shape1MutableExplicitID>, Shape1MutableExplicitIDTypeSupport, Shape1MutableExplicitIDDataWriter>();
        }
        else if (strcmp(type_name, "Shape2Default") == 0 ) {
            return new Writer<Shape2Default, Shape2Filler<Shape2Default>, Shape2DefaultTypeSupport, Shape2DefaultDataWriter>();
        }
        else if (strcmp(type_name, "Shape2Final") == 0 ) {
            return new Writer<Shape2Final, Shape2Filler<Shape2Final>, Shape2FinalTypeSupport, Shape2FinalDataWriter>();
        }
        else if (strcmp(type_name, "Shape2Extensible") == 0 ) {
            return new Writer<Shape2Extensible, Shape2Filler<Shape2Extensible>, Shape2ExtensibleTypeSupport, Shape2ExtensibleDataWriter>();
        }
        else if (strcmp(type_name, "Shape2Mutable") == 0 ) {
            return new Writer<Shape2Mutable, Shape2Filler<Shape2Mutable>, Shape2MutableTypeSupport, Shape2MutableDataWriter>();
        }
        else if (strcmp(type_name, "Shape2MutableExplicitID") == 0 ) {
            return new Writer<Shape2MutableExplicitID, Shape2Filler<Shape2MutableExplicitID>, Shape2MutableExplicitIDTypeSupport, Shape2MutableExplicitIDDataWriter>();
        }
        else if (strcmp(type_name, "Shape3Default") == 0 ) {
            return new Writer<Shape3Default, Shape3Filler<Shape3Default>, Shape3DefaultTypeSupport, Shape3DefaultDataWriter>();
        }
        else if (strcmp(type_name, "Shape3Final") == 0 ) {
            return new Writer<Shape3Final, Shape3Filler<Shape3Final>, Shape3FinalTypeSupport, Shape3FinalDataWriter>();
        }
        else if (strcmp(type_name, "Shape3Extensible") == 0 ) {
            return new Writer<Shape3Extensible, Shape3Filler<Shape3Extensible>, Shape3ExtensibleTypeSupport, Shape3ExtensibleDataWriter>();
        }
        else if (strcmp(type_name, "Shape3Mutable") == 0 ) {
            return new Writer<Shape3Mutable, Shape3Filler<Shape3Mutable>, Shape3MutableTypeSupport, Shape3MutableDataWriter>();
        }
        else if (strcmp(type_name, "Shape3MutableExplicitID") == 0 ) {
            return new Writer<Shape3MutableExplicitID, Shape3Filler<Shape3MutableExplicitID>, Shape3MutableExplicitIDTypeSupport, Shape3MutableExplicitIDDataWriter>();
        }
        else if (strcmp(type_name, "Shape4Default") == 0 ) {
            return new Writer<Shape4Default, Shape4Filler<Shape4Default>, Shape4DefaultTypeSupport, Shape4DefaultDataWriter>();
        }
        else if (strcmp(type_name, "Shape4Final") == 0 ) {
            return new Writer<Shape4Final, Shape4Filler<Shape4Final>, Shape4FinalTypeSupport, Shape4FinalDataWriter>();
        }
        else if (strcmp(type_name, "Shape4Extensible") == 0 ) {
            return new Writer<Shape4Extensible, Shape4Filler<Shape4Extensible>, Shape4ExtensibleTypeSupport, Shape4ExtensibleDataWriter>();
        }
        else if (strcmp(type_name, "Shape4Mutable") == 0 ) {
            return new Writer<Shape4Mutable, Shape4Filler<Shape4Mutable>, Shape4MutableTypeSupport, Shape4MutableDataWriter>();
        }
        else if (strcmp(type_name, "Shape4MutableExplicitID") == 0 ) {
            return new Writer<Shape4MutableExplicitID, Shape4Filler<Shape4MutableExplicitID>, Shape4MutableExplicitIDTypeSupport, Shape4MutableExplicitIDDataWriter>();
        }
#if HAS_SHAPE5_IDL
        else if (strcmp(type_name, "Shape5Default") == 0 ) {
            return new Writer<Shape5Default, Shape5Filler<Shape5Default>, Shape5DefaultTypeSupport, Shape5DefaultDataWriter>();
        }
        else if (strcmp(type_name, "Shape5Final") == 0 ) {
            return new Writer<Shape5Final, Shape5Filler<Shape5Final>, Shape5FinalTypeSupport, Shape5FinalDataWriter>();
        }
        else if (strcmp(type_name, "Shape5Extensible") == 0 ) {
            return new Writer<Shape5Extensible, Shape5Filler<Shape5Extensible>, Shape5ExtensibleTypeSupport, Shape5ExtensibleDataWriter>();
        }
        else if (strcmp(type_name, "Shape5Mutable") == 0 ) {
            return new Writer<Shape5Mutable, Shape5Filler<Shape5Mutable>, Shape5MutableTypeSupport, Shape5MutableDataWriter>();
        }
        else if (strcmp(type_name, "Shape5MutableExplicitID") == 0 ) {
            return new Writer<Shape5MutableExplicitID, Shape5Filler<Shape5MutableExplicitID>, Shape5MutableExplicitIDTypeSupport, Shape5MutableExplicitIDDataWriter>();
        }
#endif

        //  else
        fprintf(stderr, "create_writer: Unrecognized type: \"%s\"\n", type_name);
        return NULL;
    }
};


