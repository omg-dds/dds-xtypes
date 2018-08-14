//
//  TestSerializationHelper.cxx
//  xtypes-interop
//
//  Created by Kelvin Zhang on 6/4/18.
//

#include "DDS_Variants.h"
#include "TestController.h"
#include <iostream>
#include "../Utils.h"
#include "SerializationData.h"

DDSDomainParticipant *participant;
DDSPublisher *publisher;
DDSSubscriber *subscriber;
TestController *controller;


template <typename T, typename TestVersion>
void write_data(CDRPolicy policy)
{
    T *data = create_data<T>();
    TestVersion version;
    fill_data(*data, version);
    write_and_wait(
                   participant,
                   publisher,
                   controller,
                   SERIALIZATION_TOPIC,
                   data,
                   policy,
                   version.set_to_one,
                   version.set_to_zero);
    delete_data<T>(data);
}

int main(int argc, const char *argv[])
{
    init_entities(participant, publisher, subscriber, controller, false);
    
    while(true)
    {
        ControlMessage msg = controller->recv_msg();
        if (msg.action == TEST_ACTION_EXIT) {
            return 0;
        }
        if (msg.action == TEST_ACTION_WRITE_SERIALIZATION) {
            switch (msg.param1._u.testConfig.testNum) {
                case T5_2_1_SHAPE1FINAL_SHAPE1FINAL:
                    write_data<Shape1Final, test_versions::T5_2_1>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_1_SHAPE1APPENDABLE_SHAPE1APPENDABLE:
                case T5_2_1_SHAPE1APPENDABLE_SHAPE1DEFAULT:
                    write_data<Shape1Appendable, test_versions::T5_2_1>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_1_SHAPE1MUTABLE_SHAPE1MUTABLE:
                    write_data<Shape1Mutable, test_versions::T5_2_1>(msg.param1._u.testConfig.policy);
                    break;
                    
                    
                case T5_2_2_SHAPE1APPENDABLE_SHAPE2APPENDABLE:
                    write_data<Shape1Appendable, test_versions::T5_2_2>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_2_SHAPE1MUTABLE_SHAPE2MUTABLE:
                    write_data<Shape1Mutable, test_versions::T5_2_2>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_2_SHAPE2APPENDABLE_SHAPE1APPENDABLE:
                    write_data<Shape2Appendable, test_versions::T5_2_2>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_2_SHAPE2MUTABLE_SHAPE1MUTABLE:
                    write_data<Shape2Mutable, test_versions::T5_2_2>(msg.param1._u.testConfig.policy);
                    break;
                    
                    
                case T5_2_3_SHAPE1APPENDABLE_SHAPE3APPENDABLE:
                    write_data<Shape1Appendable, test_versions::T5_2_3>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_3_SHAPE1MUTABLE_SHAPE3MUTABLE:
                    write_data<Shape1Mutable, test_versions::T5_2_3>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_3_SHAPE3APPENDABLE_SHAPE1APPENDABLE:
                    write_data<Shape3Appendable, test_versions::T5_2_3>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_3_SHAPE3MUTABLE_SHAPE1MUTABLE:
                    write_data<Shape3Mutable, test_versions::T5_2_3>(msg.param1._u.testConfig.policy);
                    break;
                    
                    
                case T5_2_4_SHAPE1MUTABLEEXPLICITID_SHAPE4MUTABLEEXPLICITID:
                    write_data<Shape1MutableExplicitID, test_versions::T5_2_4>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_4_SHAPE4MUTABLEEXPLICITID_SHAPE1MUTABLEEXPLICITID:
                    write_data<Shape4MutableExplicitID, test_versions::T5_2_4>(msg.param1._u.testConfig.policy);
                    break;
                    
                    
                case T5_2_5_SHAPE2FINAL_SHAPE5FINAL:
                    write_data<Shape2Final, test_versions::T5_2_5>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_5_SHAPE2APPENDABLE_SHAPE5APPENDABLE:
                    write_data<Shape2Appendable, test_versions::T5_2_5>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_5_SHAPE2MUTABLE_SHAPE5MUTABLE:
                    write_data<Shape2Mutable, test_versions::T5_2_5>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_5_SHAPE5FINAL_SHAPE2FINAL:
                    write_data<Shape5Final, test_versions::T5_2_5>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_5_SHAPE5APPENDABLE_SHAPE2APPENDABLE:
                    write_data<Shape5Appendable, test_versions::T5_2_5>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_5_SHAPE5MUTABLE_SHAPE2MUTABLE:
                    write_data<Shape5Mutable, test_versions::T5_2_5>(msg.param1._u.testConfig.policy);
                    break;
                    
                case T5_2_6_SHAPE1FINAL_SHAPE6FINAL:
                    write_data<Shape1Final, test_versions::T5_2_6>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_6_SHAPE1APPENDABLE_SHAPE6APPENDABLE:
                    write_data<Shape1Appendable, test_versions::T5_2_6>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_6_SHAPE1MUTABLE_SHAPE6MUTABLE:
                    write_data<Shape1Mutable, test_versions::T5_2_6>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_6_SHAPE6FINAL_SHAPE1FINAL:
                    write_data<Shape6Final, test_versions::T5_2_6>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_6_SHAPE6APPENDABLE_SHAPE1APPENDABLE:
                    write_data<Shape6Appendable, test_versions::T5_2_6>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_6_SHAPE6MUTABLE_SHAPE1MUTABLE:
                    write_data<Shape6Mutable, test_versions::T5_2_6>(msg.param1._u.testConfig.policy);
                    break;
                    
                case T5_2_7_SHAPE1FINAL_SHAPE6FINAL:
                    write_data<Shape1Final, test_versions::T5_2_7>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_7_SHAPE1APPENDABLE_SHAPE6APPENDABLE:
                    write_data<Shape1Appendable, test_versions::T5_2_7>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_7_SHAPE1MUTABLE_SHAPE6MUTABLE:
                    write_data<Shape1Mutable, test_versions::T5_2_7>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_7_SHAPE6FINAL_SHAPE1FINAL:
                    write_data<Shape6Final, test_versions::T5_2_7>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_7_SHAPE6APPENDABLE_SHAPE1APPENDABLE:
                    write_data<Shape6Appendable, test_versions::T5_2_7>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_7_SHAPE6MUTABLE_SHAPE1MUTABLE:
                    write_data<Shape6Mutable, test_versions::T5_2_7>(msg.param1._u.testConfig.policy);
                    break;
                    
                case T5_2_8_PARAMETER1FINAL_PARAMETER3FINAL:
                    write_data<Parameter1Final, test_versions::T5_2_8>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_8_PARAMETER1APPENDABLE_PARAMETER3APPENDABLE:
                    write_data<Parameter1Appendable, test_versions::T5_2_8>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_8_PARAMETER1MUTABLE_PARAMETER3MUTABLE:
                    write_data<Parameter1Mutable, test_versions::T5_2_8>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_8_PARAMETER3FINAL_PARAMETER1FINAL:
                    write_data<Parameter3Final, test_versions::T5_2_8>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_8_PARAMETER3APPENDABLE_PARAMETER1APPENDABLE:
                    write_data<Parameter3Appendable, test_versions::T5_2_8>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_8_PARAMETER3MUTABLE_PARAMETER1MUTABLE:
                    write_data<Parameter3Mutable, test_versions::T5_2_8>(msg.param1._u.testConfig.policy);
                    break;
                    
                    
                case T5_2_9_PARAMETER1APPENDABLE_PARAMETER2APPENDABLE:
                    write_data<Parameter1Appendable, test_versions::T5_2_9>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_9_PARAMETER1MUTABLE_PARAMETER2MUTABLE:
                    write_data<Parameter1Mutable, test_versions::T5_2_9>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_9_PARAMETER2APPENDABLE_PARAMETER1APPENDABLE:
                    write_data<Parameter2Appendable, test_versions::T5_2_9>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_9_PARAMETER2MUTABLE_PARAMETER1MUTABLE:
                    write_data<Parameter2Mutable, test_versions::T5_2_9>(msg.param1._u.testConfig.policy);
                    break;
                    
                case T5_2_10_PARAMETER1APPENDABLE_PARAMETER7APPENDABLE:
                    write_data<Parameter1Appendable, test_versions::T5_2_10>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_10_PARAMETER1MUTABLE_PARAMETER7MUTABLE:
                    write_data<Parameter1Mutable, test_versions::T5_2_10>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_10_PARAMETER7APPENDABLE_PARAMETER1APPENDABLE:
                    write_data<Parameter7Appendable, test_versions::T5_2_10>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_10_PARAMETER7MUTABLE_PARAMETER1MUTABLE:
                    write_data<Parameter7Mutable, test_versions::T5_2_10>(msg.param1._u.testConfig.policy);
                    break;
                    
                case T5_2_11_PARAMETER1APPENDABLE_PARAMETER4APPENDABLE:
                    write_data<Parameter1Appendable, test_versions::T5_2_11>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_11_PARAMETER1MUTABLE_PARAMETER4MUTABLE:
                    write_data<Parameter1Mutable, test_versions::T5_2_11>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_11_PARAMETER4APPENDABLE_PARAMETER1APPENDABLE:
                    write_data<Parameter4Appendable, test_versions::T5_2_11>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_11_PARAMETER4MUTABLE_PARAMETER1MUTABLE:
                    write_data<Parameter4Mutable, test_versions::T5_2_11>(msg.param1._u.testConfig.policy);
                    break;
                    
                    
                case T5_2_12_SHAPE1FINAL_SHAPE1FINAL:
                    write_data<Shape1Final, test_versions::T5_2_12>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_12_SHAPE1APPENDABLE_SHAPE1APPENDABLE:
                    write_data<Shape1Appendable, test_versions::T5_2_12>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_12_SHAPE1MUTABLE_SHAPE1MUTABLE:
                    write_data<Shape1Mutable, test_versions::T5_2_12>(msg.param1._u.testConfig.policy);
                    break;
                    
                case T5_2_13_A_SHAPE1MUTABLE_SHAPE11MUTABLE:
                    write_data<Shape1Mutable, test_versions::T5_2_13_A>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_13_B_SHAPE1MUTABLE_SHAPE11MUTABLE:
                    write_data<Shape1Mutable, test_versions::T5_2_13_B>(msg.param1._u.testConfig.policy);
                    break;
                case T5_2_13_C_SHAPE11MUTABLE_SHAPE1MUTABLE:
                    write_data<Shape11Mutable, test_versions::T5_2_13_C>(msg.param1._u.testConfig.policy);
                    break;
                default:
                    throw std::runtime_error("Unexpected test id: " + std::to_string(msg.param1._u.kind));
                    break;
            }
        }
    }
    return 0;
}
