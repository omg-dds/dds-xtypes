//
//  TestHelloWorld.cxx
//  xtypes-interop
//
//  Created by Kelvin Zhang on 5/29/18.
//

#include "DDS_Variants.h"

#include <gtest/gtest.h>
#include "TestController.h"
#include "Utils.h"

//A class inhereted from testing::test is called a "Fixture", which
//can contain any number of tests.

//The process which runs Google Test framework is the master process.
//The master process will start first and launch slave processes during test.
//To reduce overhead of creating processes, a slave process is launched for
//every fixture, not for every test. Once a slave process is launched, it will
//stay around until all tests of that fixture run.

class TestHelloWorld : public ::testing::Test
{
public:
    static DDSDomainParticipant *participant;
    static DDSPublisher         *publisher;
    static DDSSubscriber        *subscriber;
    static TestController       *controller;
    
    //Run only ONCE across all tests under TestHelloWorld fixture
    static void SetUpTestCase()
    {
        //Reuse these entities across tests
        init_entities(participant, publisher, subscriber, controller, true);
        int ret = 0;
        ret = Point2DFinalTypeSupport::register_type(participant, Point2DFinalTypeSupport::get_type_name());
        ASSERT_EQ(ret, DDS_RETCODE_OK);
        controller->clear_msg();
        
        //Launch the helper process
        //Once started, the helper will try to write a data sample
        launch_helper("./TestHelloWorldHelper");
    }
    static void TearDownTestCase()
    {
        destroy_entities(participant, publisher, subscriber, controller);
    }
};

DDSDomainParticipant    *TestHelloWorld::participant;
DDSPublisher          *TestHelloWorld::publisher;
DDSSubscriber          *TestHelloWorld::subscriber;
TestController          *TestHelloWorld::controller;


//This test determines if the master and slave can communicate.
//The same type will be used on both the master and slave process.
//In this test, the slave process will be the DataWriter,
//and the master process will be the DataReader.
TEST_F(TestHelloWorld, SimpleTest)
{
    ASSERT_NE(participant, nullptr);
    int ret = 0;
    
    //The helper will write to this topic name
    DDSTopic *topic = participant->create_topic(
          HELLOWORLD_TOPIC,
          Point2DFinalTypeSupport::get_type_name(),
          DDS_TOPIC_QOS_DEFAULT,
          NULL,
          DDS_STATUS_MASK_NONE);
    ASSERT_NE(topic, nullptr);
    
    DDSDataReader *reader = subscriber->create_datareader(
           topic,
           get_test_datareader_default_qos(),
           NULL,
           DDS_STATUS_MASK_NONE);
    ASSERT_NE(reader, nullptr);

    //Slave process will use this type as well
    Point2DFinalDataReader *point_reader = Point2DFinalDataReader::narrow(reader);
    ASSERT_NE(point_reader, nullptr) << "failed to narrow reader";
    
    //wait for data available status
    DDSStatusCondition *read_condition = reader->get_statuscondition();
    read_condition->set_enabled_statuses(DDS_DATA_AVAILABLE_STATUS);
    DDSWaitSet waitset;
    ret = waitset.attach_condition(read_condition);
    ASSERT_EQ(ret, DDS_RETCODE_OK);
    
    DDSConditionSeq active_condition;
    active_condition.maximum(1);

    //wait 3 seconds for data to arrive.
    DDS_Duration_t timeout = {3, 0};
    ret = waitset.wait(active_condition, timeout);
    
    //Tell the slave process that the master is done
    //so that slave can teardown/cleanup and exit   .
    controller->send_ack();
    
    //Read a data sample
    ASSERT_EQ(ret, DDS_RETCODE_OK);
    Point2DFinal received_data;
    DDS_SampleInfo info;
    ret = point_reader->take_next_sample(received_data, info);
    ASSERT_EQ(ret, DDS_RETCODE_OK);
    ASSERT_EQ(info.valid_data, DDS_BOOLEAN_TRUE);
}
