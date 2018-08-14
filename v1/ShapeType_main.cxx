/* 
 * Reference source code for the XTYPES interoperability test
 * 
 * To compile the code you will need to define a variable that selects
 * the header/configuration for the specific vendor implementation
 * The possibilities are:
 *    RTI_CONNEXT_DDS
 *    TWINOAKS_COREDX
 *    PRISMTECH_OPENSPLICE
 */

#include <stdio.h>
#include <stdlib.h>
#include <csignal>

#include "ShapeType_variants.h"

using namespace DDS;

// Keeps main thread iterating until a signal changes its value to false
bool exit_application = false;

// Signal handler changes the sets exit_service to true.
void signal_handler(int signal)
{
    fprintf(stderr, "Stopping...\n");
    exit_application = true;
}

// Assign all the exit signals to signal handler
void setup_signal_handler()
{
#ifndef _WIN32
    signal(SIGHUP,  signal_handler); //Terminal is closed
    signal(SIGQUIT, signal_handler); //Quit
#endif
    signal(SIGTERM, signal_handler); //Terminate
    signal(SIGINT,  signal_handler); //Interrupt
    signal(SIGABRT, signal_handler); //Abort
}

DomainParticipant *create_participant(int domain_id) 
{
    DomainParticipantQos pQos;
    DomainParticipantFactory::get_instance()->get_default_participant_qos(pQos);
 
#if defined(RTI_CONNEXT_DDS)
    pQos.discovery_config.participant_message_reader_reliability_kind = DDS_RELIABLE_RELIABILITY_QOS;
#endif
    
    DomainParticipant *participant = DomainParticipantFactory::get_instance()->create_participant(
        domain_id, pQos,             
        NULL /* listener */, DDS_STATUS_MASK_NONE);
    
    return participant;
}

int publish(DomainParticipant *participant, const char *type_name)
{
    int count = 0;  
    Duration_t send_period;
    send_period.sec     = 1;
    send_period.nanosec = 0;
    
    WriterBase *writer = ShapeTypeVariants::create_writer(type_name);

    if ( ( writer == NULL ) || (!writer->initialize(participant, "XTYPESTestTopic")) ) {
       return -1;
    }

    /* Main loop */
    WaitSet *wait_set = new WaitSet();
    ConditionSeq active_cond;
    for (count=0; exit_application == false ; ++count) {
        writer->write_data("BLUE", count);
        wait_set->wait(active_cond, send_period);
    }
    delete wait_set;
    delete writer;
    return 0;
}


int subscribe(DomainParticipant *participant, const char *type_name)
{
    Duration_t receive_period;
    receive_period.sec     = 1;
    receive_period.nanosec = 0;
    
    ReaderBase *reader = ShapeTypeVariants::create_reader(type_name);;
    if ( (reader == NULL) || (!reader->initialize(participant, "XTYPESTestTopic")) ) {
        return -1;
    }

    /* Main loop */
    printf("Waiting for data on topic \"%s\", type \"%s\"\n", 
        reader->get_topic()->get_name(),
        reader->get_topic()->get_type_name());
  
    while (exit_application == false) {
        reader->wait_for_data(receive_period);
        reader->take_data();
    }
    
    delete reader;

    return 0;
}

 
int run(int domain_id, const char *type_name, bool is_publisher)
{
    int exit_value = 0;
    DomainParticipant *participant = create_participant(domain_id);
    if (participant == NULL) {
        printf("create_participant error\n");
        return -1;
    }

    fprintf(stderr, "Info: Starting %s application. Domain: %d, Type: %s\n\n",
            is_publisher?"publishing":"subscribing",
            domain_id, type_name);
    
    if ( is_publisher == true) {
        exit_value = publish(participant, type_name);
    } 
    else {
        exit_value = subscribe(participant, type_name);
    } 

    /* Delete all entities */
    participant->delete_contained_entities();
    DomainParticipantFactory::get_instance()->delete_participant(participant);

    return exit_value;
}

int main(int argc, char *argv[])
{
    enum PubSubMode {
        UNDEFINED = 0,
        PUBLISH = 1,
        SUBSCRIBE = 2
    };
    
    int pubsub_mode       = UNDEFINED;
    int domain_id         = -1;
    const char *type_name = NULL;

    if ( argc != 5 ) {
        fprintf(stderr, "Usage:  %s [-pub | -sub] [-domain <domainId>] [-type <typeName>]\n", argv[0]);
    }
    
    for (int i=0; i<argc; ++i) {
        if ( strcmp(argv[i], "-pub") == 0 ) {
            pubsub_mode = 1;
        } else if ( strcmp(argv[i], "-sub") == 0 ) {
            pubsub_mode = 2;
        } else if ( strcmp(argv[i], "-domain") == 0 ) {
            if ( ++i == argc) {
                fprintf(stderr, "Error: missing <domainId> after \"-domain\"\n");
                return -1;
            }
            domain_id = atoi(argv[i]);
        } else if ( strcmp(argv[i], "-type") == 0 ) {
            if ( ++i == argc) {
                fprintf(stderr, "Error: missing <typeName> after \"-type\"\n");
                return -1;
            }
            type_name = argv[i];
        }       
    }

    if ( type_name == NULL ) {
        type_name = "Shape1Default";
        fprintf(stderr, "Info: \"-type\" was not specified. Defaulting to: \"%s\"\n", type_name);
    }
    
    if ( !ShapeTypeVariants::check_type_variant(type_name) ) {
        fprintf(stderr, "Error: Type \"%s\" is not a valid type. The possible types are:\n", type_name);
        ShapeTypeVariants::print_type_variants(stderr);
        return -1;
    }
    
    if ( domain_id == -1 ) {
        domain_id = 0;
        fprintf(stderr, "Info: \"-domain\" was not specified. Defaulting to: %d\n", domain_id);
    }
    
    if ( pubsub_mode == UNDEFINED ) {
        pubsub_mode = PUBLISH;
        fprintf(stderr, "Info: Neither \"-pub\"  nor \"-sub\" was specified. Defaulting to: publish\n");
    }
 
    setup_signal_handler();
    
    return run(domain_id, type_name, pubsub_mode == PUBLISH);
}

