//
//  TestExtensibility.cxx
//  xtypes-interop
//
//  Created by Kelvin Zhang on 5/31/18.
//


#include <gtest/gtest.h>

#include "TestController.h"
#include "Utils.h"
#include "DDS_Variants.h"


class TestExtensibility : public ::testing::Test
{
public:
    static DDSDomainParticipant *participant;
    static TestController       *controller;
    static DDSPublisher         *publisher;
    static DDSSubscriber        *subscriber;
    DDSTopic                    *topic;
    DDSDataReader               *reader;
    
    static void SetUpTestCase()
    {
        init_entities(participant, publisher, subscriber, controller, true);
        launch_helper("./TestExtensibilityHelper");
    }
    
    static void TearDownTestCase()
    {
        destroy_entities(participant, publisher, subscriber, controller);
    }
    
    virtual void SetUp() override
    {
    }
    
    virtual void TearDown() override
    {
        if (reader != NULL) {
            subscriber->delete_datareader(reader);
            reader = NULL;
        }

        if (topic != NULL) {
            const char *type_name = topic->get_type_name();
            participant->delete_topic(topic);
            topic = NULL;
            participant->unregister_type(type_name);
        }
    }
    
    template <typename T1, typename T2>
    void test_types_compatibility(
            bool expect_compatible,
            ExtensibilityKind test_id,
            std::initializer_list<QosProperty> set_to_one = {},
            std::initializer_list<QosProperty> set_to_zero = {});
};

DDSDomainParticipant    *TestExtensibility::participant;
DDSPublisher            *TestExtensibility::publisher;
DDSSubscriber           *TestExtensibility::subscriber;
TestController          *TestExtensibility::controller;


//test compatibility with writer type T1, reader type T2
template <typename T1, typename T2>
void TestExtensibility::test_types_compatibility(
        bool expect_compatible,
        ExtensibilityKind test_id,
        std::initializer_list<QosProperty> set_to_one,
        std::initializer_list<QosProperty> set_to_zero)
{
    ASSERT_NE(subscriber, nullptr);
    const char *type_name = topic_traits<T2>::TypeSupport::get_type_name();
    const char *topic_name = topic_traits<T1>::TypeSupport::get_type_name();
    int ret = 0;
    
    ret = topic_traits<T2>::TypeSupport::register_type(participant, type_name);
    ASSERT_EQ(ret, DDS_RETCODE_OK);
    topic = participant->create_topic(
            topic_name,
            type_name,
            DDS_TOPIC_QOS_DEFAULT,
            NULL,
            DDS_STATUS_MASK_NONE);
    ASSERT_NE(topic, nullptr);
    
    DDS_DataReaderQos qos = get_test_datareader_default_qos();
    apply_qos_properties(qos, set_to_one, set_to_zero);
    
    reader = subscriber->create_datareader(
           topic,
           qos,
           NULL,
           DDS_STATUS_MASK_NONE);
    
    ASSERT_NE(reader, nullptr);
    
    DDSWaitSet waitset;

    DDSStatusCondition *reader_status_condition = reader->get_statuscondition();
    reader_status_condition->set_enabled_statuses(DDS_DATA_AVAILABLE_STATUS);
    ASSERT_NE(reader_status_condition, nullptr);
    
    DDSStatusCondition *topic_status_condition = topic->get_statuscondition();
    ASSERT_NE(topic_status_condition, nullptr);
    topic_status_condition->set_enabled_statuses(DDS_INCONSISTENT_TOPIC_STATUS);
    
    
    ret = waitset.attach_condition(reader_status_condition);
    ASSERT_EQ(ret, DDS_RETCODE_OK);
    ret = waitset.attach_condition(topic_status_condition);
    ASSERT_EQ(ret, DDS_RETCODE_OK);
    
    DDSConditionSeq active_condition;
    active_condition.maximum(2);
    controller->clear_msg();
    
    ControlMessage msg;
    msg.action = TEST_ACTION_WRITE_EXTENSIBILITY_KIND;
    msg.param1._d = msg.action;
    msg.param1._u.kind = test_id;
    controller->send_msg(msg);
    
    DDS_Duration_t timeout = {3, 0};
    ret = waitset.wait(active_condition, timeout);
    waitset.detach_condition(reader_status_condition);
    waitset.detach_condition(topic_status_condition);
    
    //Tell the slave that this test is done
    controller->send_ack();
    controller->recv_ack();
//    ASSERT_EQ(ret, DDS_RETCODE_OK);
    
    if (expect_compatible) {
        typename topic_traits<T2>::DataReader *type_reader = topic_traits<T2>::DataReader::narrow(reader);
        T2 *data = create_data<T2>();
        DDS_SampleInfo info;
        ret = type_reader->take_next_sample(*data, info);
        delete_data<T2>(data);
        
        ASSERT_EQ(active_condition.length(), 1);
        ASSERT_EQ(active_condition[0], reader_status_condition) <<
                "expect data available, but inconsistent topic triggered";
        
        ASSERT_EQ(ret, DDS_RETCODE_OK);
        ASSERT_TRUE(info.valid_data);
    } else {
        DDS_InconsistentTopicStatus status;
        topic->get_inconsistent_topic_status(status);

        ASSERT_EQ(active_condition.length(), 1);
        ASSERT_EQ(active_condition[0], topic_status_condition) <<
                "expect inconsistent topic, but available triggered";
        ASSERT_EQ(status.total_count, 1);
    }
}

//=======================Test 4.2.1===============================

//Writer with Shape1Final

TEST_F(TestExtensibility, T4_2_1_Shape1Final__Shape1Final)
{
    test_types_compatibility<Shape1Final, Shape1Final>(true, T4_2_1_SHAPE1FINAL_SHAPE1FINAL);
}

TEST_F(TestExtensibility, T4_2_1_Shape1Final__Shape1Appendable)
{
    test_types_compatibility<Shape1Final, Shape1Appendable>(false, T4_2_1_SHAPE1FINAL_SHAPE1APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_1_Shape1Final__Shape1Mutable)
{
    test_types_compatibility<Shape1Final, Shape1Mutable>(false, T4_2_1_SHAPE1FINAL_SHAPE1APPENDABLE);
}


//Writer with Shape1Appendable

TEST_F(TestExtensibility, T4_2_1_Shape1Appendable__Shape1Appendable)
{
    test_types_compatibility<Shape1Appendable, Shape1Appendable>(true, T4_2_1_SHAPE1APPENDABLE_SHAPE1APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_1_Shape1Appendable__Shape1Mutable)
{
    test_types_compatibility<Shape1Appendable, Shape1Mutable>(false, T4_2_1_SHAPE1APPENDABLE_SHAPE1MUTABLE);
}

TEST_F(TestExtensibility, T4_2_1_Shape1Appendable__Shape1Default)
{
    test_types_compatibility<Shape1Appendable, Shape1Default>(true, T4_2_1_SHAPE1APPENDABLE_SHAPE1DEFAULT);
}

TEST_F(TestExtensibility, T4_2_1_Shape1Mutable__Shape1Mutable)
{
    test_types_compatibility<Shape1Mutable, Shape1Mutable>(true, T4_2_1_SHAPE1MUTABLE_SHAPE1MUTABLE);
}

TEST_F(TestExtensibility, T4_2_1_Shape1Mutable__Shape1Default)
{
    test_types_compatibility<Shape1Mutable, Shape1Default>(false, T4_2_1_SHAPE1MUTABLE_SHAPE1DEFAULT);
}

//=======================Test 4.2.2===============================

TEST_F(TestExtensibility, T4_2_2_Shape1Final__Shape2Final)
{
    test_types_compatibility<Shape1Final, Shape2Final>(false, T4_2_2_SHAPE1FINAL_SHAPE2FINAL);
}

TEST_F(TestExtensibility, T4_2_2_Shape1Appendable__Shape2Appendable)
{
    test_types_compatibility<Shape1Appendable, Shape2Appendable>(true, T4_2_2_SHAPE1APPENDABLE_SHAPE2APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_2_Shape1Mutable__Shape2Mutable)
{
    test_types_compatibility<Shape1Mutable, Shape2Mutable>(true, T4_2_2_SHAPE1MUTABLE_SHAPE2MUTABLE);
}

TEST_F(TestExtensibility, T4_2_2_Shape1Final__Shape2Final_Prevent_Widening)
{
    test_types_compatibility<Shape1Final, Shape2Final>(false, T4_2_2_SHAPE1FINAL_SHAPE2FINAL_PREVENT_WIDENING,
       {PREVENT_TYPE_WIDENING});
}

TEST_F(TestExtensibility, T4_2_2_Shape1Appendable__Shape2Appendable_Prevent_Widening)
{
    test_types_compatibility<Shape1Appendable, Shape2Appendable>(false, T4_2_2_SHAPE1APPENDABLE_SHAPE2APPENDABLE_PREVENT_WIDENING,
         {PREVENT_TYPE_WIDENING});
}

TEST_F(TestExtensibility, T4_2_2_Shape1Mutable__Shape2Mutable_Prevent_Widening)
{
    test_types_compatibility<Shape1Mutable, Shape2Mutable>(false, T4_2_2_SHAPE1MUTABLE_SHAPE2MUTABLE_PREVENT_WIDENING,
       {PREVENT_TYPE_WIDENING});
}

TEST_F(TestExtensibility, T4_2_2_Shape2Final__Shape1Final_Prevent_Widening)
{
    test_types_compatibility<Shape2Final, Shape1Final>(false, T4_2_2_SHAPE2FINAL_SHAPE1FINAL_PREVENT_WIDENING,
       {PREVENT_TYPE_WIDENING});
}

TEST_F(TestExtensibility, T4_2_2_Shape2Appendable__Shape1Appendable_Prevent_Widening)
{
    test_types_compatibility<Shape2Appendable, Shape1Appendable>(true, T4_2_2_SHAPE2APPENDABLE_SHAPE1APPENDABLE_PREVENT_WIDENING,
         {PREVENT_TYPE_WIDENING});
}

TEST_F(TestExtensibility, T4_2_2_Shape2Mutable__Shape1Mutable_Prevent_Widening)
{
    test_types_compatibility<Shape2Mutable, Shape1Mutable>(true, T4_2_2_SHAPE2MUTABLE_SHAPE1MUTABLE_PREVENT_WIDENING,
       {PREVENT_TYPE_WIDENING});
}

//=======================Test 4.2.4===============================

TEST_F(TestExtensibility, T4_2_4_Shape1Final_Shape4Final)
{
    test_types_compatibility<Shape1Mutable, Shape1Default>(false, T4_2_4_SHAPE1FINAL_SHAPE4FINAL);
}

TEST_F(TestExtensibility, T4_2_4_Shape1Appendable_Shape4Appendable)
{
    test_types_compatibility<Shape1Appendable, Shape4Appendable>(false, T4_2_4_SHAPE1APPENDABLE_SHAPE4APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_4_Shape1Mutable_Shape4Mutable)
{
    test_types_compatibility<Shape1Mutable, Shape4Mutable>(false, T4_2_4_SHAPE1MUTABLE_SHAPE4MUTABLE);
}

TEST_F(TestExtensibility, T4_2_4_Shape1MutableExplicitID_Shape4MutableExplicitID)
{
    test_types_compatibility<Shape1MutableExplicitID, Shape4MutableExplicitID>(true, T4_2_4_SHAPE1MUTABLEEXPLICITID_SHAPE4MUTABLEEXPLICITID);
}


//=======================Test 4.2.5===============================

TEST_F(TestExtensibility, T4_2_5_Shape2Final_Shape5Final)
{
    test_types_compatibility<Shape2Final, Shape5Final>(true, T4_2_5_SHAPE2FINAL_SHAPE5FINAL);
}

TEST_F(TestExtensibility, T4_2_5_Shape2Appendable_Shape5Appendable)
{
    test_types_compatibility<Shape2Appendable, Shape5Appendable>(true, T4_2_5_SHAPE2APPENDABLE_SHAPE5APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_5_Shape2Mutable_Shape5Mutable)
{
    test_types_compatibility<Shape2Mutable, Shape5Mutable>(true, T4_2_5_SHAPE2MUTABLE_SHAPE5MUTABLE);
}

//=======================Test 4.2.6===============================

TEST_F(TestExtensibility, T4_2_6_Shape1Final_Shape6Final)
{
    test_types_compatibility<Shape1Final, Shape6Final>(true, T4_2_6_SHAPE1FINAL_SHAPE6FINAL,
        {},
        {IGNORE_STRING_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_6_Shape1Appendable_Shape6Appendable)
{
    test_types_compatibility<Shape1Appendable, Shape6Appendable>(true, T4_2_6_SHAPE1APPENDABLE_SHAPE6APPENDABLE,
           {},
           {IGNORE_STRING_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_6_Shape1Mutable_Shape6Mutable)
{
    test_types_compatibility<Shape1Mutable, Shape6Mutable>(true, T4_2_6_SHAPE1MUTABLE_SHAPE6MUTABLE,
         {},
         {IGNORE_STRING_BOUNDS});
}


TEST_F(TestExtensibility, T4_2_6_Shape6Final_Shape1Final)
{
    test_types_compatibility<Shape6Final, Shape1Final>(false, T4_2_6_SHAPE6FINAL_SHAPE1FINAL,
       {},
       {IGNORE_STRING_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_6_Shape6Appendable_Shape1Appendable)
{
    test_types_compatibility<Shape6Appendable, Shape1Appendable>(false, T4_2_6_SHAPE6APPENDABLE_SHAPE1APPENDABLE,
         {},
         {IGNORE_STRING_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_6_Shape6Mutable_Shape1Mutable)
{
    test_types_compatibility<Shape6Mutable, Shape1Mutable>(false, T4_2_6_SHAPE6MUTABLE_SHAPE1MUTABLE,
           {},
           {IGNORE_STRING_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_6_Shape1Final_Shape6Final_Ignore_String_Bounds)
{
    test_types_compatibility<Shape1Final, Shape6Final>(true, T4_2_6_SHAPE1FINAL_SHAPE6FINAL_IGNORE_STRING_BOUNDS,
       {IGNORE_SEQUENCE_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_6_Shape1Appendable_Shape6Appendable_Ignore_String_Bounds)
{
    test_types_compatibility<Shape1Appendable, Shape6Appendable>(true, T4_2_6_SHAPE1APPENDABLE_SHAPE6APPENDABLE_IGNORE_STRING_BOUNDS,
           {IGNORE_SEQUENCE_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_6_Shape1Mutable_Shape6Mutable_Ignore_String_Bounds)
{
    test_types_compatibility<Shape1Mutable, Shape6Mutable>(true, T4_2_6_SHAPE1MUTABLE_SHAPE6MUTABLE_IGNORE_STRING_BOUNDS,
          {IGNORE_SEQUENCE_BOUNDS});
}


TEST_F(TestExtensibility, T4_2_6_Shape6Final_Shape1Final_Ignore_String_Bounds)
{
    test_types_compatibility<Shape6Final, Shape1Final>(true, T4_2_6_SHAPE6FINAL_SHAPE1FINAL_IGNORE_STRING_BOUNDS,
          {IGNORE_SEQUENCE_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_6_Shape6Appendable_Shape1Appendable_Ignore_String_Bounds)
{
    test_types_compatibility<Shape6Appendable, Shape1Appendable>(true, T4_2_6_SHAPE6APPENDABLE_SHAPE1APPENDABLE_IGNORE_STRING_BOUNDS,
          {IGNORE_SEQUENCE_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_6_Shape6Mutable_Shape1Mutable_Ignore_String_Bounds)
{
    test_types_compatibility<Shape6Mutable, Shape1Mutable>(true, T4_2_6_SHAPE6MUTABLE_SHAPE1MUTABLE_IGNORE_STRING_BOUNDS,
          {IGNORE_SEQUENCE_BOUNDS});
}


//=======================Test 4.2.7===============================

TEST_F(TestExtensibility, T4_2_7_Shape1Final_Shape7Final)
{
    test_types_compatibility<Shape1Final, Shape7Final>(false, T4_2_7_SHAPE1FINAL_SHAPE7FINAL,
           {},
           {IGNORE_SEQUENCE_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_7_Shape1Appendable_Shape7Appendable)
{
    test_types_compatibility<Shape1Appendable, Shape7Appendable>(false, T4_2_7_SHAPE1APPENDABLE_SHAPE7APPENDABLE,
             {},
             {IGNORE_SEQUENCE_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_7_Shape1Mutable_Shape7Mutable)
{
    test_types_compatibility<Shape1Mutable, Shape7Mutable>(false, T4_2_7_SHAPE1MUTABLE_SHAPE7MUTABLE,
           {},
           {IGNORE_SEQUENCE_BOUNDS});
}


TEST_F(TestExtensibility, T4_2_7_Shape7Final_Shape1Final)
{
    test_types_compatibility<Shape7Final, Shape1Final>(true, T4_2_7_SHAPE7FINAL_SHAPE1FINAL,
           {},
           {IGNORE_SEQUENCE_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_7_Shape7Appendable_Shape1Appendable)
{
    test_types_compatibility<Shape7Appendable, Shape1Appendable>(true, T4_2_7_SHAPE7APPENDABLE_SHAPE1APPENDABLE,
         {},
         {IGNORE_SEQUENCE_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_7_Shape7Mutable_Shape1Mutable)
{
    test_types_compatibility<Shape7Mutable, Shape1Mutable>(true, T4_2_7_SHAPE7MUTABLE_SHAPE1MUTABLE,
           {},
           {IGNORE_SEQUENCE_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_7_Shape1Final_Shape7Final_Ignore_Sequence_Bounds)
{
    test_types_compatibility<Shape1Final, Shape7Final>(true, T4_2_7_SHAPE1FINAL_SHAPE7FINAL_IGNORE_SEQUENCE_BOUNDS,
           {IGNORE_SEQUENCE_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_7_Shape1Appendable_Shape7Appendable_Ignore_Sequence_Bounds)
{
    test_types_compatibility<Shape1Appendable, Shape7Appendable>(true, T4_2_7_SHAPE1APPENDABLE_SHAPE7APPENDABLE_IGNORE_SEQUENCE_BOUNDS,
           {IGNORE_SEQUENCE_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_7_Shape1Mutable_Shape7Mutable_Ignore_Sequence_Bounds)
{
    test_types_compatibility<Shape1Mutable, Shape7Mutable>(true, T4_2_7_SHAPE1MUTABLE_SHAPE7MUTABLE_IGNORE_SEQUENCE_BOUNDS,
           {IGNORE_SEQUENCE_BOUNDS});
}


TEST_F(TestExtensibility, T4_2_7_Shape7Final_Shape1Final_Ignore_Sequence_Bounds)
{
    test_types_compatibility<Shape7Final, Shape1Final>(true, T4_2_7_SHAPE7FINAL_SHAPE1FINAL,
           {IGNORE_SEQUENCE_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_7_Shape7Appendable_Shape1Appendable_Ignore_Sequence_Bounds)
{
    test_types_compatibility<Shape7Appendable, Shape1Appendable>(true, T4_2_7_SHAPE7APPENDABLE_SHAPE1APPENDABLE_IGNORE_SEQUENCE_BOUNDS,
           {IGNORE_SEQUENCE_BOUNDS});
}

TEST_F(TestExtensibility, T4_2_7_Shape7Mutable_Shape1Mutable_Ignore_Sequence_Bounds)
{
    test_types_compatibility<Shape7Mutable, Shape1Mutable>(true, T4_2_7_SHAPE7MUTABLE_SHAPE1MUTABLE_IGNORE_SEQUENCE_BOUNDS,
           {IGNORE_SEQUENCE_BOUNDS});
}

//=======================Test 4.2.8===============================

TEST_F(TestExtensibility, T4_2_8_Shape1Final_Shape8Final_Ignore_Member_Names)
{
    test_types_compatibility<Shape1Final, Shape8Final>(true, T4_2_8_SHAPE1FINAL_SHAPE8FINAL_IGNORE_MEMBER_NAME,
        {IGNORE_MEMBER_NAMES,
        IGNORE_MEMBER_NAMES});
}

TEST_F(TestExtensibility, T4_2_8_Shape1Appendable_Shape8Appendable_Ignore_Member_Names)
{
    test_types_compatibility<Shape1Appendable, Shape8Appendable>(true, T4_2_8_SHAPE1APPENDABLE_SHAPE8APPENDABLE_IGNORE_MEMBER_NAME,
           {IGNORE_MEMBER_NAMES,
           IGNORE_MEMBER_NAMES});
}

TEST_F(TestExtensibility, T4_2_8_Shape1Mutable_Shape8Mutable_Ignore_Member_Names)
{
    test_types_compatibility<Shape1Mutable, Shape8Mutable>(true, T4_2_8_SHAPE1MUTABLE_SHAPE8MUTABLE_IGNORE_MEMBER_NAME,
         {IGNORE_MEMBER_NAMES,
         IGNORE_MEMBER_NAMES});
}

TEST_F(TestExtensibility, T4_2_8_Shape1Final_Shape8Final)
{
    test_types_compatibility<Shape1Final, Shape8Final>(false, T4_2_8_SHAPE1FINAL_SHAPE8FINAL);
}

TEST_F(TestExtensibility, T4_2_8_Shape1Appendable_Shape8Appendable)
{
    test_types_compatibility<Shape1Appendable, Shape8Appendable>(false, T4_2_8_SHAPE1APPENDABLE_SHAPE8APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_8_Shape1Mutable_Shape8Mutable)
{
    test_types_compatibility<Shape1Mutable, Shape8Mutable>(false, T4_2_8_SHAPE1MUTABLE_SHAPE8MUTABLE);
}

//=======================Test 4.2.9===============================

TEST_F(TestExtensibility, T4_2_9_Shape1Final_Shape9Final)
{
    test_types_compatibility<Shape1Final, Shape9Final>(false, T4_2_9_SHAPE1FINAL_SHAPE9FINAL);
}

TEST_F(TestExtensibility, T4_2_9_Shape1Appendable_Shape9Appendable)
{
    test_types_compatibility<Shape1Appendable, Shape9Appendable>(false, T4_2_9_SHAPE1APPENDABLE_SHAPE9APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_9_Shape1Mutable_Shape9Mutable)
{
    test_types_compatibility<Shape1Mutable, Shape9Mutable>(false, T4_2_9_SHAPE1MUTABLE_SHAPE9MUTABLE);
}

//=======================Test 4.2.10==============================

TEST_F(TestExtensibility, T4_2_10_Shape1Final_Shape10Final)
{
    test_types_compatibility<Shape1Final, Shape10Final>(false, T4_2_10_SHAPE1FINAL_SHAPE10FINAL);
}

TEST_F(TestExtensibility, T4_2_10_Shape1Appendable_Shape10Appendable)
{
    test_types_compatibility<Shape1Appendable, Shape10Appendable>(false, T4_2_10_SHAPE1APPENDABLE_SHAPE10APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_10_Shape1Mutable_Shape10Mutable)
{
    test_types_compatibility<Shape1Mutable, Shape10Mutable>(false, T4_2_10_SHAPE1MUTABLE_SHAPE10MUTABLE);
}



TEST_F(TestExtensibility, T4_2_10_Shape10Final_Shape1Final)
{
    test_types_compatibility<Shape10Final, Shape1Final>(false, T4_2_10_SHAPE10FINAL_SHAPE1FINAL);
}

TEST_F(TestExtensibility, T4_2_10_Shape10Appendable_Shape1Appendable)
{
    test_types_compatibility<Shape10Appendable, Shape1Appendable>(false, T4_2_10_SHAPE10APPENDABLE_SHAPE1APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_10_Shape10Mutable_Shape1Mutable)
{
    test_types_compatibility<Shape10Mutable, Shape1Mutable>(false, T4_2_10_SHAPE10MUTABLE_SHAPE1MUTABLE);
}

//=======================Test 4.2.11==============================

//                These 3 tests currently do not work!
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
TEST_F(TestExtensibility, T4_2_11_Parameter1Final_Parameter3Final)
{
    test_types_compatibility<Parameter1Final, Parameter3Final>(true, T4_2_11_PARAMETER1FINAL_PARAMETER3FINAL);
}

TEST_F(TestExtensibility, T4_2_11_Parameter1Appendable_Parameter3Appendable)
{
    test_types_compatibility<Parameter1Appendable, Parameter3Appendable>(true, T4_2_11_PARAMETER1APPENDABLE_PARAMETER3APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_11_Parameter1Mutable_Parameter3Mutable)
{
    test_types_compatibility<Parameter1Mutable, Parameter3Mutable>(true, T4_2_11_PARAMETER1MUTABLE_PARAMETER3MUTABLE);
}
////@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


//=======================Test 4.2.12==============================

TEST_F(TestExtensibility, T4_2_12_Parameter1Final_Parameter2Final)
{
    test_types_compatibility<Parameter1Final, Parameter2Final>(false, T4_2_12_PARAMETER1FINAL_PARAMETER2FINAL);
}

//                These 2 tests currently do not work!
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

TEST_F(TestExtensibility, T4_2_12_Parameter1Appendable_Parameter2Appendable)
{
    test_types_compatibility<Parameter1Appendable, Parameter2Appendable>(true, T4_2_12_PARAMETER1APPENDABLE_PARAMETER2APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_12_Parameter1Mutable_Parameter2Mutable)
{
    test_types_compatibility<Parameter1Mutable, Parameter2Mutable>(true, T4_2_12_PARAMETER1MUTABLE_PARAMETER2MUTABLE);
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//=======================Test 4.2.13==============================

TEST_F(TestExtensibility, T4_2_13_Parameter1Final_Parameter4Final)
{
    test_types_compatibility<Parameter1Final, Parameter4Final>(false, T4_2_13_PARAMETER1FINAL_PARAMETER4FINAL);
}

TEST_F(TestExtensibility, T4_2_13_Parameter1Appendable_Parameter4Appendable)
{
    test_types_compatibility<Parameter1Appendable, Parameter4Appendable>(true, T4_2_13_PARAMETER1APPENDABLE_PARAMETER4APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_13_Parameter1Mutable_Parameter4Mutable)
{
    test_types_compatibility<Parameter1Mutable, Parameter4Mutable>(true, T4_2_13_PARAMETER1MUTABLE_PARAMETER4MUTABLE);
}


//=======================Test 4.2.14==============================

TEST_F(TestExtensibility, T4_2_14_Parameter4Final_Parameter5Final)
{
    test_types_compatibility<Parameter4Final, Parameter5Final>(false, T4_2_14_PARAMETER4FINAL_PARAMETER5FINAL);
}
//                These 2 tests currently do not work!
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
TEST_F(TestExtensibility, T4_2_14_Parameter4Appendable_Parameter5Appendable)
{
    test_types_compatibility<Parameter4Appendable, Parameter5Appendable>(true, T4_2_14_PARAMETER4APPENDABLE_PARAMETER5APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_14_Parameter4Mutable_Parameter5Mutable)
{
    test_types_compatibility<Parameter4Mutable, Parameter5Mutable>(true, T4_2_14_PARAMETER4MUTABLE_PARAMETER5MUTABLE);
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
TEST_F(TestExtensibility, T4_2_14_Parameter4Final_Parameter6Final)
{
    test_types_compatibility<Parameter4Final, Parameter6Final>(false, T4_2_14_PARAMETER4FINAL_PARAMETER6FINAL);
}

TEST_F(TestExtensibility, T4_2_14_Parameter4Appendable_Parameter6Appendable)
{
    test_types_compatibility<Parameter4Appendable, Parameter6Appendable>(false, T4_2_14_PARAMETER4APPENDABLE_PARAMETER6APPENDABLE);
}

TEST_F(TestExtensibility, T4_2_14_Parameter4Mutable_Parameter6Mutable)
{
    test_types_compatibility<Parameter4Mutable, Parameter6Mutable>(false, T4_2_14_PARAMETER4MUTABLE_PARAMETER6MUTABLE);
}

//=======================Test 4.2.15===============================

TEST_F(TestExtensibility, T4_2_15_Shape1Final_Shape11Final)
{
    test_types_compatibility<Shape1Final, Shape11Final>(false, T4_2_15_SHAPE1FINAL_SHAPE11FINAL);
}

TEST_F(TestExtensibility, T4_2_15_Shape1Appendable_Shape11Appendable)
{
    test_types_compatibility<Shape1Appendable, Shape11Appendable>(false, T4_2_15_SHAPE1APPENDABLE_SHAPE11APPENDABLE);
}
//                This test currently does not work!
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
TEST_F(TestExtensibility, T4_2_15_Shape1Mutable_Shape11Mutable)
{
    test_types_compatibility<Shape1Mutable, Shape11Mutable>(true, T4_2_15_SHAPE1MUTABLE_SHAPE11MUTABLE);
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
