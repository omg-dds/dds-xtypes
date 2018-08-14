#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "DDS_Variants.h"

#include "TestController.h"
#include "Utils.h"

typedef std::string string;

DDSDomainParticipant *participant = NULL;
DDSPublisher *publisher = NULL;
DDSSubscriber *subscriber = NULL;
TestController *controller = NULL;
DDSDataWriter *writer = NULL;
DDSTopic        *topic = NULL;

#if DATA_REPRESENTATION_QOS_SUPPORTED == 1
void write_data(CDRPolicy cdr_policy)
{
    DDS_ReturnCode_t ret;
    assert(publisher != nullptr);
    DDS_DataRepresentationIdSeq writer_cdr = deserialize_cdr_list(cdr_policy);
    
    
    writer = ::create_writer_with_data_representation(publisher, topic, writer_cdr);
    if (writer == NULL) {
        throw std::runtime_error("failed to create a writer");
    }
    
    //write the data immediately.
    Point2DFinalDataWriter *point_writer = Point2DFinalDataWriter::narrow(writer);
    if (point_writer != NULL) {
        //master process is expecting these numbers
        Point2DFinal sample = {0, 0};
        ret = point_writer->write(sample, DDS_HANDLE_NIL);
        if (ret != DDS_RETCODE_OK) {
            std::cout << "point_writer->write() " << ret << std::endl;
        }
    }
    controller->recv_ack();
    controller->send_ack();
    if (writer != NULL) {
        publisher->delete_datawriter(writer);
        writer = NULL;
    }
}
#endif

//This program is not designed to run on its own.
//It is designed to be a child process of TestDataRepresentationQos
//The parent process will tell it what CDR representation to use
int main(int argc, const char *argv[])
{
#if DATA_REPRESENTATION_QOS_SUPPORTED == 1
    int ret = 0;

    init_entities(participant, publisher, subscriber, controller, false);
    
    const char *type_name = Point2DFinalTypeSupport::get_type_name();
    ret = Point2DFinalTypeSupport::register_type(participant, type_name);
    assert(ret == DDS_RETCODE_OK);
    
    topic = participant->create_topic(
          DATA_REPRESENTATION_QOS_TOPIC,
          type_name,
          DDS_TOPIC_QOS_DEFAULT,
          NULL,
          DDS_STATUS_MASK_NONE);
    assert(topic != NULL);
    
    //tell master process that we are ready.
    ControlMessage msg;
    msg.action = TEST_INFO_INIT_DONE;
    msg.param1._d = msg.action;
    
    controller->send_msg(msg);
    try{
        while (true) {
            ControlMessage received_data = controller->recv_msg();
            if (received_data.action == TEST_ACTION_EXIT) {
                break;
            }else if (received_data.action == TEST_ACTION_WRITE_CDR_QOS) {
                switch (received_data.param1._u.cdrQosTest) {
                    case T1_XCDR2__XCDR:
                    case T1_XCDR2__XCDR2:
                    case T1_XCDR2__XCDR_XCDR2:
                        write_data(CDR_POLICY_XCDR2);
                        break;
                    case T1_XCDR__XCDR:
                    case T1_XCDR__XCDR2:
                    case T1_XCDR__XCDR_XCDR2:
                        write_data(CDR_POLICY_XCDR);
                        break;
                    default:
                        break;
                }
            }else {
                throw std::runtime_error("unknown test action " + std::to_string(received_data.action));
            }
        }
    } catch (std::runtime_error e)
    {
        std::cerr << e.what() << std::endl;
    }
    delete controller;
    participant->delete_contained_entities();
    DDSTheParticipantFactory->delete_participant(participant);
#endif
    return -1;
}
