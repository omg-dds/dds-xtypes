//
//  TestExtensibilityHelper.cpp
//  TestExtensibilityHelper
//
//  Created by Kelvin Zhang on 5/31/18.
//

#include "DDS_Variants.h"
#include "TestController.h"
#include "../Utils.h"
#include <iostream>

template <typename T>
void write_default_data(
        DDSDomainParticipant *participant,
        DDSPublisher *publisher,
        TestController *controller,
        std::initializer_list<QosProperty> set_to_one = {},
        std::initializer_list<QosProperty> set_to_zero = {})
{
    T *data = create_data<T>();
    //We use writer's type name as topic name
    const char *topic_name = topic_traits<T>::TypeSupport::get_type_name();
    write_and_wait(
        participant,
        publisher,
        controller,
        topic_name,
        data,
        CDR_POLICY_XCDR,
        set_to_one,
        set_to_zero);
    delete_data<T>(data);
}

int main(int argc, const char *argv[])
{
    DDSDomainParticipant *participant = NULL;
    DDSPublisher *publisher;
    DDSSubscriber *subscriber;
    TestController *controller;
    init_entities(participant, publisher, subscriber, controller, false);
    
    while(true)
    {
        ControlMessage msg = controller->recv_msg();
        if (msg.action == TEST_ACTION_EXIT) {
            return 0;
        }
        if (msg.action != TEST_ACTION_WRITE_EXTENSIBILITY_KIND) {
            throw std::runtime_error("Unexpected test action: " + std::to_string(msg.action));
        }
        switch (msg.param1._u.kind) {
            case T4_2_1_SHAPE1FINAL_SHAPE1FINAL:
            case T4_2_1_SHAPE1FINAL_SHAPE1DEFAULT:
            case T4_2_1_SHAPE1FINAL_SHAPE1APPENDABLE:
            case T4_2_1_SHAPE1FINAL_SHAPE1MUTABLE:
            case T4_2_4_SHAPE1FINAL_SHAPE4FINAL:
            case T4_2_2_SHAPE1FINAL_SHAPE2FINAL:
            case T4_2_8_SHAPE1FINAL_SHAPE8FINAL:
            case T4_2_9_SHAPE1FINAL_SHAPE9FINAL:
            case T4_2_10_SHAPE1FINAL_SHAPE10FINAL:
            case T4_2_15_SHAPE1FINAL_SHAPE11FINAL:
                write_default_data<Shape1Final>(participant, publisher, controller);
                break;
            case T4_2_1_SHAPE1APPENDABLE_SHAPE1DEFAULT:
            case T4_2_1_SHAPE1APPENDABLE_SHAPE1APPENDABLE:
            case T4_2_1_SHAPE1APPENDABLE_SHAPE1MUTABLE:
            case T4_2_4_SHAPE1APPENDABLE_SHAPE4APPENDABLE:
            case T4_2_8_SHAPE1APPENDABLE_SHAPE8APPENDABLE:
            case T4_2_9_SHAPE1APPENDABLE_SHAPE9APPENDABLE:
            case T4_2_10_SHAPE1APPENDABLE_SHAPE10APPENDABLE:
            case T4_2_15_SHAPE1APPENDABLE_SHAPE11APPENDABLE:
            case T4_2_2_SHAPE1APPENDABLE_SHAPE2APPENDABLE:
                write_default_data<Shape1Appendable>(participant, publisher, controller);
                break;
            case T4_2_1_SHAPE1MUTABLE_SHAPE1DEFAULT:
            case T4_2_1_SHAPE1MUTABLE_SHAPE1MUTABLE:
            case T4_2_4_SHAPE1MUTABLE_SHAPE4MUTABLE:
            case T4_2_8_SHAPE1MUTABLE_SHAPE8MUTABLE:
            case T4_2_9_SHAPE1MUTABLE_SHAPE9MUTABLE:
            case T4_2_10_SHAPE1MUTABLE_SHAPE10MUTABLE:
            case T4_2_15_SHAPE1MUTABLE_SHAPE11MUTABLE:
            case T4_2_2_SHAPE1MUTABLE_SHAPE2MUTABLE:
                //TODO: support other test cases...
                write_default_data<Shape1Mutable>(participant, publisher, controller);
                break;
            
            case T4_2_2_SHAPE1FINAL_SHAPE2FINAL_PREVENT_WIDENING:
                write_default_data<Shape1Final>(participant, publisher, controller,
                    {PREVENT_TYPE_WIDENING});
                break;
            case T4_2_2_SHAPE1APPENDABLE_SHAPE2APPENDABLE_PREVENT_WIDENING:
                write_default_data<Shape1Appendable>(participant, publisher, controller,
                    {PREVENT_TYPE_WIDENING});
                break;
            case T4_2_2_SHAPE1MUTABLE_SHAPE2MUTABLE_PREVENT_WIDENING:
                write_default_data<Shape1Mutable>(participant, publisher, controller,
                    {PREVENT_TYPE_WIDENING});
                break;
            case T4_2_2_SHAPE2FINAL_SHAPE1FINAL_PREVENT_WIDENING:
                write_default_data<Shape2Final>(participant, publisher, controller,
                    {PREVENT_TYPE_WIDENING});
                break;
            case T4_2_2_SHAPE2APPENDABLE_SHAPE1APPENDABLE_PREVENT_WIDENING:
                write_default_data<Shape2Appendable>(participant, publisher, controller,
                     {PREVENT_TYPE_WIDENING});
                break;
            case T4_2_2_SHAPE2MUTABLE_SHAPE1MUTABLE_PREVENT_WIDENING:
                write_default_data<Shape2Mutable>(participant, publisher, controller,
                    {PREVENT_TYPE_WIDENING});
                break;
                
            case T4_2_4_SHAPE1MUTABLEEXPLICITID_SHAPE4MUTABLEEXPLICITID:
                write_default_data<Shape1MutableExplicitID>(participant, publisher, controller);
                break;
            case T4_2_5_SHAPE2FINAL_SHAPE5FINAL:
                write_default_data<Shape2Final>(participant, publisher, controller);
                break;
            case T4_2_5_SHAPE2APPENDABLE_SHAPE5APPENDABLE:
                write_default_data<Shape2Appendable>(participant, publisher, controller);
                break;
            case T4_2_5_SHAPE2MUTABLE_SHAPE5MUTABLE:
                write_default_data<Shape2Mutable>(participant, publisher, controller);
                break;
                
            case T4_2_6_SHAPE1FINAL_SHAPE6FINAL:
                write_default_data<Shape1Final>(participant, publisher, controller,
                    {},
                    {IGNORE_STRING_BOUNDS});
                break;
            case T4_2_6_SHAPE1APPENDABLE_SHAPE6APPENDABLE:
                write_default_data<Shape1Appendable>(participant, publisher, controller,
                    {},
                    {IGNORE_STRING_BOUNDS});
                break;
            case T4_2_6_SHAPE1MUTABLE_SHAPE6MUTABLE:
                write_default_data<Shape1Mutable>(participant, publisher, controller,
                    {},
                    {IGNORE_STRING_BOUNDS});
                break;
            case T4_2_6_SHAPE6FINAL_SHAPE1FINAL:
                write_default_data<Shape6Final>(participant, publisher, controller,
                    {},
                    {IGNORE_STRING_BOUNDS});
                break;
            case T4_2_6_SHAPE6APPENDABLE_SHAPE1APPENDABLE:
                write_default_data<Shape6Appendable>(participant, publisher, controller,
                    {},
                    {IGNORE_STRING_BOUNDS});
                break;
            case T4_2_6_SHAPE6MUTABLE_SHAPE1MUTABLE:
                write_default_data<Shape6Mutable>(participant, publisher, controller,
                    {},
                    {IGNORE_STRING_BOUNDS});
                break;
                
            case T4_2_6_SHAPE1FINAL_SHAPE6FINAL_IGNORE_STRING_BOUNDS:
                write_default_data<Shape1Final>(participant, publisher, controller,
                    {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_6_SHAPE1APPENDABLE_SHAPE6APPENDABLE_IGNORE_STRING_BOUNDS:
                write_default_data<Shape1Appendable>(participant, publisher, controller,
                    {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_6_SHAPE1MUTABLE_SHAPE6MUTABLE_IGNORE_STRING_BOUNDS:
                write_default_data<Shape1Mutable>(participant, publisher, controller,
                    {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_6_SHAPE6FINAL_SHAPE1FINAL_IGNORE_STRING_BOUNDS:
                write_default_data<Shape6Final>(participant, publisher, controller,
                    {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_6_SHAPE6APPENDABLE_SHAPE1APPENDABLE_IGNORE_STRING_BOUNDS:
                write_default_data<Shape6Appendable>(participant, publisher, controller,
                    {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_6_SHAPE6MUTABLE_SHAPE1MUTABLE_IGNORE_STRING_BOUNDS:
                write_default_data<Shape6Mutable>(participant, publisher, controller,
                    {IGNORE_SEQUENCE_BOUNDS});
                break;
                
            
            case T4_2_7_SHAPE1FINAL_SHAPE7FINAL:
                write_default_data<Shape1Final>(participant, publisher, controller,
                    {},
                    {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_7_SHAPE1APPENDABLE_SHAPE7APPENDABLE:
                write_default_data<Shape1Appendable>(participant, publisher, controller,
                     {},
                     {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_7_SHAPE1MUTABLE_SHAPE7MUTABLE:
                write_default_data<Shape1Mutable>(participant, publisher, controller,
                      {},
                      {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_7_SHAPE7FINAL_SHAPE1FINAL:
                write_default_data<Shape7Final>(participant, publisher, controller,
                    {},
                    {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_7_SHAPE7APPENDABLE_SHAPE1APPENDABLE:
                write_default_data<Shape7Appendable>(participant, publisher, controller,
                     {},
                     {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_7_SHAPE7MUTABLE_SHAPE1MUTABLE:
                write_default_data<Shape7Mutable>(participant, publisher, controller,
                      {},
                      {IGNORE_SEQUENCE_BOUNDS});
                break;
                
            case T4_2_7_SHAPE1FINAL_SHAPE7FINAL_IGNORE_SEQUENCE_BOUNDS:
                write_default_data<Shape1Final>(participant, publisher, controller,                                                    {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_7_SHAPE1APPENDABLE_SHAPE7APPENDABLE_IGNORE_SEQUENCE_BOUNDS:
                write_default_data<Shape1Appendable>(participant, publisher, controller,
                     {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_7_SHAPE1MUTABLE_SHAPE7MUTABLE_IGNORE_SEQUENCE_BOUNDS:
                write_default_data<Shape1Mutable>(participant, publisher, controller,
                      {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_7_SHAPE7FINAL_SHAPE1FINAL_IGNORE_SEQUENCE_BOUNDS:
                write_default_data<Shape7Final>(participant, publisher, controller,
                      {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_7_SHAPE7APPENDABLE_SHAPE1APPENDABLE_IGNORE_SEQUENCE_BOUNDS:
                write_default_data<Shape7Appendable>(participant, publisher, controller,
                      {IGNORE_SEQUENCE_BOUNDS});
                break;
            case T4_2_7_SHAPE7MUTABLE_SHAPE1MUTABLE_IGNORE_SEQUENCE_BOUNDS:
                write_default_data<Shape7Mutable>(participant, publisher, controller,
                      {IGNORE_SEQUENCE_BOUNDS});
                break;
                
            
            case T4_2_8_SHAPE1FINAL_SHAPE8FINAL_IGNORE_MEMBER_NAME:
                write_default_data<Shape1Final>(participant, publisher, controller,
                    {IGNORE_MEMBER_NAMES,
                    IGNORE_MEMBER_NAMES});
                break;
            case T4_2_8_SHAPE1APPENDABLE_SHAPE8APPENDABLE_IGNORE_MEMBER_NAME:
                write_default_data<Shape1Appendable>(participant, publisher, controller,
                    {IGNORE_MEMBER_NAMES,
                        IGNORE_MEMBER_NAMES});
                break;
            case T4_2_8_SHAPE1MUTABLE_SHAPE8MUTABLE_IGNORE_MEMBER_NAME:
                write_default_data<Shape1Mutable>(participant, publisher, controller,
                     {IGNORE_MEMBER_NAMES,
                     IGNORE_MEMBER_NAMES});
                break;
                
                
            case T4_2_10_SHAPE10FINAL_SHAPE1FINAL:
                write_default_data<Shape10Final>(participant, publisher, controller);
                break;
            case T4_2_10_SHAPE10APPENDABLE_SHAPE1APPENDABLE:
                write_default_data<Shape10Appendable>(participant, publisher, controller);
                break;
            case T4_2_10_SHAPE10MUTABLE_SHAPE1MUTABLE:
                write_default_data<Shape10Mutable>(participant, publisher, controller);
                break;
            case T4_2_11_PARAMETER1FINAL_PARAMETER3FINAL:
            case T4_2_12_PARAMETER1FINAL_PARAMETER2FINAL:
            case T4_2_13_PARAMETER1FINAL_PARAMETER4FINAL:
                write_default_data<Parameter1Final>(participant, publisher, controller);
                break;
            case T4_2_11_PARAMETER1APPENDABLE_PARAMETER3APPENDABLE:
            case T4_2_12_PARAMETER1APPENDABLE_PARAMETER2APPENDABLE:
            case T4_2_13_PARAMETER1APPENDABLE_PARAMETER4APPENDABLE:
                write_default_data<Parameter1Appendable>(participant, publisher, controller);
                break;
            case T4_2_11_PARAMETER1MUTABLE_PARAMETER3MUTABLE:
            case T4_2_12_PARAMETER1MUTABLE_PARAMETER2MUTABLE:
            case T4_2_13_PARAMETER1MUTABLE_PARAMETER4MUTABLE:
                write_default_data<Parameter1Mutable>(participant, publisher, controller);
                break;
            case T4_2_14_PARAMETER4FINAL_PARAMETER5FINAL:
            case T4_2_14_PARAMETER4FINAL_PARAMETER6FINAL:
                write_default_data<Parameter4Final>(participant, publisher, controller);
                break;
            case T4_2_14_PARAMETER4APPENDABLE_PARAMETER5APPENDABLE:
            case T4_2_14_PARAMETER4APPENDABLE_PARAMETER6APPENDABLE:
                write_default_data<Parameter4Appendable>(participant, publisher, controller);
                break;
            case T4_2_14_PARAMETER4MUTABLE_PARAMETER5MUTABLE:
            case T4_2_14_PARAMETER4MUTABLE_PARAMETER6MUTABLE:
                write_default_data<Parameter4Mutable>(participant, publisher, controller);
                break;
            default:
                throw std::runtime_error("Unexpected test id: " + std::to_string(msg.param1._u.kind));
        }
    }
    return 0;
}
