#ifndef __UTILS_HEADER
#define __UTILS_HEADER
#include "DDS_Variants.h"

#include <vector>
#include "TestController.h"
#include <iostream>

#if DATA_REPRESENTATION_QOS_SUPPORTED == 1
DDSDataReader *create_reader_with_data_representation(
        DDSSubscriber *subscriber,
        DDSTopic *topic,
        const DDS_DataRepresentationIdSeq &representation_id_seq);
DDSDataWriter *create_writer_with_data_representation(
        DDSPublisher *publisher,
        DDSTopic *topic,
        const DDS_DataRepresentationIdSeq &representation_id_seq);

#endif

#define LIVELINESS_TEST_ASSERT_PERIOD       1
#define LIVELINESS_TEST_LEASE_DURATION      5

DDS_DataReaderQos get_test_datareader_default_qos();
DDS_DataWriterQos get_test_datawriter_default_qos();
DDS_DomainParticipantQos get_test_participant_default_qos();

DDSSubscriber *create_subscriber(
                                 DDSDomainParticipant *participant,
                                 const DDS_SubscriberQos &qos = DDS_SUBSCRIBER_QOS_DEFAULT);
DDSPublisher *create_publisher(
                               DDSDomainParticipant *participant,
                               const DDS_PublisherQos &qos = DDS_PUBLISHER_QOS_DEFAULT);

#if DATA_REPRESENTATION_QOS_SUPPORTED == 1
DDS_DataRepresentationIdSeq deserialize_cdr_list(CDRPolicy cdr_list);
CDRPolicy serialize_cdr_list(std::vector<DDS_DataRepresentationId_t> list);
#endif

void launch_helper(std::string name);

void init_entities(
        DDSDomainParticipant *&participant,
        DDSPublisher *&publisher,
        DDSSubscriber *&subscriber,
        TestController *&controller,
        bool isMaster);

void destroy_entities(
        DDSDomainParticipant *&participant,
        DDSPublisher *&publisher,
        DDSSubscriber *&subscriber,
        TestController *&controller);

enum QosProperty : unsigned short{
    PREVENT_TYPE_WIDENING,
    IGNORE_STRING_BOUNDS,
    IGNORE_SEQUENCE_BOUNDS,
    IGNORE_MEMBER_NAMES
};

void apply_qos_properties(
        DDS_DataReaderQos qos,
        const std::vector<QosProperty> &set_to_one,
        const std::vector<QosProperty> &set_to_zero);

void apply_qos_properties(
        DDS_DataWriterQos qos,
        const std::vector<QosProperty> &set_to_one,
        const std::vector<QosProperty> &set_to_zero);

template <typename T>
void write_and_wait(
        DDSDomainParticipant *participant,
        DDSPublisher *publisher,
        TestController *controller,
        const char *topic_name,
        T *data,
        CDRPolicy policy,
        const std::vector<QosProperty> &set_to_one,
        const std::vector<QosProperty> &set_to_zero)
{
    const char *type_name = topic_traits<T>::TypeSupport::get_type_name();
    int ret = 0;
    ret = topic_traits<T>::TypeSupport::register_type(participant, type_name);
    DDSTopic *topic = participant->create_topic(
            topic_name,
            type_name,
            DDS_TOPIC_QOS_DEFAULT,
            NULL,
            DDS_STATUS_MASK_NONE);
    if (topic == NULL) {
        throw std::runtime_error("topic creation failed");
    }
    
    DDS_DataWriterQos qos = get_test_datawriter_default_qos();
    apply_qos_properties(qos, set_to_one, set_to_zero);
#if DATA_REPRESENTATION_QOS_SUPPORTED == 1
    qos.representation.value.length(1);
    if(policy == CDR_POLICY_XCDR)
        qos.representation.value[0] = DDS_XCDR_DATA_REPRESENTATION;
    else if(policy == CDR_POLICY_XCDR2)
        qos.representation.value[0] = DDS_XCDR2_DATA_REPRESENTATION;
    else
        throw std::runtime_error("incompatible CDRPolicy: " + std::to_string(policy));
#endif
    
    DDSDataWriter *writer = publisher->create_datawriter(
            topic,
            qos,
            NULL,
            DDS_STATUS_MASK_NONE);
    if (writer == NULL) {
        std::cerr << "Slave failed to create data writer in topic: " << topic->get_name() << std::endl;
    } else {
        topic_traits<T>::DataWriter::narrow(writer)->write(*data, DDS_HANDLE_NIL);
    }
    
    controller->recv_ack();
    publisher->delete_datawriter(writer);
    participant->delete_topic(topic);
    topic_traits<T>::TypeSupport::unregister_type(participant, type_name);
    controller->send_ack();
}

DDS_ReturnCode_t wait_on_status(
        DDSDataReader *reader,
        const DDS_StatusMask &mask,
        DDSConditionSeq &active_condition,
        const DDS_Duration_t &timeout = {3, 0});


#endif
