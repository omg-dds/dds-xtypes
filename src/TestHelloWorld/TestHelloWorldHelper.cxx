//
//  TestHelloWorldHelper.cpp
//  TestHelloWorldHelper
//
//  Created by Kelvin Zhang on 5/29/18.
//

//This is the slave process of TestHelloWorld fixture.
//Each fixture has its own slave process. Generally, the slave process will
//be the DataWriter and the master process will be the DataReader. Since each
//fixture can contain more than 1 tests, the slave process is reused across
//multiple tests of the same fixture to reduce process creation overhead.
//Master and slave process will communicate through DDS middleware to negotiate
//what to do.

#include "DDS_Variants.h"

#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <Utils.h>
#include "TestController.h"


int main(int argc, const char *argv[])
{
    DDSDomainParticipant *participant;
    DDSPublisher *publisher;
    DDSSubscriber *subscriber;
    //Controller is used to communicate with master process
    TestController *controller;
    
    //Create participants, publisher, subscriber, controller
    init_entities(participant, publisher, subscriber, controller, false);
    
    int ret = 0;
    
    //Register type and topic
    ret = Point2DFinalTypeSupport::register_type(
         participant,
         Point2DFinalTypeSupport::get_type_name());
    assert(ret == DDS_RETCODE_OK);
    
    DDSTopic *topic = participant->create_topic(
            HELLOWORLD_TOPIC,
            Point2DFinalTypeSupport::get_type_name(),
            DDS_TOPIC_QOS_DEFAULT,
            NULL,
            DDS_STATUS_MASK_NONE);
    assert(topic != NULL);
    assert(publisher != NULL);
    assert(subscriber != NULL);
    
    //Write 1 sample to this topic.
    DDSDataWriter *writer = publisher->create_datawriter(
         topic,
         get_test_datawriter_default_qos(),
         NULL,
         DDS_STATUS_MASK_NONE);
    assert(writer != NULL);
    
    Point2DFinal *sample = create_data<Point2DFinal>();
    Point2DFinalDataWriter *point_writer = Point2DFinalDataWriter::narrow(writer);
    assert(writer != NULL);
    point_writer->write(*sample, DDS_HANDLE_NIL);
    
    //Application level ack.
    //Wait for the master to receive the message before destroying data writer.
    controller->recv_ack();
    delete_data<Point2DFinal>(sample);
    
    delete controller;
    participant->delete_contained_entities();
    DDSTheParticipantFactory->delete_participant(participant);
    return 0;
}
