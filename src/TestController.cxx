//
//  TestController.cxx
//  xtypes-interop
//
//  Created by Kelvin Zhang on 5/29/18.
//

#include "TestController.h"
#include "Utils.h"
#include <iostream>


TestController::TestController(
        DDSDomainParticipant *participant,
        DDSPublisher *publisher,
        DDSSubscriber *subscriber,
        bool isMaster) :
         participant(participant), publisher(publisher), subscriber(subscriber), seq(0)
{
    if (publisher == NULL) {
        throw std::invalid_argument("publisher is NULL");
    }
    if (subscriber == NULL) {
        throw std::invalid_argument("subscriber is NULL");
    }
    const char *type_name = topic_traits<ControlMessage>::TypeSupport::get_type_name();
    int ret = 0;
    ret = topic_traits<ControlMessage>::TypeSupport::register_type(participant, type_name);
    if (ret != DDS_RETCODE_OK) {
        throw std::runtime_error("register type failed " + std::to_string(ret));
    }
    //Outbound topic is for sending data to the other side. No reading!
    //Inbound topic is for receiving data from the other side. No writing!
    if(isMaster)
    {
        outbound_topic = participant->create_topic(
               INTER_OP_CONTROL_TOPIC_MASTER_TO_SLAVE,
               type_name,
               DDS_TOPIC_QOS_DEFAULT,
               NULL,
               DDS_STATUS_MASK_NONE);
        inbound_topic = participant->create_topic(
               INTER_OP_CONTROL_TOPIC_SLAVE_TO_MASTER,
               type_name,
               DDS_TOPIC_QOS_DEFAULT,
               NULL,
               DDS_STATUS_MASK_NONE);
    }else
    {
        outbound_topic = participant->create_topic(
                INTER_OP_CONTROL_TOPIC_SLAVE_TO_MASTER,
                type_name,
                DDS_TOPIC_QOS_DEFAULT,
                NULL,
                DDS_STATUS_MASK_NONE);
        inbound_topic = participant->create_topic(
                INTER_OP_CONTROL_TOPIC_MASTER_TO_SLAVE,
                type_name,
                DDS_TOPIC_QOS_DEFAULT,
                NULL,
                DDS_STATUS_MASK_NONE);
    }
    if (outbound_topic == NULL) {
        std::string msg = "failed to create outbound topic ";
        msg += isMaster ? INTER_OP_CONTROL_TOPIC_MASTER_TO_SLAVE : INTER_OP_CONTROL_TOPIC_SLAVE_TO_MASTER;
        throw std::runtime_error(msg);
    }
    if (inbound_topic == NULL) {
        std::string msg = "failed to create inbound topic ";
        msg += isMaster ? INTER_OP_CONTROL_TOPIC_SLAVE_TO_MASTER : INTER_OP_CONTROL_TOPIC_MASTER_TO_SLAVE;
        throw std::runtime_error(msg);
    }
    
    DDS_DataWriterQos writer_qos = get_test_datawriter_default_qos();
#if WRITER_RESOURCE_LIMITS_SUPPORTED == 1
    writer_qos.writer_resource_limits.max_remote_readers = 16;
    writer_qos.writer_resource_limits.max_routes_per_reader = 16;
#endif

    writer = publisher->create_datawriter(
          outbound_topic,
          writer_qos,
          NULL,
          DDS_STATUS_MASK_NONE);
    if(writer == NULL)
        throw std::runtime_error("failed to create DataWriter");
    
    DDS_DataReaderQos reader_qos = get_test_datareader_default_qos();
#if WRITER_RESOURCE_LIMITS_SUPPORTED == 1
    reader_qos.reader_resource_limits.max_remote_writers = 16;
    reader_qos.reader_resource_limits.max_routes_per_writer = 16;
#endif

    
    reader = subscriber->create_datareader(
        inbound_topic,
        reader_qos,
        NULL,
        DDS_STATUS_MASK_NONE);
    if(reader == NULL)
        throw std::runtime_error("failed to create DataReader");
    waitset = new DDSWaitSet();
    status_condition = reader->get_statuscondition();
    status_condition->set_enabled_statuses(DDS_DATA_AVAILABLE_STATUS);
    waitset->attach_condition(status_condition);
    
    //Slave process will monitor subscription match/unmatch event just in case
    //the master process exit prematurely without sending TEST_ACTION_EXIT
    if (!isMaster) {
        DDSStatusCondition *writer_status_condition = writer->get_statuscondition();
        writer_status_condition->set_enabled_statuses(DDS_SUBSCRIPTION_MATCHED_STATUS);
        waitset->attach_condition(writer_status_condition);
    }
}

TestController::~TestController()
{
    waitset->detach_condition(status_condition);
    delete waitset;
    publisher->delete_datawriter(writer);
    subscriber->delete_datareader(reader);
    participant->delete_topic(inbound_topic);
    participant->delete_topic(outbound_topic);
}

ControlMessage TestController::peek_msg()
{
    DDSConditionSeq active_condition;
    int ret = 0;
    ret = waitset->wait(active_condition, DDS_DURATION_INFINITE);
    if (ret != DDS_RETCODE_OK)
        throw std::runtime_error("waitset->wait() failed " + std::to_string(ret));
    
    ControlMessageDataReader *control_message_reader = ControlMessageDataReader::narrow(reader);
    if(control_message_reader == NULL)
        throw std::runtime_error("failed to narrow control_message_reader");
    
    ControlMessage received_message;
    DDS_SampleInfo info;
    control_message_reader->read_next_sample(received_message, info);
    return received_message;
}

ControlMessage TestController::recv_msg(DDS_Duration_t duration)
{
    DDSConditionSeq active_condition;
    
    active_condition.maximum(2);
    
    int ret = 0;
    while (true) {
        ret = waitset->wait(active_condition, duration);
        if (ret == DDS_RETCODE_TIMEOUT)
            throw std::runtime_error("receive msg time out");
        if (ret != DDS_RETCODE_OK)
            throw std::runtime_error("waitset->wait() failed " + std::to_string(ret));
        
        //Check whether DATA_AVAILABLE or SUBSCRIPTION_MATCHED is triggered
        bool data_available = false;
        for(int i = 0; i < active_condition.length(); i ++) {
            if (active_condition[i] == status_condition) {
                data_available = true;
                break;
            } else {
                //If SUBSCRIPTION_MATCHED is triggered and all process readers
                //are gone, that means the master exited prematurely. No need to
                //wait anymore.
                DDS_PublicationMatchedStatus status;
                writer->get_publication_matched_status(status);
                if (status.total_count_change < 0 && status.total_count == 0) {
                    throw std::runtime_error("all readers are gone");
                }
            }
        }
        if (data_available) {
            break;
        }
    }
    
    //read a message and return it
    ControlMessageDataReader *control_message_reader = topic_traits<ControlMessage>::DataReader::narrow(reader);
    
    ControlMessage received_message;
    DDS_SampleInfo info;
    control_message_reader->take_next_sample(received_message, info);
    if (!info.valid_data)
        throw std::runtime_error("controller received invalid data " + std::to_string(info.sample_state));
    return received_message;
}

void TestController::send_msg(const ControlMessage &msg)
{
    ControlMessageDataWriter *control_message_writer = topic_traits<ControlMessage>::DataWriter::narrow(writer);
    if(control_message_writer == NULL)
        throw std::runtime_error("failed to narrow control_message_writer");
    int ret = 0;
    ret = control_message_writer->write(msg, DDS_HANDLE_NIL);
    if (ret != DDS_RETCODE_OK)
        throw std::runtime_error("control_message_writer->write() failed " + std::to_string(ret));
}

void TestController::clear_msg()
{
    ControlMessageDataReader *control_message_reader = topic_traits<ControlMessage>::DataReader::narrow(reader);
    if(control_message_reader == NULL)
        throw std::runtime_error("failed to narrow control_message_reader");
    ControlMessageSeq received_msg;
    DDS_SampleInfoSeq info_seq;
    int ret = control_message_reader->take(
           received_msg,
           info_seq,
           DDS_LENGTH_UNLIMITED,
           DDS_ANY_SAMPLE_STATE,
           DDS_ANY_VIEW_STATE,
           DDS_ANY_INSTANCE_STATE);
    
    if (ret == DDS_RETCODE_OK) {
        control_message_reader->return_loan(received_msg, info_seq);
    }
}

void TestController::send_ack()
{
    ControlMessage ack;
    ack.action = TEST_INFO_ACK;
    ack.param1._d = ack.action = TEST_INFO_ACK;
    ack.param1._u.not_used = seq++;
    this->send_msg(ack);
}

void TestController::recv_ack(DDS_Duration_t timeout)
{
    ControlMessage ack = this->recv_msg(timeout);
    if (ack.action != TEST_INFO_ACK) {
        throw std::runtime_error("unexpected control message in recv_ack(): " + std::to_string(ack.action));
    }
}

//Try to detect presence of slave process
bool TestController::isSlaveLive() {
    DDS_SubscriptionMatchedStatus subscription_status;
    reader->get_subscription_matched_status(subscription_status);
    if (subscription_status.total_count <= 0) {
        return false;
    }
    if (subscription_status.total_count > 1) {
        std::cout << "\x1B[33m Multiple slaves detected: " << subscription_status.total_count;
        std::cout << "\x1B[0m" << std::endl;
    }
    DDS_PublicationMatchedStatus publication_status;
    writer->get_publication_matched_status(publication_status);
    if (publication_status.total_count <= 0) {
        return false;
    }
    if (publication_status.total_count > 1) {
        std::cout << "\x1B[33m Multiple slaves detected: " << subscription_status.total_count;
        std::cout << "\x1B[0m" << std::endl;
    }
    return true;
}

bool TestController::wait_for_slave_discovery(DDS_Duration_t timeout)
{
    DDSStatusCondition *status_condition = writer->get_statuscondition();
    status_condition->set_enabled_statuses(DDS_PUBLICATION_MATCHED_STATUS);
    DDSWaitSet waitset;
    waitset.attach_condition(status_condition);
    DDSConditionSeq active_condition;
    active_condition.maximum(1);
    DDS_ReturnCode_t ret = waitset.wait(active_condition, timeout);
    return ret == DDS_RETCODE_OK;
}
