//
//  TestSerialization.cxx
//  xtypes-interop
//
//  Created by Kelvin Zhang on 6/2/18.
//

#include "DDS_Variants.h"

#include <gtest/gtest.h>
#include "TestController.h"
#include "Utils.h"
#include "SerializationData.h"


class TestSerialization : public ::testing::Test
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
        launch_helper("./TestSerializationHelper");
        init_entities(participant, publisher, subscriber, controller, true);
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
            participant->unregister_type(type_name);
            topic = NULL;
        }
    }
    
    template <typename T1, typename T2>
    void test_serialization(
            T2 &data,
            DDS_SampleInfo &info,
            SerializationTest test_id,
            CDRPolicy policy,
            const std::vector<QosProperty> &set_to_one = {},
            const std::vector<QosProperty> &set_to_zero = {},
            bool expect_sample_drop = false);
    
    template <typename T1, typename T2>
    void T5_2_7_expect_sample_drop(
            SerializationTest test_id,
            CDRPolicy cdr_to_use = CDR_POLICY_XCDR);
    
    template <typename T1, typename T2, typename TestVersion>
    void run_test(SerializationTest test_id, CDRPolicy cdr_to_use = CDR_POLICY_XCDR);
};

template <typename T1, typename T2>
void TestSerialization::test_serialization(
        T2 &data,
        DDS_SampleInfo &info,
        SerializationTest test_id,
        CDRPolicy policy,
        const std::vector<QosProperty> &set_to_one,
        const std::vector<QosProperty> &set_to_zero,
        bool expect_sample_drop
        )
{
    ASSERT_NE(subscriber, nullptr);
    ASSERT_NE(publisher, nullptr);
    
    const char *type_name = topic_traits<T2>::TypeSupport::get_type_name();
    int ret = 0;
    ret = topic_traits<T2>::TypeSupport::register_type(participant, type_name);
    ASSERT_EQ(ret, DDS_RETCODE_OK);
    topic = participant->create_topic(
            SERIALIZATION_TOPIC,
            type_name,
            DDS_TOPIC_QOS_DEFAULT,
            NULL,
            DDS_STATUS_MASK_NONE);
    ASSERT_NE(topic, nullptr);
    DDSWaitSet waitset;
    
    DDS_DataReaderQos qos = get_test_datareader_default_qos();
    apply_qos_properties(qos, set_to_one, set_to_zero);
    
    reader = subscriber->create_datareader(
            topic,
            qos,
            NULL,
            DDS_STATUS_MASK_NONE);
    ASSERT_NE(reader, nullptr);
    DDSStatusCondition *status_condition = status_condition = reader->get_statuscondition();
    ASSERT_NE(status_condition, nullptr);
    if(expect_sample_drop)
        status_condition->set_enabled_statuses(DDS_SAMPLE_REJECTED_STATUS);
    else
        status_condition->set_enabled_statuses(DDS_DATA_AVAILABLE_STATUS);
    
    DDSStatusCondition *topic_status_condition = topic->get_statuscondition();
    ASSERT_NE(topic_status_condition, nullptr);
    topic_status_condition->set_enabled_statuses(DDS_INCONSISTENT_TOPIC_STATUS);
    
    ret = waitset.attach_condition(status_condition);
    ASSERT_EQ(ret, DDS_RETCODE_OK);
    ret = waitset.attach_condition(topic_status_condition);
    ASSERT_EQ(ret, DDS_RETCODE_OK);
    
    DDSConditionSeq active_condition;
    active_condition.maximum(2);
    
    ControlMessage msg;
    msg.action = TEST_ACTION_WRITE_SERIALIZATION;
    msg.param1._d = msg.action;
    msg.param1._u.testConfig.testNum = test_id;
    msg.param1._u.testConfig.policy = policy;
    if(policy != CDR_POLICY_XCDR && policy != CDR_POLICY_XCDR2)
    {
        throw std::runtime_error("invalid data representation qos: " +
                 std::to_string(policy));
    }
    controller->send_msg(msg);
    
    DDS_Duration_t timeout = {3, 0};
    ret = waitset.wait(active_condition, timeout);
    controller->send_ack();
    controller->recv_ack();
    typename topic_traits<T2>::DataReader *type_reader = topic_traits<T2>::DataReader::narrow(reader);
    ret = type_reader->take_next_sample(data, info);
    
    ASSERT_EQ(active_condition.length(), 1);
    ASSERT_NE(active_condition[0], topic_status_condition) << "inconsistent topic triggered";
    if(!expect_sample_drop)
        ASSERT_EQ(ret, DDS_RETCODE_OK) << "No data available";
}

DDSDomainParticipant    *TestSerialization::participant;
DDSPublisher            *TestSerialization::publisher;
DDSSubscriber           *TestSerialization::subscriber;
TestController          *TestSerialization::controller;

//==========================Test 5.2.1===================================

template <typename T1, typename T2>
void point_data_compare(T1 point1, T2 point2)
{
    ASSERT_EQ(point1.x, point2.x);
    ASSERT_EQ(point1.y, point2.y);
}


template <typename T>
void verify_data(const T &data, test_versions::T5_2_1)
{
    ASSERT_STREQ(data.color, "RED");
    ASSERT_STREQ(data.description, "RED SHAPE DESC");
    ASSERT_EQ(data.tags.length(), 1);
    ASSERT_STREQ(data.tags[0], "Tag_1");
    
    ASSERT_EQ(data.points[0].x, 0);
    ASSERT_EQ(data.points[0].y, 0);
    ASSERT_EQ(data.points[1].x, 10);
    ASSERT_EQ(data.points[1].y, 15);
    ASSERT_EQ(data.points[2].x, 20);
    ASSERT_EQ(data.points[2].y, 25);
    ASSERT_EQ(data.shapesize, 10);
}

TEST_F(TestSerialization, T5_2_1_Shape1Final__Shape1Final)
{
    run_test<Shape1Final, Shape1Final, test_versions::T5_2_1>(T5_2_1_SHAPE1FINAL_SHAPE1FINAL);
}

TEST_F(TestSerialization, T5_2_1_Shape1Appendable__Shape1Appendable)
{
    run_test<Shape1Appendable, Shape1Appendable, test_versions::T5_2_1>(T5_2_1_SHAPE1APPENDABLE_SHAPE1APPENDABLE);
}

TEST_F(TestSerialization, T5_2_1_Shape1Appendable__Shape1Default)
{
    run_test<Shape1Appendable, Shape1Default, test_versions::T5_2_1>(T5_2_1_SHAPE1APPENDABLE_SHAPE1DEFAULT);
}

TEST_F(TestSerialization, T5_2_1_Shape1Mutable__Shape1Mutable)
{
    run_test<Shape1Mutable, Shape1Mutable, test_versions::T5_2_1>(T5_2_1_SHAPE1MUTABLE_SHAPE1MUTABLE);
}

//==========================Test 5.2.3===================================

template <typename T, typename int_<decltype(T::angle)>::type = 0>
void T5_2_2_verify_angle(const T &data, struct special_)
{
    ASSERT_EQ(data.angle, 45.0);
}

template <typename T>
void T5_2_2_verify_angle(const T &data, struct general_)
{
}

template <typename T>
void verify_data(const T &data, test_versions::T5_2_2)
{
    verify_data(data, test_versions::T5_2_1());
    T5_2_2_verify_angle(data, special_());
}


TEST_F(TestSerialization, T5_2_2_Shape1Appendable__Shape2Appendable)
{
    run_test<Shape1Appendable, Shape2Appendable, test_versions::T5_2_2>(T5_2_2_SHAPE1APPENDABLE_SHAPE2APPENDABLE);
}

TEST_F(TestSerialization, T5_2_2_Shape1Mutable__Shape2Mutable)
{
    run_test<Shape1Mutable, Shape2Mutable, test_versions::T5_2_2>(T5_2_2_SHAPE1MUTABLE_SHAPE2MUTABLE);
}

TEST_F(TestSerialization, T5_2_2_Shape2Appendable__Shape1Appendable)
{
    run_test<Shape2Appendable, Shape1Appendable, test_versions::T5_2_2>(T5_2_2_SHAPE2APPENDABLE_SHAPE1APPENDABLE);
}

TEST_F(TestSerialization, T5_2_2_Shape2Mutable__Shape1Mutable)
{
    run_test<Shape2Mutable, Shape1Mutable, test_versions::T5_2_2>(T5_2_2_SHAPE2MUTABLE_SHAPE1MUTABLE);
}


//==========================Test 5.2.3===================================

template <typename T, typename int_<decltype(T::z)>::type = 0>
void T5_2_3_verify_z(const T &data, struct special_)
{
    ASSERT_EQ(data.z, 10);
}

template <typename T>
void T5_2_3_verify_z(const T &data, struct general_)
{
}

template <typename T>
void verify_data(const T &data, test_versions::T5_2_3)
{
    verify_data(data, test_versions::T5_2_1());
    T5_2_3_verify_z(data.points[0], special_());
    T5_2_3_verify_z(data.points[1], special_());
    T5_2_3_verify_z(data.points[2], special_());
}


TEST_F(TestSerialization, T5_2_3_Shape1Appendable__Shape3Appendable)
{
    run_test<Shape1Appendable, Shape3Appendable, test_versions::T5_2_3>(T5_2_3_SHAPE1APPENDABLE_SHAPE3APPENDABLE);
}

TEST_F(TestSerialization, T5_2_3_Shape1Mutable__Shape3Mutable)
{
    run_test<Shape1Mutable, Shape3Mutable, test_versions::T5_2_3>(T5_2_3_SHAPE1MUTABLE_SHAPE3MUTABLE);
}

TEST_F(TestSerialization, T5_2_3_Shape3Appendable__Shape1Appendable)
{
    run_test<Shape3Appendable, Shape1Appendable, test_versions::T5_2_3>(T5_2_3_SHAPE3APPENDABLE_SHAPE1APPENDABLE);
}

TEST_F(TestSerialization, T5_2_3_Shape3Mutable__Shape1Mutable)
{
    run_test<Shape3Mutable, Shape1Mutable, test_versions::T5_2_3>(T5_2_3_SHAPE3MUTABLE_SHAPE1MUTABLE);
}

//==========================Test 5.2.4===================================

template <typename T>
void verify_data(const T &data, test_versions::T5_2_4)
{
    verify_data(data, test_versions::T5_2_1());
    T5_2_2_verify_angle(data, special_());
}


TEST_F(TestSerialization, T5_2_4_Shape1MutableExplicitID__Shape4MutableExplicitID)
{
    run_test<Shape1MutableExplicitID, Shape4MutableExplicitID, test_versions::T5_2_4>(T5_2_4_SHAPE1MUTABLEEXPLICITID_SHAPE4MUTABLEEXPLICITID);
}

TEST_F(TestSerialization, T5_2_4_Shape4MutableExplicitID__Shape1MutableExplicitID)
{
    run_test<Shape4MutableExplicitID, Shape1MutableExplicitID, test_versions::T5_2_4>(T5_2_4_SHAPE4MUTABLEEXPLICITID_SHAPE1MUTABLEEXPLICITID);
}

//==========================Test 5.2.5===================================

template <typename T>
void verify_data(const T &data, test_versions::T5_2_5)
{
    verify_data(data, test_versions::T5_2_1());
    T5_2_2_verify_angle(data, special_());
}


TEST_F(TestSerialization, T5_2_5_Shape2Final__Shape5Final)
{
    run_test<Shape2Final, Shape5Final, test_versions::T5_2_5>(T5_2_5_SHAPE2FINAL_SHAPE5FINAL);
}

TEST_F(TestSerialization, T5_2_5_Shape2Appendable__Shape5Appendable)
{
    run_test<Shape2Appendable, Shape5Appendable, test_versions::T5_2_5>(T5_2_5_SHAPE2APPENDABLE_SHAPE5APPENDABLE);
}

TEST_F(TestSerialization, T5_2_5_Shape2Mutable__Shape5Mutable)
{
    run_test<Shape2Mutable, Shape5Mutable, test_versions::T5_2_5>(T5_2_5_SHAPE2MUTABLE_SHAPE5MUTABLE);
}

TEST_F(TestSerialization, T5_2_5_Shape5Final__Shape2Final)
{
    run_test<Shape5Final, Shape2Final, test_versions::T5_2_5>(T5_2_5_SHAPE5FINAL_SHAPE2FINAL);
}

TEST_F(TestSerialization, T5_2_5_Shape5Appendable__Shape2Appendable)
{
    run_test<Shape5Appendable, Shape2Appendable, test_versions::T5_2_5>(T5_2_5_SHAPE5APPENDABLE_SHAPE2APPENDABLE);
}

TEST_F(TestSerialization, T5_2_5_Shape5Mutable__Shape2Mutable)
{
    run_test<Shape5Mutable, Shape2Mutable, test_versions::T5_2_5>(T5_2_5_SHAPE5MUTABLE_SHAPE2MUTABLE);
}

//==========================Test 5.2.6===================================

template <typename T>
void verify_data(const T &data, test_versions::T5_2_6)
{
    ASSERT_STREQ(data.color, "RED");
    ASSERT_STREQ(data.description, "RED SHAPE");
    ASSERT_EQ(data.tags.length(), 1);
    ASSERT_STREQ(data.tags[0], "Tag_1");
    
    ASSERT_EQ(data.points[0].x, 0);
    ASSERT_EQ(data.points[0].y, 0);
    ASSERT_EQ(data.points[1].x, 10);
    ASSERT_EQ(data.points[1].y, 15);
    ASSERT_EQ(data.points[2].x, 20);
    ASSERT_EQ(data.points[2].y, 25);
    ASSERT_EQ(data.shapesize, 10);
}

TEST_F(TestSerialization, T5_2_6_Shape1Final__Shape6Final)
{
    run_test<Shape1Final, Shape6Final, test_versions::T5_2_6>(T5_2_6_SHAPE1FINAL_SHAPE6FINAL);
}

TEST_F(TestSerialization, T5_2_6_Shape1Appendable__Shape6Appendable)
{
    run_test<Shape1Appendable, Shape6Appendable, test_versions::T5_2_6>(T5_2_6_SHAPE1APPENDABLE_SHAPE6APPENDABLE);
}

TEST_F(TestSerialization, T5_2_6_Shape1Mutable__Shape6Mutable)
{
    run_test<Shape1Mutable, Shape6Mutable, test_versions::T5_2_6>(T5_2_6_SHAPE1MUTABLE_SHAPE6MUTABLE);
}

TEST_F(TestSerialization, T5_2_6_Shape6Final__Shape1Final)
{
    run_test<Shape6Final, Shape1Final, test_versions::T5_2_6>(T5_2_6_SHAPE6FINAL_SHAPE1FINAL);
}

TEST_F(TestSerialization, T5_2_6_Shape6Appendable__Shape1Appendable)
{
    run_test<Shape6Appendable, Shape1Appendable, test_versions::T5_2_6>(T5_2_6_SHAPE6APPENDABLE_SHAPE1APPENDABLE);
}

TEST_F(TestSerialization, T5_2_6_Shape6Mutable__Shape1Mutable)
{
    run_test<Shape6Mutable, Shape1Mutable, test_versions::T5_2_6>(T5_2_6_SHAPE6MUTABLE_SHAPE1MUTABLE);
}

//==========================Test 5.2.7===================================

template <typename T1, typename T2>
void TestSerialization::T5_2_7_expect_sample_drop(
                                                  SerializationTest test_id,
                                                  CDRPolicy cdr_to_use)
{
    T2 *data = create_data<T2>();
    DDS_SampleInfo info;
    test_serialization<T1, T2>(*data, info, test_id, cdr_to_use, {IGNORE_SEQUENCE_BOUNDS}, {}, true);
    ASSERT_EQ(info.valid_data, DDS_BOOLEAN_FALSE);
    delete_data<T2>(data);
}

template <typename T>
void verify_data(const T &data, test_versions::T5_2_7)
{
    ASSERT_STREQ(data.color, "RED");
    ASSERT_STREQ(data.description, "RED SHAPE DESC");
    ASSERT_EQ(data.tags.length(), 2);
    ASSERT_STREQ(data.tags[0], "Tag_1");
    ASSERT_STREQ(data.tags[1], "Tag_2");
    
    ASSERT_EQ(data.points[0].x, 0);
    ASSERT_EQ(data.points[0].y, 0);
    ASSERT_EQ(data.points[1].x, 10);
    ASSERT_EQ(data.points[1].y, 15);
    ASSERT_EQ(data.points[2].x, 20);
    ASSERT_EQ(data.points[2].y, 25);
    ASSERT_EQ(data.shapesize, 10);
}

TEST_F(TestSerialization, T5_2_7_Shape1Final__Shape6Final)
{
    T5_2_7_expect_sample_drop<Shape1Final, Shape6Final>(T5_2_7_SHAPE1FINAL_SHAPE6FINAL);
}

TEST_F(TestSerialization, T5_2_7_Shape1Appendable__Shape6Appendable)
{
    T5_2_7_expect_sample_drop<Shape1Appendable, Shape6Appendable>(T5_2_7_SHAPE1APPENDABLE_SHAPE6APPENDABLE);
}

TEST_F(TestSerialization, T5_2_7_Shape1Mutable__Shape6Mutable)
{
    T5_2_7_expect_sample_drop<Shape1Mutable, Shape6Mutable>(T5_2_7_SHAPE1MUTABLE_SHAPE6MUTABLE);
}

TEST_F(TestSerialization, T5_2_7_Shape6Final__Shape1Final)
{
    run_test<Shape6Final, Shape1Final, test_versions::T5_2_7>(T5_2_7_SHAPE6FINAL_SHAPE1FINAL);
}

TEST_F(TestSerialization, T5_2_7_Shape6Appendable__Shape1Appendable)
{
    run_test<Shape6Appendable, Shape1Appendable, test_versions::T5_2_7>(T5_2_7_SHAPE6APPENDABLE_SHAPE1APPENDABLE);
}

TEST_F(TestSerialization, T5_2_7_Shape6Mutable__Shape1Mutable)
{
    run_test<Shape6Mutable, Shape1Mutable, test_versions::T5_2_6>(T5_2_7_SHAPE6MUTABLE_SHAPE1MUTABLE);
}


//==========================Test 5.2.8===================================
template <typename T>
void verify_data(const T &data, test_versions::T5_2_8)
{
    ASSERT_EQ(data._d, IntKind::USHORT_PARAM);
    ASSERT_EQ(data._u.val_us, 34);
}


TEST_F(TestSerialization, T5_2_8_Parameter1Final__Parameter3Final)
{
    run_test<Parameter1Final, Parameter3Final, test_versions::T5_2_8>(T5_2_8_PARAMETER1FINAL_PARAMETER3FINAL);
}

TEST_F(TestSerialization, T5_2_8_Parameter1Appendable__Parameter3Appendable)
{
    run_test<Parameter1Appendable, Parameter3Appendable, test_versions::T5_2_8>(T5_2_8_PARAMETER1APPENDABLE_PARAMETER3APPENDABLE);
}

TEST_F(TestSerialization, T5_2_8_Parameter1Mutable__Parameter3Mutable)
{
    run_test<Parameter1Mutable, Parameter3Mutable, test_versions::T5_2_8>(T5_2_8_PARAMETER1MUTABLE_PARAMETER3MUTABLE);
}

TEST_F(TestSerialization, T5_2_8_Parameter3Final__Parameter1Final)
{
    run_test<Parameter3Final, Parameter1Final, test_versions::T5_2_8>(T5_2_8_PARAMETER3FINAL_PARAMETER1FINAL);
}

TEST_F(TestSerialization, T5_2_8_Parameter3Appendable__Parameter1Appendable)
{
    run_test<Parameter3Appendable, Parameter1Appendable, test_versions::T5_2_8>(T5_2_8_PARAMETER3APPENDABLE_PARAMETER1APPENDABLE);
}

TEST_F(TestSerialization, T5_2_8_Parameter3Mutable__Parameter1Mutable)
{
    run_test<Parameter3Mutable, Parameter1Mutable, test_versions::T5_2_8>(T5_2_8_PARAMETER3MUTABLE_PARAMETER1MUTABLE);
}


//==========================Test 5.2.9===================================

template <typename T, typename int_<decltype(T::_u::val_l)>::type = 0>
void T5_2_9_verify_union(const T &data, int val, struct special_)
{
    ASSERT_EQ(data._d, IntKind::LONG_PARAM);
    ASSERT_EQ(data._u.val_l, val);
}

template <typename T>
void T5_2_9_verify_union(const T &data, int val, struct general_)
{
    ASSERT_EQ(data._d, IntKind::USHORT_PARAM);
    ASSERT_EQ(data._u.val_us, val);
}

template <typename T>
void verify_data(const T &data, test_versions::T5_2_9)
{
    T5_2_9_verify_union(data, 34, special_());
}

TEST_F(TestSerialization, T5_2_9_Parameter1Appendable__Parameter2Appendable)
{
    run_test<Parameter1Appendable, Parameter2Appendable, test_versions::T5_2_9>(T5_2_9_PARAMETER1APPENDABLE_PARAMETER2APPENDABLE);
}

TEST_F(TestSerialization, T5_2_9_Parameter1Mutable__Parameter2Mutable)
{
    run_test<Parameter1Mutable, Parameter2Mutable, test_versions::T5_2_9>(T5_2_9_PARAMETER1MUTABLE_PARAMETER2MUTABLE);
}

TEST_F(TestSerialization, T5_2_9_Parameter2Appendable__Parameter1Appendable)
{
    run_test<Parameter2Appendable, Parameter1Appendable, test_versions::T5_2_9>(T5_2_9_PARAMETER1APPENDABLE_PARAMETER2APPENDABLE);
}

TEST_F(TestSerialization, T5_2_9_Parameter2Mutable__Parameter1Mutable)
{
    run_test<Parameter2Mutable, Parameter1Mutable, test_versions::T5_2_9>(T5_2_9_PARAMETER1MUTABLE_PARAMETER2MUTABLE);
}

//==========================Test 5.2.10===================================

template <typename T>
void verify_data(const T &data, test_versions::T5_2_10)
{
    ASSERT_EQ(data._d, IntKind::SHORT_PARAM);
    ASSERT_EQ(data._u.val_s, 0);
}

template <>
void verify_data<Parameter7Appendable>(const Parameter7Appendable &data, test_versions::T5_2_10)
{
    ASSERT_EQ(data._d, LongIntKind::USHORT_PARAM);
    ASSERT_EQ(data._u.val_us, 34);
}

template <>
void verify_data<Parameter7Mutable>(const Parameter7Mutable &data, test_versions::T5_2_10)
{
    ASSERT_EQ(data._d, LongIntKind::USHORT_PARAM);
    ASSERT_EQ(data._u.val_us, 34);
}

TEST_F(TestSerialization, T5_2_10_Parameter1Appendable__Parameter7Appendable)
{
    run_test<Parameter1Appendable, Parameter7Appendable, test_versions::T5_2_10>(T5_2_10_PARAMETER1APPENDABLE_PARAMETER7APPENDABLE);
}

TEST_F(TestSerialization, T5_2_10_Parameter1Mutable__Parameter7Mutable)
{
    run_test<Parameter1Mutable, Parameter7Mutable, test_versions::T5_2_10>(T5_2_10_PARAMETER1MUTABLE_PARAMETER7MUTABLE);
}

TEST_F(TestSerialization, T5_2_10_Parameter7Appendable__Parameter1Appendable)
{
    run_test<Parameter7Appendable, Parameter1Appendable, test_versions::T5_2_10>(T5_2_10_PARAMETER7APPENDABLE_PARAMETER1APPENDABLE);
}

TEST_F(TestSerialization, T5_2_10_Parameter7Mutable__Parameter1Mutable)
{
    run_test<Parameter7Mutable, Parameter1Mutable, test_versions::T5_2_10>(T5_2_10_PARAMETER7MUTABLE_PARAMETER1MUTABLE);
}

//==========================Test 5.2.11===================================

template <typename T, typename int_<decltype(T::_u::val_l)>::type = 0>
void T5_2_11_verify_union(T &data, int val, struct special_)
{
    ASSERT_EQ(data._d, IntKind::LONG_PARAM);
    ASSERT_EQ(data._u.val_l, val);
}

template <typename T>
void T5_2_11_verify_union(T &data, int val, struct general_)
{
    ASSERT_EQ(data._d, IntKind::LONG_PARAM);
}

template <typename T>
void verify_data(const T &data, test_versions::T5_2_11)
{
    T5_2_9_verify_union(data, 34, special_());
}

TEST_F(TestSerialization, T5_2_11_Parameter1Appendable__Parameter7Appendable)
{
    run_test<Parameter1Appendable, Parameter4Appendable, test_versions::T5_2_11>(T5_2_11_PARAMETER1APPENDABLE_PARAMETER4APPENDABLE);
}

TEST_F(TestSerialization, T5_2_11_Parameter1Mutable__Parameter4Mutable)
{
    run_test<Parameter1Mutable, Parameter4Mutable, test_versions::T5_2_11>(T5_2_11_PARAMETER1MUTABLE_PARAMETER4MUTABLE);
}

TEST_F(TestSerialization, T5_2_11_Parameter4Appendable__Parameter1Appendable)
{
    run_test<Parameter4Appendable, Parameter1Appendable, test_versions::T5_2_11>(T5_2_11_PARAMETER4APPENDABLE_PARAMETER1APPENDABLE);
}

TEST_F(TestSerialization, T5_2_11_Parameter4Mutable__Parameter1Mutable)
{
    run_test<Parameter4Mutable, Parameter1Mutable, test_versions::T5_2_11>(T5_2_11_PARAMETER4MUTABLE_PARAMETER1MUTABLE);
}

//==========================Test 5.2.12===================================

template <typename T>
void verify_data(const T &data, test_versions::T5_2_12)
{
    ASSERT_STREQ(data.color, "RED");
    ASSERT_EQ(data.description, nullptr);
    ASSERT_EQ(data.tags.length(), 1);
    ASSERT_STREQ(data.tags[0], "Tag_1");
    
    ASSERT_EQ(data.points[0].x, 0);
    ASSERT_EQ(data.points[0].y, 0);
    ASSERT_EQ(data.points[1].x, 10);
    ASSERT_EQ(data.points[1].y, 15);
    ASSERT_EQ(data.points[2].x, 20);
    ASSERT_EQ(data.points[2].y, 25);
    ASSERT_EQ(data.shapesize, 10);
}

TEST_F(TestSerialization, T5_2_12_Shape1Final__Shape1Final)
{
    run_test<Shape1Final, Shape1Final, test_versions::T5_2_12>(T5_2_12_SHAPE1FINAL_SHAPE1FINAL);
}

TEST_F(TestSerialization, T5_2_12_Shape1Appendable__Shape1Appendable)
{
    run_test<Shape1Appendable, Shape1Appendable, test_versions::T5_2_12>(T5_2_12_SHAPE1APPENDABLE_SHAPE1APPENDABLE);
}

TEST_F(TestSerialization, T5_2_12_Shape1Mutable__Shape1Mutable)
{
    run_test<Shape1Mutable, Shape1Mutable, test_versions::T5_2_12>(T5_2_12_SHAPE1MUTABLE_SHAPE1MUTABLE);
}

//==========================Test 5.2.13===================================

template <typename T>
void verify_data(const T &data, test_versions::T5_2_13_A)
{
    verify_data(data, test_versions::T5_2_12());
}

template <typename T>
void verify_data(const T &data, test_versions::T5_2_13_B)
{
    ASSERT_STREQ(data.color, "RED");
    ASSERT_STREQ(data.description, "RED SHAPE DESC");
    ASSERT_EQ(data.tags.length(), 1);
    ASSERT_STREQ(data.tags[0], "Tag_1");
    
    ASSERT_EQ(data.points[0].x, 0);
    ASSERT_EQ(data.points[0].y, 0);
    ASSERT_EQ(data.points[1].x, 10);
    ASSERT_EQ(data.points[1].y, 15);
    ASSERT_EQ(data.points[2].x, 20);
    ASSERT_EQ(data.points[2].y, 25);
    ASSERT_EQ(data.shapesize, 10);
}

template <typename T>
void verify_data(const T &data, test_versions::T5_2_13_C)
{
    verify_data(data, test_versions::T5_2_13_B());
}

TEST_F(TestSerialization, T5_2_13_Shape1Mutable__Shape11Mutable_With_Null)
{
    run_test<Shape1Mutable, Shape11Mutable, test_versions::T5_2_13_A>(T5_2_13_A_SHAPE1MUTABLE_SHAPE11MUTABLE);
}

TEST_F(TestSerialization, T5_2_13_Shape1Mutable__Shape11Mutable)
{
    run_test<Shape1Mutable, Shape11Mutable, test_versions::T5_2_13_B>(T5_2_13_B_SHAPE1MUTABLE_SHAPE11MUTABLE);
}

TEST_F(TestSerialization, T5_2_13_Shape11Mutable__Shape1Mutable)
{
    run_test<Shape11Mutable, Shape1Mutable, test_versions::T5_2_13_C>(T5_2_13_C_SHAPE11MUTABLE_SHAPE1MUTABLE);
}

template <typename T1, typename T2, typename TestVersion>
void TestSerialization::run_test(SerializationTest test_id, CDRPolicy cdr_to_use)
{
    T2 *data = create_data<T2>();
    DDS_SampleInfo info;
    TestVersion version;
    test_serialization<T1, T2>(*data, info, test_id, cdr_to_use, version.set_to_one, version.set_to_zero);
    ASSERT_EQ(info.valid_data, DDS_BOOLEAN_TRUE);
    verify_data(*data, version);
}
