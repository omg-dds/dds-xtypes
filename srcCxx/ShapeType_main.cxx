/* 
 * Reference source code for the XTYPES interoperability test
 * 
 * To compile the code you will need to define a variable that selects
 * the header/configuration for the specific vendor implementation
 * The possibilities are:
 *    RTI_CONNEXT_DDS
 *    TWINOAKS_COREDX
 */

#include <stdio.h>
#include <stdlib.h>
#include <csignal>
#include <climits>
#include <strings.h>
#include <time.h>

#include "ShapeType_variants.h"

using namespace DDS;

const long INVALID_LONG = -1L;
const long NOT_SET = -2L;
enum ExitStatus {
    OK        =  0,
    ERROR     = -1,
    INTERRUPT = -2,
    TIMEOUT   = -3
};

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

ExitStatus publish(DomainParticipant *participant, const char *topic_name, const char *type_name, const long samples, const long seconds)
{
    ExitStatus status = ExitStatus::OK;
    long count = 0;
    Duration_t send_period;
    send_period.sec     = 1;
    send_period.nanosec = 0;
    
    WriterBase *writer = ShapeTypeVariants::create_writer(type_name);

    if ( ( writer == NULL ) || (!writer->initialize(participant, topic_name)) ) {
       return ExitStatus::ERROR;
    }

    time_t start_time = time(NULL);
    /* Main loop */
    WaitSet *wait_set = new WaitSet();
    ConditionSeq active_cond;

    for (count=0; exit_application == false ; ++count) {
        wait_set->wait(active_cond, send_period);
        writer->write_data("BLUE", count);
        if (samples != NOT_SET && count+1 >= samples) {
            fprintf(stderr, "reached %ld samples, exiting\n", samples);
            status = ExitStatus::OK;
            break;
        }
        if (seconds != NOT_SET && time(NULL) > start_time + seconds) {
            fprintf(stderr, "reached %ld seconds, exiting\n", seconds);
            status = ExitStatus::TIMEOUT;
            break;
        }
    }
    delete wait_set;
    delete writer;
    if (exit_application) {
        status = ExitStatus::INTERRUPT;
    }
    return status;
}


ExitStatus subscribe(DomainParticipant *participant, const char *topic_name, const char *type_name, const long samples, const long seconds)
{
    ExitStatus exit_status = ExitStatus::OK;
    Duration_t receive_period;
    receive_period.sec     = 1;
    receive_period.nanosec = 0;
    
    ReaderBase *reader = ShapeTypeVariants::create_reader(type_name);
    if ( (reader == NULL) || (!reader->initialize(participant, topic_name)) ) {
        return ExitStatus::ERROR;
    }

    long total = 0L;  
    time_t start_time = time(NULL);
    /* Main loop */
    printf("Waiting for data on topic \"%s\", type \"%s\"\n", 
        reader->get_topic()->get_name(),
        reader->get_topic()->get_type_name());
  
    while (exit_application == false) {
        reader->wait_for_data(receive_period);
        int count = reader->take_data();
        if (count > 0) {
            total += count;
            if (samples != NOT_SET && total >= samples) {
                fprintf(stderr, "reached %ld samples, exiting\n", samples);
                break;
            }
        }
        if (seconds != NOT_SET && time(NULL) > start_time + seconds) {
            fprintf(stderr, "reached %ld seconds, exiting\n", seconds);
            break;
        }
    }
    
    delete reader;

    return exit_status;
}

 
ExitStatus run(int domain_id, const char *topic_name, const char *type_name, bool is_publisher, long samples, long seconds)
{
    ExitStatus exit_status = ExitStatus::OK;
    DomainParticipant *participant = create_participant(domain_id);
    if (participant == NULL) {
        printf("create_participant error\n");
        return ExitStatus::ERROR;
    }

    fprintf(stderr, "Info: Starting %s application. Domain: %d, Type: %s\n\n",
            is_publisher?"publishing":"subscribing",
            domain_id, type_name);
    if (seconds != NOT_SET) {
        fprintf(stderr, "Info: will stop after %ld seconds\n", seconds);
    }
    if (samples != NOT_SET) {
        fprintf(stderr, "Info: will stop after %ld samples\n", samples);
    }
 
    if ( is_publisher == true) {
        exit_status = publish(participant, topic_name, type_name, samples, seconds);
    } 
    else {
        exit_status = subscribe(participant, topic_name, type_name, samples, seconds);
    } 

    /* Delete all entities */
    participant->delete_contained_entities();
    DomainParticipantFactory::get_instance()->delete_participant(participant);

    return exit_status;
}

long validate_long(const char* str, const char* msg="", long min=0, long max=LONG_MAX)
{
    char * endptr = NULL;
    const int BASE = 10;

    long value = strtol(str, &endptr, BASE);
    if (str == endptr) {
        fprintf(stderr, "%s %s contains no digits\n", msg, str);
        return INVALID_LONG;
    }
    if (errno == ERANGE) {
        fprintf(stderr, "%s %s out of range\n", msg, str);
        return INVALID_LONG;
    }
    if (errno == EINVAL) {
        fprintf(stderr, "%s %s invalid\n", msg, str);
        return INVALID_LONG;
    }

    if (value < min || value > max) {
        fprintf(stderr, "%s Value %ld not in range %ld - %ld\n", 
                msg, value, min, max);
        return INVALID_LONG;
    }
    return value;
}

void show_usage(const char* iam, int status=0, bool quit=true, bool brief=false)
{
    char cmd[] = 
    "[-help] [help-type] "
    "[-pub | -sub] [-domain <domainId>] "
    "[-topic <topicName>] [-type <typeName>] "
    "[-timeout <seconds>] [-samples M]"
    ;
    fprintf(stderr, "Usage: %s %s\n\n", iam, cmd);

    char desc[] = 
    "-help              -- display this help\n"
    "-help-types        -- list <typeNames> available for \"-type\" option\n"
    "-pub               -- run as a publisher [default]\n"
    "-sub               -- run as a subscriber\n"
    "-domain <domainId> -- publish or subscribe on the DDS domain with id <domainId> [default: 0]\n"
    "-topic <topicName> -- publish or subscribe topic <topicName> [default: XTYPESTestTopic]\n"
    "-type  <typeName>  -- publish or subscribe the data type <typeName> [default: Shape1Default]\n"
    "-samples <count>   -- exit (successfuly) after sending or receiving <count> samples [default: unlimited]\n"
    "-timeout <seconds> -- exit (with error) after <seconds> seconds [default: unlimited]\n"
    ;
    if (! brief) {
        fprintf(stderr, "%s", desc);
    }
    if (quit) {
        exit(status);
    }
}

int main(int argc, char *argv[])
{
    enum PubSubMode {
        UNDEFINED = 0,
        PUBLISH = 1,
        SUBSCRIBE = 2
    };
    
    int pubsub_mode        = UNDEFINED;
    int domain_id          = -1;
    const char *type_name  = NULL;
    const char *topic_name = NULL;
    long samples           = NOT_SET;
    long seconds           = NOT_SET;

    for (int i=1; i<argc; ++i) {
        if ( strcmp(argv[i], "-pub") == 0 ) {
            pubsub_mode = 1;
        } else if ( strcmp(argv[i], "-sub") == 0 ) {
            pubsub_mode = 2;
        } else if ( strcmp(argv[i], "-domain") == 0 ) {
            if ( ++i >= argc) {
                fprintf(stderr, "Error: missing <domainId> after \"-domain\"\n");
                return ExitStatus::ERROR;
            }
            long domain_id_long = validate_long(argv[i], "domainId:", 0, 255);
            if (domain_id_long != INVALID_LONG) {
                domain_id = domain_id_long;
            } else {
                fprintf(stderr, "Error: invalid domainId");
                return ExitStatus::ERROR;
            }
        } else if ( strcmp(argv[i], "-topic") == 0 ) {
            if ( ++i >= argc) {
                fprintf(stderr, "Error: missing <topicName> after \"-topic\"\n");
                return ExitStatus::ERROR;
            }
            topic_name = argv[i];
        } else if ( strcmp(argv[i], "-type") == 0 ) {
            if ( ++i >= argc) {
                fprintf(stderr, "Error: missing <typeName> after \"-type\"\n");
                return ExitStatus::ERROR;
            }
            type_name = argv[i];
        } else if ( strcmp(argv[i], "-samples") == 0 ) {
            if ( ++i >= argc ) {
                fprintf(stderr, "Error: missing <M> after \"-samples\"\n");
                return ExitStatus::ERROR;
            }
            samples = validate_long(argv[i], "samples:", 1, 100);
            if ( samples == INVALID_LONG) {
                fprintf(stderr, "Error: invalid samples\n");
                return ExitStatus::ERROR;
            }
        } else if ( strcmp(argv[i], "-timeout") == 0 ) {
            if ( ++i >= argc ) {
                fprintf(stderr, "Error: missing <N> after \"-seconds\"\n");
                return ExitStatus::ERROR;
            }
            seconds = validate_long(argv[i], "timeout:", 2);
            if ( seconds == INVALID_LONG) {
                fprintf(stderr, "Error: invalid seconds\n");
                return ExitStatus::ERROR;
            }
        } else if ( strcmp(argv[i], "-help-types") == 0 ) {
            ShapeTypeVariants::print_type_variants(stderr);
            return ExitStatus::OK;
        } else if ( strcmp(argv[i], "-help") == 0 ) {
            show_usage(argv[0], 0);
            return ExitStatus::OK;
        } else {
            fprintf(stderr, "Error: unrecognized option: \"%s\"\n", argv[i]);
            show_usage(argv[0], 0);
            return ExitStatus::ERROR;
        }
    }

    if ( type_name == NULL ) {
        type_name = "Shape1Default";
        fprintf(stderr, "Info: \"-type\" was not specified. Defaulting to: \"%s\"\n", type_name);
    }
    
    if ( !ShapeTypeVariants::check_type_variant(type_name) ) {
        fprintf(stderr, "Error: Type \"%s\" is not a valid type. The possible types are:\n", type_name);
        ShapeTypeVariants::print_type_variants(stderr);
        return ExitStatus::ERROR;
    }
    
    if ( domain_id == -1 ) {
        domain_id = 0;
        fprintf(stderr, "Info: \"-domain\" was not specified. Defaulting to: %d\n", domain_id);
    }
    
    if ( topic_name == NULL ) {
        topic_name = "XTYPESTestTopic";
        fprintf(stderr, "Info: \"-topic\" was not specified. Defaulting to: \"%s\"\n", topic_name);
    }

    if ( pubsub_mode == UNDEFINED ) {
        pubsub_mode = PUBLISH;
        fprintf(stderr, "Info: Neither \"-pub\"  nor \"-sub\" was specified. Defaulting to: publish\n");
    }
 
    setup_signal_handler();
    
    return run(domain_id, topic_name, type_name, pubsub_mode == PUBLISH, samples, seconds);
}

