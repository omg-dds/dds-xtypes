

#include <stdio.h>
#include <stdlib.h>

#if defined RTI_CONNEXT_DDS
#include "ShapeType.h"
#include "ShapeTypeSupport.h"
#include "ndds/ndds_cpp.h"
#include "ndds/ndds_namespace_cpp.h"
#else
#error No DDS vendor -define was provided. No DDS header files included.  Compilation will fail
#error Please configure the makefile to define one for the following variables: RTI_CONNEXT_DDS, TWINOAKS_COREDX, or PRISMTECH_OPENSPLICE
#endif


class TheTopicListener : public DDSTopicListener {
public:
    virtual void on_inconsistent_topic(
        DDSTopic *topic,
        const DDS_InconsistentTopicStatus & status) {
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
        data->x = count % 250;;
        data->y = 2*count %250;
        data->shapesize = 30;
     }
};

template <typename T> class Shape2Filler {
public:
    static void fill_data(T *data, const char *color, int count) {
        strcpy(data->color, color);
        data->x = count % 250;;
        data->y = 2*count %250;
        data->shapesize = 30;
        data->angle = (float)((5*count)%360);
    };
};

template <typename T> class Shape3Filler {
public:
    static void fill_data(T *data, const char *color, int count) {
        strcpy(data->color, color);
        data->x = count % 250;;
        data->y = 2*count %250;
        data->shapesize = 30;
        data->z = 3*count %250;
    }
};

template <typename T> class Shape4Filler {
public:
    static void fill_data(T *data, const char *color, int count) {
        strcpy(data->color, color);
        data->x = count % 250;;
        data->y = 2*count %250;
        data->shapesize = 30;
    }
};

template <typename T> class Shape5Filler {
public:
    static void fill_data(T *data, const char *color, int count) {
        strcpy(data->color, color);
        data->x = count % 250;;
        data->y = 2*count %250;
        data->shapesize = 30;
        data->angle = (float)((5*count)%360);
    }
};

template <typename T> class ShapeFiller {
public:
    static void fill_data(T *data, const char *color, int count);
};

template <typename T1, typename T2> class ShapeFiller2 {
public:
  static void fill_data(T1 *data, const char *color, int count) {
    T2::fill_data(data, color, count);
  }
};

template <> class ShapeFiller2<Shape1Default, Shape1Filler<Shape1Default> >;


/*  Default types   */
template <> class ShapeFiller<Shape1Default> {
public:
    static void fill_data(Shape1Default *data, const char *color, int count) {
        Shape1Filler<Shape1Default>::fill_data(data, color, count);
     }
};

template <> class ShapeFiller<Shape2Default> {
public:
    static void fill_data(Shape2Default *data, const char *color, int count) {
        Shape2Filler<Shape2Default>::fill_data(data, color, count);
    }
};

template <> class ShapeFiller<Shape3Default> {
public:
    static void fill_data(Shape3Default *data, const char *color, int count) {
        Shape3Filler<Shape3Default>::fill_data(data, color, count);
    }
};

template <> class ShapeFiller<Shape4Default> {
public:
    static void fill_data(Shape4Default *data, const char *color, int count) {
        Shape4Filler<Shape4Default>::fill_data(data, color, count);
    }
};

template <> class ShapeFiller<Shape5Default> {
public:
    static void fill_data(Shape5Default *data, const char *color, int count) {
        Shape5Filler<Shape5Default>::fill_data(data, color, count);
    }
};

/*   Final types   */
template <> class ShapeFiller<Shape1Final> {
public:
    static void fill_data(Shape1Final *data, const char *color, int count) {
        Shape1Filler<Shape1Final>::fill_data(data, color, count);
     }
};

template <> class ShapeFiller<Shape2Final> {
public:
    static void fill_data(Shape2Final *data, const char *color, int count) {
        Shape2Filler<Shape2Final>::fill_data(data, color, count);
    };
};

template <> class ShapeFiller<Shape3Final> {
public:
    static void fill_data(Shape3Final *data, const char *color, int count) {
        Shape3Filler<Shape3Final>::fill_data(data, color, count);
    }
};

template <> class ShapeFiller<Shape4Final> {
public:
    static void fill_data(Shape4Final *data, const char *color, int count) {
        Shape4Filler<Shape4Final>::fill_data(data, color, count);
    }
};

template <> class ShapeFiller<Shape5Final> {
public:
    static void fill_data(Shape5Final *data, const char *color, int count) {
        Shape5Filler<Shape5Final>::fill_data(data, color, count);
    }
};

/*   Extensible types   */

template <> class ShapeFiller<Shape1Extensible> {
public:
    static void fill_data(Shape1Extensible *data, const char *color, int count) {
        Shape1Filler<Shape1Extensible>::fill_data(data, color, count);
     }
};

template <> class ShapeFiller<Shape2Extensible> {
public:
    static void fill_data(Shape2Extensible *data, const char *color, int count) {
         Shape2Filler<Shape2Extensible>::fill_data(data, color, count);
    };
};

template <> class ShapeFiller<Shape3Extensible> {
public:
    static void fill_data(Shape3Extensible *data, const char *color, int count) {
         Shape3Filler<Shape3Extensible>::fill_data(data, color, count);
    }
};

template <> class ShapeFiller<Shape4Extensible> {
public:
    static void fill_data(Shape4Extensible *data, const char *color, int count) {
        Shape4Filler<Shape4Extensible>::fill_data(data, color, count);
    }
};

template <> class ShapeFiller<Shape5Extensible> {
public:
    static void fill_data(Shape5Extensible *data, const char *color, int count) {
        Shape5Filler<Shape5Extensible>::fill_data(data, color, count);
    }
};

/*   Mutable types   */
template <> class ShapeFiller<Shape1Mutable> {
public:
    static void fill_data(Shape1Mutable *data, const char *color, int count) {
        Shape1Filler<Shape1Mutable>::fill_data(data, color, count);
     }
};

template <> class ShapeFiller<Shape2Mutable> {
public:
    static void fill_data(Shape2Mutable *data, const char *color, int count) {
        Shape2Filler<Shape2Mutable>::fill_data(data, color, count);
    };
};

template <> class ShapeFiller<Shape3Mutable> {
public:
    static void fill_data(Shape3Mutable *data, const char *color, int count) {
        Shape3Filler<Shape3Mutable>::fill_data(data, color, count);
    }
};

template <> class ShapeFiller<Shape4Mutable> {
public:
    static void fill_data(Shape4Mutable *data, const char *color, int count) {
        Shape4Filler<Shape4Mutable>::fill_data(data, color, count);
    }
};

template <> class ShapeFiller<Shape5Mutable> {
public:
    static void fill_data(Shape5Mutable *data, const char *color, int count) {
        Shape5Filler<Shape5Mutable>::fill_data(data, color, count);
    }
};

/*   MutableExplicitID types   */
template <> class ShapeFiller<Shape1MutableExplicitID> {
public:
    static void fill_data(Shape1MutableExplicitID *data, const char *color, int count) {
        Shape1Filler<Shape1MutableExplicitID>::fill_data(data, color, count);
     }
};

template <> class ShapeFiller<Shape2MutableExplicitID> {
public:
    static void fill_data(Shape2MutableExplicitID *data, const char *color, int count) {
        Shape2Filler<Shape2MutableExplicitID>::fill_data(data, color, count);
    };
};

template <> class ShapeFiller<Shape3MutableExplicitID> {
public:
    static void fill_data(Shape3MutableExplicitID *data, const char *color, int count) {
        Shape3Filler<Shape3MutableExplicitID>::fill_data(data, color, count);
    }
};

template <> class ShapeFiller<Shape4MutableExplicitID> {
public:
    static void fill_data(Shape4MutableExplicitID *data, const char *color, int count) {
        Shape4Filler<Shape4MutableExplicitID>::fill_data(data, color, count);
    }
};

template <> class ShapeFiller<Shape5MutableExplicitID> {
public:
    static void fill_data(Shape5MutableExplicitID *data, const char *color, int count) {
        Shape5Filler<Shape5MutableExplicitID>::fill_data(data, color, count);
    }
};


class WriterBase {
public:
    virtual bool initialize(DDSDomainParticipant *participant, const char *topic_name) = 0;
    virtual bool write_data(const char *color, int count) = 0;
    virtual DDSTopic *get_topic() = 0;
};

template <typename T, typename TSupport, typename TDataWriter>
class Writer : public WriterBase {
public:
    typedef T data_type;
    typedef TSupport type_support;
    typedef TDataWriter data_writer;

  public:
    virtual bool initialize(DDSDomainParticipant *participant, const char *topic_name) {
        DDS_ReturnCode_t retcode;
        retcode = TSupport::register_type(participant, TSupport::get_type_name());
        if ( retcode != DDS_RETCODE_OK) {
            fprintf(stderr, "Failed to register type for topic \"%s\" retcode=%d\n", topic_name, retcode);
            return false;
        }
         
        DDSPublisher *publisher = participant->create_publisher(
            DDS_PUBLISHER_QOS_DEFAULT, NULL /* listener */, DDS_STATUS_MASK_NONE);
        if (publisher == NULL) {
            printf("create_publisher error for topic \"%s\"\n", topic_name);
            return false;
        }

        _topic = participant->create_topic(
            topic_name, TSupport::get_type_name(),
            DDS_TOPIC_QOS_DEFAULT, new TheTopicListener(),
            DDS_INCONSISTENT_TOPIC_STATUS);
        if (_topic == NULL) {
            printf("create_topic error for topic \"%s\"\n", topic_name);
            return false;
        }

        DDSDataWriter *writer = publisher->create_datawriter(
            _topic, DDS_DATAWRITER_QOS_DEFAULT, NULL /* listener */,
            DDS_STATUS_MASK_NONE);
        if (writer == NULL) {
            printf("create_datawriter error for topic \"%s\"\n", topic_name);
            return false;
        }

        _writer = TDataWriter::narrow(writer);
        if (_writer == NULL) {
            printf("DataWriter narrow error for topic \"%s\"\n", topic_name);
            return false;
        }

        _data = TSupport::create_data();
        return true;
    }

    virtual bool write_data(const char *color, int count) {
        _shapeFiller.fill_data(_data, color, count);
        printf("Writing Topic \"%s\", type \"%s\", count %d, data:", 
               _writer->get_topic()->get_name(),
               _writer->get_topic()->get_type_name(), count);
        TSupport::print_data(_data);
        return _writer->write(*_data, DDS_HANDLE_NIL) == DDS_RETCODE_OK;
    }

    virtual DDSTopic *get_topic() { return _topic;  }

private:
    TDataWriter *_writer;
    DDSTopic *_topic;
    T *_data;
    ShapeFiller<T> _shapeFiller;
};

class ReaderBase {
public:
    virtual bool initialize(DDSDomainParticipant *participant, const char *topic_name) = 0;
    virtual bool take_data() = 0;
    virtual DDSTopic *get_topic() = 0;
    virtual bool wait_for_data(DDS_Duration_t timeout) = 0;
};


class TheReaderListener : public DDSDataReaderListener {
  public:    
    virtual void on_requested_incompatible_qos(
        DDSDataReader* reader,
        const DDS_RequestedIncompatibleQosStatus& status) {
           fprintf(stderr, "on_requested_incompatible_qos: topic \"%s\", type \"%s\" last_policy: %d, count: %d, change: %d\n",
                    reader->get_topicdescription()->get_name(),
                    reader->get_topicdescription()->get_type_name(), 
                    status.last_policy_id,
                    status.total_count, status.total_count_change);
    }

    virtual void on_subscription_matched(
        DDSDataReader* reader,
        const DDS_SubscriptionMatchedStatus& status) {
             fprintf(stderr, "on_subscription_matched: topic \"%s\", type \"%s\", count: %d, change: %d\n",
                    reader->get_topicdescription()->get_name(),
                    reader->get_topicdescription()->get_type_name(), 
                    status.current_count, status.current_count_change);
    }
};

template <typename T, typename TSupport, typename TDataReader>
class Reader : public ReaderBase {
public:
    typedef T data_type;
    typedef TSupport type_support_type;
    typedef TDataReader data_reader_type;

  public:
    bool initialize(DDSDomainParticipant *participant, const char *topic_name) {
        DDS_ReturnCode_t retcode;
        retcode = TSupport::register_type(participant, TSupport::get_type_name());
        if ( retcode != DDS_RETCODE_OK) {
            fprintf(stderr, "Failed to register type for topic \"%s\" retcode=%d\n", topic_name, retcode);
            return false;
        }
         
        DDSSubscriber *subscriber = participant->create_subscriber(
            DDS_SUBSCRIBER_QOS_DEFAULT, NULL /* listener */, DDS_STATUS_MASK_NONE);
        if (subscriber == NULL) {
            printf("create subscriber error for topic \"%s\"\n", topic_name);
            return false;
        }

        _topic = participant->create_topic(
            topic_name, TSupport::get_type_name(),
            DDS_TOPIC_QOS_DEFAULT, new TheTopicListener(),
            DDS_INCONSISTENT_TOPIC_STATUS );
        if (_topic == NULL) {
            printf("create topic error for topic \"%s\"\n", topic_name);
            return false;
        }

        DDSDataReader *reader = subscriber->create_datareader(
            _topic, DDS_DATAREADER_QOS_DEFAULT, new TheReaderListener(),
            DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS | DDS_SUBSCRIPTION_MATCHED_STATUS);
        if (reader == NULL) {
            printf("create datareader error for topic \"%s\"\n", topic_name);
            return false;
        }

        _reader = TDataReader::narrow(reader);
        if (_reader == NULL) {
            printf("DataWriter narrow error for topic \"%s\"\n", topic_name);
            return false;
        }

        _data = TSupport::create_data();

        _waitset.attach_condition(_reader->get_statuscondition());
        return true;
    }

    bool take_data() {
        DDS_SampleInfo info;
        DDS_ReturnCode_t retcode;

        do {
            retcode = _reader->take_next_sample(*_data, info);
            if ( (retcode == DDS_RETCODE_OK ) && info.valid_data ) {
                printf("\nRead data for Topic %s", _reader->get_topicdescription()->get_name()); 
                TSupport::print_data(_data);
            }
            else if (retcode == DDS_RETCODE_NO_DATA) {
                return true; // all done reading return success
            }
        }
        while ( retcode == DDS_RETCODE_OK );
        
        fprintf(stderr, "Take error %d for Topic %s\n",retcode,  _reader->get_topicdescription()->get_name()); 
        return false; // done reading but return error
    }

    DDSTopic *get_topic() { return _topic;  }

    bool wait_for_data(DDS_Duration_t timeout) {
        DDSConditionSeq active_cond;
        return _waitset.wait(active_cond, timeout) == DDS_RETCODE_OK;
    }

private:
    TDataReader *_reader;
    DDSTopic *_topic;
    T *_data;
    DDSWaitSet _waitset;
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

        //  else 
        fprintf(stderr, "create_reader: Unrecognized type: \"%s\"\n", type_name);
        return NULL;
    }

    static WriterBase *create_writer(const char *type_name) {
        if ( strcmp(type_name, "Shape1Default") == 0 ) {
            return new Writer<Shape1Default, Shape1DefaultTypeSupport, Shape1DefaultDataWriter>();
        }
        else if ( strcmp(type_name, "Shape1Final") == 0 ) {
            return new Writer<Shape1Final, Shape1FinalTypeSupport, Shape1FinalDataWriter>();
        }
        else if ( strcmp(type_name, "Shape1Extensible") == 0 ) {
            return new Writer<Shape1Extensible, Shape1ExtensibleTypeSupport, Shape1ExtensibleDataWriter>();
        }
        else if ( strcmp(type_name, "Shape1Mutable") == 0 ) {
            return new Writer<Shape1Mutable, Shape1MutableTypeSupport, Shape1MutableDataWriter>();
        }
        else if ( strcmp(type_name, "Shape1MutableExplicitID") == 0 ) {
            return new Writer<Shape1MutableExplicitID, Shape1MutableExplicitIDTypeSupport, Shape1MutableExplicitIDDataWriter>();
        }
        else if (strcmp(type_name, "Shape2Default") == 0 ) {
            return new Writer<Shape2Default, Shape2DefaultTypeSupport, Shape2DefaultDataWriter>();
        }
        else if (strcmp(type_name, "Shape2Final") == 0 ) {
            return new Writer<Shape2Final, Shape2FinalTypeSupport, Shape2FinalDataWriter>();
        }
        else if (strcmp(type_name, "Shape2Extensible") == 0 ) {
            return new Writer<Shape2Extensible, Shape2ExtensibleTypeSupport, Shape2ExtensibleDataWriter>();
        }
        else if (strcmp(type_name, "Shape2Mutable") == 0 ) {
            return new Writer<Shape2Mutable, Shape2MutableTypeSupport, Shape2MutableDataWriter>();
        }
        else if (strcmp(type_name, "Shape2MutableExplicitID") == 0 ) {
            return new Writer<Shape2MutableExplicitID, Shape2MutableExplicitIDTypeSupport, Shape2MutableExplicitIDDataWriter>();
        }
        else if (strcmp(type_name, "Shape3Default") == 0 ) {
            return new Writer<Shape3Default, Shape3DefaultTypeSupport, Shape3DefaultDataWriter>();
        }
        else if (strcmp(type_name, "Shape3Final") == 0 ) {
            return new Writer<Shape3Final, Shape3FinalTypeSupport, Shape3FinalDataWriter>();
        }
        else if (strcmp(type_name, "Shape3Extensible") == 0 ) {
            return new Writer<Shape3Extensible, Shape3ExtensibleTypeSupport, Shape3ExtensibleDataWriter>();
        }
        else if (strcmp(type_name, "Shape3Mutable") == 0 ) {
            return new Writer<Shape3Mutable, Shape3MutableTypeSupport, Shape3MutableDataWriter>();
        }
        else if (strcmp(type_name, "Shape3MutableExplicitID") == 0 ) {
            return new Writer<Shape3MutableExplicitID, Shape3MutableExplicitIDTypeSupport, Shape3MutableExplicitIDDataWriter>();
        }
        else if (strcmp(type_name, "Shape4Default") == 0 ) {
            return new Writer<Shape4Default, Shape4DefaultTypeSupport, Shape4DefaultDataWriter>();
        }
        else if (strcmp(type_name, "Shape4Final") == 0 ) {
            return new Writer<Shape4Final, Shape4FinalTypeSupport, Shape4FinalDataWriter>();
        }
        else if (strcmp(type_name, "Shape4Extensible") == 0 ) {
            return new Writer<Shape4Extensible, Shape4ExtensibleTypeSupport, Shape4ExtensibleDataWriter>();
        }
        else if (strcmp(type_name, "Shape4Mutable") == 0 ) {
            return new Writer<Shape4Mutable, Shape4MutableTypeSupport, Shape4MutableDataWriter>();
        }
        else if (strcmp(type_name, "Shape4MutableExplicitID") == 0 ) {
            return new Writer<Shape4MutableExplicitID, Shape4MutableExplicitIDTypeSupport, Shape4MutableExplicitIDDataWriter>();
        }
        else if (strcmp(type_name, "Shape5Default") == 0 ) {
            return new Writer<Shape5Default, Shape5DefaultTypeSupport, Shape5DefaultDataWriter>();
        }
        else if (strcmp(type_name, "Shape5Final") == 0 ) {
            return new Writer<Shape5Final, Shape5FinalTypeSupport, Shape5FinalDataWriter>();
        }
        else if (strcmp(type_name, "Shape5Extensible") == 0 ) {
            return new Writer<Shape5Extensible, Shape5ExtensibleTypeSupport, Shape5ExtensibleDataWriter>();
        }
        else if (strcmp(type_name, "Shape5Mutable") == 0 ) {
            return new Writer<Shape5Mutable, Shape5MutableTypeSupport, Shape5MutableDataWriter>();
        }
        else if (strcmp(type_name, "Shape5MutableExplicitID") == 0 ) {
            return new Writer<Shape5MutableExplicitID, Shape5MutableExplicitIDTypeSupport, Shape5MutableExplicitIDDataWriter>();
        }


        //  else 
        fprintf(stderr, "create_Writer: Unrecognized type: \"%s\"\n", type_name);
        return NULL;
    }
};

