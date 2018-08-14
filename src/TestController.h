//
//  TestController.hpp
//  xtypes-interop
//
//  Created by Kelvin Zhang on 5/29/18.
//  Copyright © 2018 Kelvin Zhang. All rights reserved.
//

#ifndef TestController_hpp
#define TestController_hpp

#include "DDS_Variants.h"

#define INTER_OP_CONTROL_TOPIC_MASTER_TO_SLAVE "xtype-interop-master"
#define INTER_OP_CONTROL_TOPIC_SLAVE_TO_MASTER "xtype-interop-slave"

#define DATA_REPRESENTATION_QOS_TOPIC "TestDataRepresentationQos"
#define DATA_REPRESENTATION_TOPIC "TestDataRepresentation"
#define HELLOWORLD_TOPIC "TestHelloWorld"
#define EXTENSIBILITY_TOPIC "TestExtensibility"
#define SERIALIZATION_TOPIC "TestSerialization"
#define FLATDATA_TOPIC "TestFlatData"
#define ZEROCOPY_TOPIC "TestZeroCopy"
#define ZEROCOPY_TYPE_NAME "ZeroCopyTestType"

//138 and 139 are reserved for this project
#define TEST_DOMAIN_ID 138


class TestController{
private:
    DDSDomainParticipant    *participant;
    DDSPublisher            *publisher;
    DDSSubscriber           *subscriber;
    DDSDataWriter           *writer;
    DDSDataReader           *reader;
    DDSWaitSet              *waitset;
    DDSStatusCondition      *status_condition;
    DDSTopic                *outbound_topic;
    DDSTopic                *inbound_topic;
    int                     seq;
    
public:
    TestController(
           DDSDomainParticipant *participant,
           DDSPublisher *publisher,
           DDSSubscriber *subscriber,
           bool isMaster);
    ~TestController();
    
    ControlMessage recv_msg(DDS_Duration_t timeout = DDS_DURATION_INFINITE);
    ControlMessage peek_msg();
    void send_msg(const ControlMessage & msg);
    void clear_msg();
    
    void send_ack();
    void recv_ack(DDS_Duration_t timeout = {20, 0});
    bool isSlaveLive();
    bool wait_for_slave_discovery(DDS_Duration_t timeout = {10, 0});
};

#endif /* TestController_hpp */
