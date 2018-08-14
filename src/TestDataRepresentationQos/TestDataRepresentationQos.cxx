#include <vector>
#include <gtest/gtest.h>

#include "DDS_Variants.h"
#include "Utils.h"
#include "TestController.h"


#if DATA_REPRESENTATION_QOS_SUPPORTED == 1
class TestDataRepresentationQosPolicy : public ::testing::Test
{
public:
    static DDSDomainParticipant *participant;
    static DDSTopic *control_topic;
    static DDSTopic *topic;
    
    static DDSSubscriber *subscriber;
    static DDSPublisher *publisher;
    
    //used to communicate test commands
    static TestController *controller;
    
    DDSDataReader *reader;
    
    static void SetUpTestCase()
    {
        launch_helper("./TestDataRepresentationQosHelper");
        DDS_ReturnCode_t ret = DDS_RETCODE_OK;
        init_entities(participant, publisher, subscriber, controller, true);
        controller->clear_msg();
        
        ret = Point2DFinalTypeSupport::register_type(participant, Point2DFinalTypeSupport::get_type_name());
        ASSERT_EQ(ret, DDS_RETCODE_OK);
        
        topic = participant->create_topic(
              DATA_REPRESENTATION_QOS_TOPIC,
              Point2DFinalTypeSupport::get_type_name(),
              DDS_TOPIC_QOS_DEFAULT,
              NULL,
              DDS_STATUS_MASK_NONE);
        
        ControlMessage msg = controller->recv_msg();
        ASSERT_EQ (msg.action, TEST_INFO_INIT_DONE) << "expect init_done msg from slave";
    }
    
    static void TearDownTestCase()
    {
        if (topic != NULL) {
            const char *type_name = topic->get_type_name();
            participant->delete_topic(topic);
            topic = NULL;
            participant->unregister_type(type_name);
        }
        destroy_entities(participant, publisher, subscriber, controller);
    }
    
    virtual void SetUp() override
    {
        subscriber = participant->create_subscriber(
                DDS_SUBSCRIBER_QOS_DEFAULT,
                NULL,
                DDS_STATUS_MASK_NONE);
    }
    
    virtual void TearDown() override
    {
        controller->send_ack();
        controller->recv_ack();
        if (reader != NULL) {
            subscriber->delete_datareader(reader);
            reader = NULL;
        }
    }
    
    static DDSDataReader *create_reader_with_data_representation(
        const DDS_DataRepresentationIdSeq &representation_id_seq)
    {
        return ::create_reader_with_data_representation(subscriber, topic, representation_id_seq);
    }

    static DDSDataWriter *create_writer_with_data_representation(
        const DDS_DataRepresentationIdSeq &representation_id_seq)
    {
        return ::create_writer_with_data_representation(publisher, topic, representation_id_seq);
    }
    
    void test_cdr_compatibility(CDRQosTest test_id, CDRPolicy requested_cdr, bool expect_communication);
};

DDSDomainParticipant *TestDataRepresentationQosPolicy::participant;

DDSTopic *TestDataRepresentationQosPolicy::topic;
DDSTopic *TestDataRepresentationQosPolicy::control_topic;

DDSSubscriber *TestDataRepresentationQosPolicy::subscriber;
DDSPublisher *TestDataRepresentationQosPolicy::publisher;


TestController *TestDataRepresentationQosPolicy::controller;


void TestDataRepresentationQosPolicy::test_cdr_compatibility(CDRQosTest test_id, CDRPolicy requested_cdr, bool expect_communication)
{
    ASSERT_NE(subscriber, nullptr);
    ASSERT_NE(topic, nullptr);
    
    DDS_DataRepresentationIdSeq reader_cdr = deserialize_cdr_list(requested_cdr);
    
    // Create a DataReader with specified data representation qos
    reader = create_reader_with_data_representation(reader_cdr);
    ASSERT_NE(reader, nullptr);
    
    DDSWaitSet waitset;

    DDS_ReturnCode_t ret = DDS_RETCODE_OK;
    DDSReadCondition *read_condition = NULL;
    DDSStatusCondition *status_condition = NULL;
    if (expect_communication) {
        
        //if they are supposed to be compatible, register a listern for data
        read_condition = reader->create_readcondition(
              DDS_ANY_SAMPLE_STATE,
              DDS_ANY_VIEW_STATE,
              DDS_ANY_INSTANCE_STATE);
        
        ret = waitset.attach_condition(read_condition);
        ASSERT_EQ(ret, DDS_RETCODE_OK);
    } else {
        //wait for offered_incompatible_qos to update, then verify
        //that offered_incompatible_qos.total_count() is correct
        
        status_condition = reader->get_statuscondition();
        ASSERT_NE(status_condition, nullptr);
        status_condition->set_enabled_statuses(
                    DDS_OFFERED_INCOMPATIBLE_QOS_STATUS |
                    DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS);
        ret = waitset.attach_condition(status_condition);
        ASSERT_EQ(ret, DDS_RETCODE_OK);
    }
    
    //tell child process to create a writer with specified cdr policy
    ControlMessage control_message;
    control_message.action = TEST_ACTION_WRITE_CDR_QOS;
    control_message.param1._d = control_message.action;
    control_message.param1._u.cdrQosTest = test_id;
    controller->send_msg(control_message);
    
    DDSConditionSeq active_condition;
    DDS_Duration_t timeout = {3, 0};
    ret = waitset.wait(active_condition, timeout);
    ASSERT_EQ(ret, DDS_RETCODE_OK);
    if (expect_communication) {
        //read a sample
        Point2DFinalDataReader *point_reader = Point2DFinalDataReader::narrow(reader);
        ASSERT_NE(point_reader, nullptr);
        Point2DFinal sample;
        DDS_SampleInfo info;
        ret = point_reader->take_next_sample(sample, info);
        ASSERT_EQ(ret, DDS_RETCODE_OK) << "expect data from DataReader";
        ret = waitset.detach_condition(read_condition);
        ASSERT_EQ(ret, DDS_RETCODE_OK);
        ASSERT_TRUE(info.valid_data);
        
        reader->delete_readcondition(read_condition);
        
        DDS_RequestedIncompatibleQosStatus status;
        ret = reader->get_requested_incompatible_qos_status(status);
        ASSERT_EQ(ret, DDS_RETCODE_OK);
        ASSERT_EQ(status.total_count, 0);
    } else {
        DDS_RequestedIncompatibleQosStatus status;
        ret = reader->get_requested_incompatible_qos_status(status);
        waitset.detach_condition(status_condition);
        ASSERT_EQ(ret, DDS_RETCODE_OK);
        ASSERT_EQ(status.total_count, 1);
    }
}

TEST_F(TestDataRepresentationQosPolicy, T1_XCDR__TO__XCDR)
{
    test_cdr_compatibility(T1_XCDR__XCDR, CDR_POLICY_XCDR, true);
}

TEST_F(TestDataRepresentationQosPolicy, T1_XCDR__TO__XCDR2)
{
    test_cdr_compatibility(T1_XCDR__XCDR2, CDR_POLICY_XCDR2, false);
}

TEST_F(TestDataRepresentationQosPolicy, T1_XCDR__TO__XCDR_AND_XCDR2)
{
    test_cdr_compatibility(T1_XCDR__XCDR_XCDR2, CDR_POLICY_XCDR_XCDR2, true);
}

TEST_F(TestDataRepresentationQosPolicy, T1_XCDR2__TO__XCDR)
{
    test_cdr_compatibility(T1_XCDR2__XCDR, CDR_POLICY_XCDR, false);
}

TEST_F(TestDataRepresentationQosPolicy, T1_XCDR2__TO__XCDR_AND_XCDR2)
{
    test_cdr_compatibility(T1_XCDR2__XCDR_XCDR2, CDR_POLICY_XCDR_XCDR2, true);
}

TEST_F(TestDataRepresentationQosPolicy, T1_XCDR2__TO__XCDR2)
{
    test_cdr_compatibility(T1_XCDR2__XCDR2, CDR_POLICY_XCDR2, true);
}
#endif
