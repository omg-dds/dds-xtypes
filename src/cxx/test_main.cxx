/****************************************************************
 * Use and redistribution is source and binary forms is permitted
 * subject to the OMG-DDS INTEROPERABILITY TESTING LICENSE found
 * at the following URL:
 *
 * https://github.com/omg-dds/dds-xtypes/blob/master/LICENSE.md
 */
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <getopt.h>
#include <iostream>

#define RTI_CONNEXT_DDS

#if defined(RTI_CONNEXT_DDS)
#include "variant_rti_connext_dds.h"
#elif defined(TWINOAKS_COREDX)
#include "variant_toc_coredx_dds.hh"
#elif defined(OPENDDS)
#include "variant_opendds.h"
#else
#error "Must define the DDS vendor"
#endif

#ifndef STRING_IN
#define STRING_IN
#endif
#ifndef STRING_INOUT
#define STRING_INOUT
#endif
#ifndef NAME_ACCESSOR
#define NAME_ACCESSOR
#endif
#ifndef LISTENER_STATUS_MASK_NONE
#define LISTENER_STATUS_MASK_NONE 0
#endif
#ifndef SECONDS_FIELD_NAME
#define SECONDS_FIELD_NAME sec
#endif
#ifndef FIELD_ACCESSOR
#define FIELD_ACCESSOR
#endif
#ifndef GET_TOPIC_DESCRIPTION
#define GET_TOPIC_DESCRIPTION(dr) dr->get_topicdescription()
#endif
#ifndef ADD_PARTITION
#define ADD_PARTITION(field, value) StringSeq_push(field.name, value)
#endif

using namespace DDS;

#define ERROR_PARSING_ARGUMENTS 1
#define ERROR_INITIALIZING 2
#define ERROR_RUNNING 3

/*************************************************************/
int  all_done  = 0;
/*************************************************************/
void
handle_sig(int sig)
{
  if (sig == SIGINT) {
    all_done = 1;
  }
}

/*************************************************************/
int
install_sig_handlers()
{
  struct sigaction int_action;
  int_action.sa_handler = handle_sig;
  sigemptyset(&int_action.sa_mask);
  sigaddset(&int_action.sa_mask, SIGINT);
  int_action.sa_flags     = 0;
  sigaction(SIGINT,  &int_action, NULL);
  return 0;
}

enum Verbosity
  {
    ERROR=1,
    DEBUG=2,
  };

class QosUtils {
public:
  static std::string to_string(ReliabilityQosPolicyKind reliability_value)
  {
    if (reliability_value == BEST_EFFORT_RELIABILITY_QOS){
      return "BEST_EFFORT";
    } else if (reliability_value == RELIABLE_RELIABILITY_QOS){
      return "RELIABLE";
    }
    return "Error stringifying Reliability kind.";
  }

  static std::string to_string(DurabilityQosPolicyKind durability_value)
  {
    if ( durability_value == VOLATILE_DURABILITY_QOS){
      return "VOLATILE";
    } else if (durability_value == TRANSIENT_LOCAL_DURABILITY_QOS){
      return "TRANSIENT_LOCAL";
    } else if (durability_value == TRANSIENT_DURABILITY_QOS){
      return "TRANSIENT";
    } else if (durability_value == PERSISTENT_DURABILITY_QOS){
      return "PERSISTENT";
    }
    return "Error stringifying Durability kind.";
  }

  static std::string to_string(DataRepresentationId_t data_representation_value)
  {
    if (data_representation_value == XCDR_DATA_REPRESENTATION){
      return "XCDR";
    } else if (data_representation_value == XCDR2_DATA_REPRESENTATION){
      return "XCDR2";
    }
    return "Error stringifying DataRepresentation.";
  }

  static std::string to_string(Verbosity verbosity_value)
  {
    switch (verbosity_value)
      {
      case ERROR:
        return "ERROR";
        break;

      case DEBUG:
        return "DEBUG";
        break;

      default:
        break;
      }
    return "Error stringifying verbosity.";
  }

  static std::string to_string(OwnershipQosPolicyKind ownership_kind_value)
  {
    if (ownership_kind_value == SHARED_OWNERSHIP_QOS){
      return "SHARED";
    } else if (ownership_kind_value == EXCLUSIVE_OWNERSHIP_QOS){
      return "EXCLUSIVE";
    }
    return "Error stringifying Ownership kind.";
  }

  static std::string to_string(HistoryQosPolicyKind history_kind_value)
  {
    if (history_kind_value == KEEP_ALL_HISTORY_QOS){
      return "KEEP_ALL";
    } else if (history_kind_value == KEEP_LAST_HISTORY_QOS){
      return "KEEP_LAST";
    }
    return "Error stringifying History kind.";
  }

  static std::string to_string(TypeConsistencyKind consistency_kind_value )
  {
    if (consistency_kind_value == DISALLOW_TYPE_COERCION){
      return "DISALLOW_TYPE_COERCION";
    } else if (consistency_kind_value == ALLOW_TYPE_COERCION){
      return "ALLOW_TYPE_COERCION";
    }
#if defined (RTI_CONNEXT_DDS)
    else if (consistency_kind_value == AUTO_TYPE_COERCION) {
      return "AUTO_TYPE_COERCION";
    }
#endif
    return "Error stringifying TypeConsistency kind.";
  }
};

class Logger{
public:
  Logger(enum Verbosity v)
  {
    verbosity_ = v;
  }

  void verbosity(enum Verbosity v)
  {
    verbosity_ = v;
  }

  enum Verbosity verbosity()
  {
    return verbosity_;
  }

  void log_message(std::string message, enum Verbosity level_verbosity)
  {
    if (level_verbosity <= verbosity_) {
      std::cout << message << std::endl;
    }
  }

private:
  enum Verbosity verbosity_;
};

/*************************************************************/
Logger logger(ERROR);


/*************************************************************/

#define OPT_FORCE_TYPE_VALIDATION    0x1000
#define OPT_CHECK_MEMBER_NAMES       0x1001
#define OPT_CHECK_SEQUENCE_BOUNDS    0x1002
#define OPT_CHECK_STRING_BOUNDS      0x1003
#define OPT_PREVENT_TYPE_WIDENING    0x1004
#define OPT_ALLOW_TYPE_COERCION      0x1005

static struct option long_opts[] =
  {
    { "help",                  no_argument, NULL, 'h'    },
    { "force-type-validation", required_argument, NULL, OPT_FORCE_TYPE_VALIDATION  },
    { "ignore-member-names",   required_argument, NULL, OPT_CHECK_MEMBER_NAMES     },
    { "ignore-seq-bounds",     required_argument, NULL, OPT_CHECK_SEQUENCE_BOUNDS  },
    { "ignore-str-bounds",     required_argument, NULL, OPT_CHECK_STRING_BOUNDS    },
    { "prevent-type-widening", required_argument, NULL, OPT_PREVENT_TYPE_WIDENING  },
    { "allow-type-coercion",   required_argument, NULL, OPT_ALLOW_TYPE_COERCION    },
    { NULL, 0, NULL, 0 }
  };


/*************************************************************/
class TestOptions {
public:
  DomainId_t                     domain_id;
  ReliabilityQosPolicyKind       reliability_kind;
  DurabilityQosPolicyKind        durability_kind;
  DataRepresentationId_t         data_representation;
  int                            history_depth;
  int                            ownership_strength;

  DDS::TypeConsistencyEnforcementQosPolicy type_consistency;

  char               *topic_name;
  char               *type_name;
  char               *types_uri;   /* xml file of defined types */

  char               *xml_data_uri;    /* xml file of data sample */
  char               *json_data_uri;    /* json file of data sample */

  char               *partition;

  bool                publish;
  bool                subscribe;

  int                 timebasedfilter_interval;
  int                 deadline_interval;

  bool                print_writer_samples;


public:
  //-------------------------------------------------------------
  TestOptions()
  {
    domain_id           = 0;
    reliability_kind    = RELIABLE_RELIABILITY_QOS;
    durability_kind     = VOLATILE_DURABILITY_QOS;
    data_representation = XCDR_DATA_REPRESENTATION;
    history_depth       = -1;      /* means default */
    ownership_strength  = -1;      /* means shared */

    type_consistency.force_type_validation  = DDS_BOOLEAN_FALSE;
    type_consistency.ignore_member_names    = DDS_BOOLEAN_TRUE;
    type_consistency.ignore_sequence_bounds = DDS_BOOLEAN_TRUE;
    type_consistency.ignore_string_bounds   = DDS_BOOLEAN_TRUE;
    type_consistency.prevent_type_widening  = DDS_BOOLEAN_FALSE;
    type_consistency.kind                   = ALLOW_TYPE_COERCION;
#if defined (RTI_CONNEXT_DDS)
    type_consistency.ignore_enum_literal_names =
            TypeConsistency_get_default().ignore_enum_literal_names;
#endif

    topic_name         = NULL;
    type_name          = NULL;
    partition          = NULL;
    types_uri          = NULL;
    xml_data_uri           = NULL;
    json_data_uri           = NULL;

    publish            = false;
    subscribe          = false;

    timebasedfilter_interval = 0; /* off */
    deadline_interval        = 0; /* off */

    print_writer_samples = false;
  }

  //-------------------------------------------------------------
  ~TestOptions()
  {
    if (topic_name)       free(topic_name);
    if (type_name)        free(type_name);
    if (types_uri)        free(types_uri);
    if (xml_data_uri)     free(xml_data_uri);
    if (json_data_uri)    free(json_data_uri);
    if (partition)        free(partition);
  }

  //-------------------------------------------------------------
  void print_usage( const char *prog )
  {
    printf("%s: \n", prog);
    printf("   -d <int>        : domain id (default: 0)\n");
    printf("   -b              : BEST_EFFORT reliability\n");
    printf("   -r              : RELIABLE reliability\n");
    printf("   -k <depth>      : keep history depth [0: KEEP_ALL]\n");
    printf("   -f <interval>   : set a 'deadline' with interval (seconds) [0: OFF]\n");
    printf("   -i <interval>   : apply 'time based filter' with interval (seconds) [0: OFF]\n");
    printf("   -s <int>        : set ownership strength [-1: SHARED]\n");
    printf("   -t <topic_name> : set the topic name\n");
    printf("   -y <type_name>  : set the type name\n");
    printf("   -p <partition>  : set a 'partition' string\n");
    printf("   -D [v|l|t|p]    : set durability [v: VOLATILE,  l: TRANSIENT_LOCAL]\n");
    printf("                                     t: TRANSIENT, p: PERSISTENT]\n");
    printf("   -P              : publish samples\n");
    printf("   -S              : subscribe samples\n");
    printf("   -x [1|2]        : set data representation [1: XCDR, 2: XCDR2]\n");
    printf("   -X <types_uri>  : xml file with type definitions\n");
    printf("   -V <xml_data_uri> : xml file with data sample values. XML and JSON may be\n");
    printf("                       provided, the app is in charge of using what it needs\n");
    printf("   -J <json_data_uri> : json file with data sample values. XML and JSON may be\n");
    printf("                        provided, the app is in charge of using what it needs\n");
    printf("   -w              : print Publisher's samples\n");
    printf("   --force-type-validation [t|f|d]: enable, disable or default value for\n");
    printf("                                 type_consistency.force_type_validation\n");
    printf("   --ignore-member-names [t|f|d]: enable, disable or default value for\n");
    printf("                                 type_consistency.ignore_member_names\n");
    printf("   --ignore-seq-bounds [t|f|d]: enable, disable or default value for\n");
    printf("                                 type_consistency.ignore_sequence_bounds\n");
    printf("   --ignore-str-bounds [t|f|d]: enable, disable or default value for\n");
    printf("                                 type_consistency.ignore_string_bounds\n");
    printf("   --prevent-type-widening [t|f|d]: enable, disable or default value for\n");
    printf("                                 type_consistency.prevent_type_widening\n");
    printf("   --allow-type-coercion [t|f|d]: enable, disable type coercion or default\n");
    printf("                                 value for type_consistency.kind\n");
    printf("   -v [e|d]        : set log message verbosity [e: ERROR, d: DEBUG]\n");
  }

  //-------------------------------------------------------------
  bool validate() {
    if (topic_name == NULL) {
      logger.log_message("topic name unspecified [-t], using \"test\" by default", Verbosity::ERROR);
      return false;
    }
    if (type_name == NULL) {
      logger.log_message("please specify type name [-y]", Verbosity::ERROR);
      return false;
    }
    if ( (!publish) && (!subscribe) ) {
      logger.log_message("please specify publish [-P] or subscribe [-S]", Verbosity::ERROR);
      return false;
    }
    if ( publish && subscribe ) {
      logger.log_message("please specify only one of: publish [-P] or subscribe [-S]", Verbosity::ERROR);
      return false;
    }
    if ( xml_data_uri == NULL && json_data_uri == NULL ) {
      logger.log_message("please provide the data either in XML [-V] or JSON [-J]", Verbosity::ERROR);
      return false;
    }
    if ( types_uri == NULL ) {
      logger.log_message("please provide the types in XML [-X]", Verbosity::ERROR);
      return false;
    }
    if ( type_consistency.kind == DISALLOW_TYPE_COERCION
            && type_consistency.ignore_sequence_bounds == DDS_BOOLEAN_TRUE
            && type_consistency.ignore_string_bounds == DDS_BOOLEAN_TRUE
            && type_consistency.ignore_member_names == DDS_BOOLEAN_TRUE
            && type_consistency.prevent_type_widening == DDS_BOOLEAN_TRUE ) {
      logger.log_message("warning: prevent_type_widening, ignore_sequence_bounds, "
            "ignore_string_bounds, and ignore_member_names only apply when the "
            "type consistency kind is ALLOW_TYPE_COERCION", Verbosity::ERROR);
    }
    return true;
  }


  //-------------------------------------------------------------
  bool parse(int argc, char *argv[])
  {
    int opt;
    bool parse_ok = true;
    // double d;
    while ((opt = getopt_long(argc, argv,
                              "hbrd:D:f:i:k:p:s:x:X:t:v:V:J:wy:PS",
                              long_opts, NULL)) != -1)
      {
        switch (opt)
          {
          case 'v':
            {
              if (optarg[0] != '\0')
                {
                  switch (optarg[0])
                    {
                    case 'd':
                      {
                        logger.verbosity(DEBUG);
                        break;
                      }
                    case 'e':
                      {
                        logger.verbosity(ERROR);
                        break;
                      }
                    default:
                      {
                        logger.log_message("unrecognized value for verbosity "
                                           + std::string(1, optarg[0]),
                                           Verbosity::ERROR);
                        parse_ok = false;
                      }
                    }
                }
              break;
            }
          case 'w':
            {
              print_writer_samples = true;
              break;
            }
          case 'b':
            {
              reliability_kind = BEST_EFFORT_RELIABILITY_QOS;
              break;
            }
          case 'd':
            {
              int converted_param = sscanf(optarg, "%d", &domain_id);
              if (converted_param == 0) {
                logger.log_message("unrecognized value for domain_id "
                                   + std::string(1, optarg[0]),
                                   Verbosity::ERROR);
                parse_ok = false;
              }
              else if (domain_id < 0) {
                logger.log_message("incorrect value for domain_id "
                                   + std::to_string(domain_id),
                                   Verbosity::ERROR);
                parse_ok = false;
              }
              break;
            }
          case 'D':
            {
              if (optarg[0] != '\0')
                {
                  switch (optarg[0])
                    {
                    case 'v':
                      {
                        durability_kind = VOLATILE_DURABILITY_QOS;
                        break;
                      }
                    case 'l':
                      {
                        durability_kind = TRANSIENT_LOCAL_DURABILITY_QOS;
                        break;
                      }
                    case 't':
                      {
                        durability_kind = TRANSIENT_DURABILITY_QOS;
                        break;
                      }
                    case 'p':
                      {
                        durability_kind = PERSISTENT_DURABILITY_QOS;
                        break;
                      }
                    default:
                      {
                        logger.log_message("unrecognized value for durability "
                                           + std::string(1, optarg[0]),
                                           Verbosity::ERROR);
                        parse_ok = false;
                      }
                    }
                }
              break;
            }
          case 'i':
            {
              int converted_param = sscanf(optarg, "%d", &timebasedfilter_interval);
              if (converted_param == 0) {
                logger.log_message("unrecognized value for timebasedfilter_interval "
                                   + std::string(1, optarg[0]),
                                   Verbosity::ERROR);
                parse_ok = false;
              }
              else if (timebasedfilter_interval < 0) {
                logger.log_message("incorrect value for timebasedfilter_interval "
                                   + std::to_string(timebasedfilter_interval),
                                   Verbosity::ERROR);
                parse_ok = false;
              }
              break;
            }
          case 'f':
            {
              int converted_param = sscanf(optarg, "%d", &deadline_interval);
              if (converted_param == 0) {
                logger.log_message("unrecognized value for deadline_interval "
                                   + std::string(1, optarg[0]),
                                   Verbosity::ERROR);
                parse_ok = false;
              } else if (deadline_interval < 0) {
                logger.log_message("incorrect value for deadline_interval "
                                   + std::to_string(deadline_interval),
                                   Verbosity::ERROR);
                parse_ok = false;
              }
              break;
            }
          case 'k':
            {
              int converted_param = sscanf(optarg, "%d", &history_depth);
              if (converted_param == 0){
                logger.log_message("unrecognized value for history_depth "
                                   + std::string(1, optarg[0]),
                                   Verbosity::ERROR);
                parse_ok = false;
              } else if (history_depth < 0) {
                logger.log_message("incorrect value for history_depth "
                                   + std::to_string(history_depth),
                                   Verbosity::ERROR);
                parse_ok = false;
              }
              break;
            }
          case 'p':
            {
              partition = strdup(optarg);
              break;
            }
          case 'r':
            {
              reliability_kind = RELIABLE_RELIABILITY_QOS;
              break;
            }
          case 's':
            {
              int converted_param = sscanf(optarg, "%d", &ownership_strength);
              if (converted_param == 0){
                logger.log_message("unrecognized value for ownership_strength "
                                   + std::string(1, optarg[0]),
                                   Verbosity::ERROR);
                parse_ok = false;
              } else if (ownership_strength < -1) {
                logger.log_message("incorrect value for ownership_strength "
                                   + std::to_string(ownership_strength),
                                   Verbosity::ERROR);
                parse_ok = false;
              }
              break;
            }
          case 't':
            {
              topic_name = strdup(optarg);
              break;
            }
          case 'P':
            {
              publish = true;
              break;
            }
          case 'S':
            {
              subscribe = true;
              break;
            }
          case 'x':
            {
              if (optarg[0] != '\0')
                {
                  switch (optarg[0])
                    {
                    case '1':
                      {
                        data_representation = XCDR_DATA_REPRESENTATION;
                        break;
                      }
                    case '2':
                      {
                        data_representation = XCDR2_DATA_REPRESENTATION;
                        break;
                      }
                    default:
                      {
                        logger.log_message("unrecognized value for data representation "
                                           + std::string(1, optarg[0]),
                                           Verbosity::ERROR);
                        parse_ok = false;
                      }
                    }
                }
              break;
            }
          case 'X':
            {
              types_uri = strdup(optarg);
              break;
            }
          case 'y':
            {
              type_name = strdup(optarg);
              break;
            }
          case 'V':
            {
              xml_data_uri = strdup(optarg);
              break;
            }
          case 'J':
            {
              json_data_uri = strdup(optarg);
              break;
            }
          case OPT_FORCE_TYPE_VALIDATION:
            if (optarg[0] != '\0') {
                switch (optarg[0]) {
                case 't':
                    type_consistency.force_type_validation = DDS_BOOLEAN_TRUE;
                    break;
                case 'f':
                    type_consistency.force_type_validation = DDS_BOOLEAN_FALSE;
                    break;
                case 'd':
                    type_consistency.force_type_validation =
                            TypeConsistency_get_default().force_type_validation;
                    break;
                default:
                    logger.log_message("unrecognized value for type_consistency."
                            "force_type_validation"
                            + std::string(1, optarg[0]),
                            Verbosity::ERROR);
                    parse_ok = false;
                    break;
                }
            }
            break;

          case OPT_CHECK_MEMBER_NAMES:
            if (optarg[0] != '\0') {
                switch (optarg[0]) {
                case 't':
                    type_consistency.ignore_member_names = DDS_BOOLEAN_TRUE;
                    break;
                case 'f':
                    type_consistency.ignore_member_names = DDS_BOOLEAN_FALSE;
                    break;
                case 'd':
                    type_consistency.ignore_member_names =
                            TypeConsistency_get_default().ignore_member_names;
                    break;
                default:
                    logger.log_message("unrecognized value for type_consistency."
                            "ignore_member_names"
                            + std::string(1, optarg[0]),
                            Verbosity::ERROR);
                    parse_ok = false;
                    break;
                }
            }
            break;

          case OPT_CHECK_SEQUENCE_BOUNDS:
            if (optarg[0] != '\0') {
                switch (optarg[0]) {
                case 't':
                    type_consistency.ignore_sequence_bounds = DDS_BOOLEAN_TRUE;
                    break;
                case 'f':
                    type_consistency.ignore_sequence_bounds = DDS_BOOLEAN_FALSE;
                    break;
                case 'd':
                    type_consistency.ignore_sequence_bounds =
                            TypeConsistency_get_default().ignore_sequence_bounds;
                    break;
                default:
                    logger.log_message("unrecognized value for type_consistency."
                            "ignore_sequence_bounds"
                            + std::string(1, optarg[0]),
                            Verbosity::ERROR);
                    parse_ok = false;
                    break;
                }
            }
            break;

          case OPT_CHECK_STRING_BOUNDS:
            if (optarg[0] != '\0') {
                switch (optarg[0]) {
                case 't':
                    type_consistency.ignore_string_bounds = DDS_BOOLEAN_TRUE;
                    break;
                case 'f':
                    type_consistency.ignore_string_bounds = DDS_BOOLEAN_FALSE;
                    break;
                case 'd':
                    type_consistency.ignore_string_bounds =
                            TypeConsistency_get_default().ignore_string_bounds;
                    break;
                default:
                    logger.log_message("unrecognized value for type_consistency."
                            "ignore_string_bounds"
                            + std::string(1, optarg[0]),
                            Verbosity::ERROR);
                    parse_ok = false;
                    break;
                }
            }
            break;

          case OPT_PREVENT_TYPE_WIDENING:
            if (optarg[0] != '\0') {
                switch (optarg[0]) {
                case 't':
                    type_consistency.prevent_type_widening = DDS_BOOLEAN_TRUE;
                    break;
                case 'f':
                    type_consistency.prevent_type_widening = DDS_BOOLEAN_FALSE;
                    break;
                case 'd':
                    type_consistency.prevent_type_widening =
                            TypeConsistency_get_default().prevent_type_widening;
                    break;
                default:
                    logger.log_message("unrecognized value for type_consistency."
                            "prevent_type_widening"
                            + std::string(1, optarg[0]),
                            Verbosity::ERROR);
                    parse_ok = false;
                    break;
                }
            }
            break;

          case OPT_ALLOW_TYPE_COERCION:
            if (optarg[0] != '\0') {
                switch (optarg[0]) {
                case 't':
                    type_consistency.kind = ALLOW_TYPE_COERCION;
                    break;
                case 'f':
                    type_consistency.kind = DISALLOW_TYPE_COERCION;
                    break;
                case 'd':
                    type_consistency.kind =
                            TypeConsistency_get_default().kind;
                    break;
                default:
                    logger.log_message("unrecognized value for type_consistency."
                            "kind"
                            + std::string(1, optarg[0]),
                            Verbosity::ERROR);
                    parse_ok = false;
                    break;
                }
            }
            break;

          case 'h':
            {
              print_usage(argv[0]);
              exit(0);
              break;
            }
          case '?':
            {
              parse_ok = false;
              break;
            }
          }

      }

    if ( parse_ok ) {
      if ( topic_name == NULL ) {
        topic_name = strdup( "test" ); /* default.. */
      }
      parse_ok = validate();
    }
    if ( !parse_ok ) {
      print_usage(argv[0]);
    } else {
        std::string app_kind = publish ? "publisher" : "subscriber";
        logger.log_message("Test Options: "
                "\n    This application is a " + app_kind +
                "\n    DomainId = " + std::to_string(domain_id) +
                "\n    ReliabilityKind = " + QosUtils::to_string(reliability_kind) +
                "\n    DurabilityKind = " + QosUtils::to_string(durability_kind) +
                "\n    DataRepresentation = " + QosUtils::to_string(data_representation) +
                "\n    HistoryDepth = " + std::to_string(history_depth) +
                "\n    OwnershipStrength = " + std::to_string(ownership_strength) +
                "\n    TimeBasedFilterInterval = " + std::to_string(timebasedfilter_interval) +
                "\n    DeadlineInterval = " + std::to_string(deadline_interval) +
                "\n    Type consistency kind = " + QosUtils::to_string(type_consistency.kind) +
                "\n    Verbosity = " + QosUtils::to_string(logger.verbosity()),
            Verbosity::DEBUG);
        if (!publish) {
            logger.log_message(
                    "\n    Force type validation = " + std::to_string(type_consistency.force_type_validation) +
                    "\n    Ignore member names = " + std::to_string(type_consistency.ignore_member_names) +
                    "\n    Ignore sequence bounds = " + std::to_string(type_consistency.ignore_sequence_bounds) +
                    "\n    Ignore string bounds = " + std::to_string(type_consistency.ignore_string_bounds) +
                    "\n    Prevent type widening = " + std::to_string(type_consistency.prevent_type_widening),
                Verbosity::DEBUG);
      }

      if (topic_name != NULL){
        logger.log_message("    Topic = " + std::string(topic_name),
                           Verbosity::DEBUG);
      }
      if (type_name != NULL){
        logger.log_message("    Type  = " + std::string(type_name),
                           Verbosity::DEBUG);
      }
      if (types_uri != NULL){
        logger.log_message("    Types URI = " + std::string(types_uri),
                           Verbosity::DEBUG);
      }
      if (xml_data_uri != NULL){
        logger.log_message("    Data  URI = " + std::string(xml_data_uri),
                           Verbosity::DEBUG);
      }
      if (json_data_uri != NULL){
        logger.log_message("    Data  URI = " + std::string(json_data_uri),
                           Verbosity::DEBUG);
      }
      if (partition != NULL) {
        logger.log_message("    Partition = " + std::string(partition), Verbosity::DEBUG);
      }
    }
    return parse_ok;
  }
};

/*************************************************************/
class DPListener : public DomainParticipantListener
{
public:
  void on_inconsistent_topic         (Topic *topic,  const InconsistentTopicStatus &) {
    const char *topic_name = topic->get_name();
    const char *type_name  = topic->get_type_name();
    printf("%s() topic: '%s'  type: '%s'\n", __FUNCTION__, topic_name, type_name);
  }

  void on_offered_incompatible_qos(DataWriter *dw,  const OfferedIncompatibleQosStatus & status) {
    Topic      *topic       = dw->get_topic( );
    const char *topic_name  = topic->get_name( );
    const char *type_name   = topic->get_type_name( );
    const char *policy_name = NULL;
    policy_name = get_qos_policy_name(status.last_policy_id);
    printf("%s() topic: '%s'  type: '%s' : %d (%s)\n", __FUNCTION__,
           topic_name, type_name,
           status.last_policy_id,
           policy_name );
  }

  void on_publication_matched (DataWriter *dw, const PublicationMatchedStatus & status) {
    Topic      *topic      = dw->get_topic( );
    const char *topic_name = topic->get_name( );
    const char *type_name  = topic->get_type_name( );
    printf("%s() topic: '%s'  type: '%s' : matched readers %d (change = %d)\n", __FUNCTION__,
           topic_name, type_name, status.current_count, status.current_count_change);
  }

  void on_offered_deadline_missed (DataWriter *dw, const OfferedDeadlineMissedStatus & status) {
    Topic      *topic      = dw->get_topic( );
    const char *topic_name = topic->get_name( );
    const char *type_name  = topic->get_type_name( );
    printf("%s() topic: '%s'  type: '%s' : (total = %d, change = %d)\n", __FUNCTION__,
           topic_name, type_name, status.total_count, status.total_count_change);
  }

  void on_liveliness_lost (DataWriter *dw, const LivelinessLostStatus & status) {
    Topic      *topic      = dw->get_topic( );
    const char *topic_name = topic->get_name( );
    const char *type_name  = topic->get_type_name( );
    printf("%s() topic: '%s'  type: '%s' : (total = %d, change = %d)\n", __FUNCTION__,
           topic_name, type_name, status.total_count, status.total_count_change);
  }

  void on_requested_incompatible_qos (DataReader *dr, const RequestedIncompatibleQosStatus & status) {
    TopicDescription *td         = dr->get_topicdescription( );
    const char       *topic_name = td->get_name( );
    const char       *type_name  = td->get_type_name( );
    const char *policy_name = NULL;
    policy_name = get_qos_policy_name(status.last_policy_id);
    printf("%s() topic: '%s'  type: '%s' : %d (%s)\n", __FUNCTION__,
           topic_name, type_name, status.last_policy_id,
           policy_name);
  }

  void on_subscription_matched (DataReader *dr, const SubscriptionMatchedStatus & status) {
    TopicDescription *td         = dr->get_topicdescription( );
    const char       *topic_name = td->get_name( );
    const char       *type_name  = td->get_type_name( );
    printf("%s() topic: '%s'  type: '%s' : matched writers %d (change = %d)\n", __FUNCTION__,
           topic_name, type_name, status.current_count, status.current_count_change);
  }

  void on_requested_deadline_missed (DataReader *dr, const RequestedDeadlineMissedStatus & status) {
    TopicDescription *td         = dr->get_topicdescription( );
    const char       *topic_name = td->get_name( );
    const char       *type_name  = td->get_type_name( );
    printf("%s() topic: '%s'  type: '%s' : (total = %d, change = %d)\n", __FUNCTION__,
           topic_name, type_name, status.total_count, status.total_count_change);
  }

  void on_liveliness_changed (DataReader *dr, const LivelinessChangedStatus & status) {
    TopicDescription *td         = dr->get_topicdescription( );
    const char       *topic_name = td->get_name( );
    const char       *type_name  = td->get_type_name( );
    printf("%s() topic: '%s'  type: '%s' : (alive = %d, not_alive = %d)\n", __FUNCTION__,
           topic_name, type_name, status.alive_count, status.not_alive_count);
  }

  void on_sample_rejected (DataReader *, const SampleRejectedStatus &) {}
  void on_data_available (DataReader *) {}
  void on_sample_lost (DataReader *, const SampleLostStatus &) {}
  void on_data_on_readers (Subscriber *) {}
};


/*************************************************************/
class TestApplication {

private:
  DPListener               dp_listener;

  DomainParticipantFactory *dpf;
  DomainParticipant        *dp;
  Publisher                *pub;
  Subscriber               *sub;
  Topic                    *topic;
  DataReader               *dr;
  DataWriter               *dw;
  DynamicType              *dt;

public:
  //-------------------------------------------------------------
  TestApplication()
  {
    dpf = NULL;
    dp  = NULL;

    pub = NULL;
    sub = NULL;
  }

  //-------------------------------------------------------------
  ~TestApplication()
  {
    if (dp)  dp->delete_contained_entities( );
    if (dpf) dpf->delete_participant( dp );
  }

  //-------------------------------------------------------------
  bool initialize(TestOptions *options)
  {
#ifndef OBTAIN_DOMAIN_PARTICIPANT_FACTORY
#define OBTAIN_DOMAIN_PARTICIPANT_FACTORY DomainParticipantFactory::get_instance()
#endif
    DomainParticipantFactory *dpf = OBTAIN_DOMAIN_PARTICIPANT_FACTORY;
    if (dpf == NULL) {
      logger.log_message("failed to create participant factory (missing license?).", Verbosity::ERROR);
      return false;
    }
    logger.log_message("Participant Factory created", Verbosity::DEBUG);
#ifdef CONFIGURE_PARTICIPANT_FACTORY
    CONFIGURE_PARTICIPANT_FACTORY
#endif

      dp = dpf->create_participant( options->domain_id, PARTICIPANT_QOS_DEFAULT, &dp_listener, LISTENER_STATUS_MASK_ALL );
    if (dp == NULL) {
      logger.log_message("failed to create participant (missing license?).", Verbosity::ERROR);
      return false;
    }
    logger.log_message("Participant created", Verbosity::DEBUG);

    dt = CREATE_TYPE( dp, options->types_uri, options->type_name );
    if (dt == NULL) {
        logger.log_message("failed to create type", Verbosity::ERROR);
        return false;
    }

    if (REGISTER_TYPE(dp, dt, options->type_name) != DDS_RETCODE_OK) {
        logger.log_message("failed to register type", Verbosity::ERROR);
        return false;
    }

    printf("Create topic: %s\n", options->topic_name );
    topic = dp->create_topic( options->topic_name, options->type_name, TOPIC_QOS_DEFAULT, NULL, 0);
    if (topic == NULL) {
      logger.log_message("failed to create topic", Verbosity::ERROR);
      return false;
    }

    if ( options->publish ) {
      return init_publisher(options);
    } else {
      return init_subscriber(options);
    }
  }

  //-------------------------------------------------------------
  bool run(TestOptions *options)
  {
    if ( pub != NULL ) {
      return run_publisher(options);
    } else if ( sub != NULL ) {
      return run_subscriber(options);
    }
    CLEANUP_TYPE( dp, dt );
    return false;
  }

  //-------------------------------------------------------------
  bool init_publisher(TestOptions *options)
  {
    logger.log_message("Initializing Publisher", Verbosity::DEBUG);
    PublisherQos  pub_qos;
    DataWriterQos dw_qos;

    dp->get_default_publisher_qos( pub_qos );
    if ( options->partition != NULL ) {
      StringSeq_push(pub_qos.partition.name, options->partition);
    }

    pub = dp->create_publisher(pub_qos, NULL, 0);
    if (pub == NULL) {
      logger.log_message("failed to create publisher", Verbosity::ERROR);
      return false;
    }
    logger.log_message("Publisher created", Verbosity::DEBUG);
    logger.log_message("Data Writer QoS:", Verbosity::DEBUG);
    pub->get_default_datawriter_qos( dw_qos );
    dw_qos.reliability.kind = options->reliability_kind;
    logger.log_message("    Reliability = " + QosUtils::to_string(dw_qos.reliability.kind), Verbosity::DEBUG);
    dw_qos.durability.kind  = options->durability_kind;
    logger.log_message("    Durability = " + QosUtils::to_string(dw_qos.durability.kind), Verbosity::DEBUG);

#if   defined(RTI_CONNEXT_DDS)
    DataRepresentationIdSeq data_representation_seq;
    data_representation_seq.ensure_length(1,1);
    data_representation_seq[0] = options->data_representation;
    dw_qos.representation.value = data_representation_seq;

#elif defined(TWINOAKS_COREDX) || defined(OPENDDS)
    dw_qos.representation.value.length(1);
    dw_qos.representation.value[0] = options->data_representation;
#endif
    logger.log_message("    Data_Representation = " + QosUtils::to_string(dw_qos.representation.value[0]), Verbosity::DEBUG);
    if ( options->ownership_strength != -1 ) {
      dw_qos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
      dw_qos.ownership_strength.value = options->ownership_strength;
    }

    if ( options->ownership_strength == -1 ) {
      dw_qos.ownership.kind = SHARED_OWNERSHIP_QOS;
    }
    logger.log_message("    Ownership = " + QosUtils::to_string(dw_qos.ownership.kind), Verbosity::DEBUG);
    if (dw_qos.ownership.kind == EXCLUSIVE_OWNERSHIP_QOS){
      logger.log_message("    OwnershipStrength = " + std::to_string(dw_qos.ownership_strength.value), Verbosity::DEBUG);
    }

    if ( options->deadline_interval > 0 ) {
      dw_qos.deadline.period.sec      = options->deadline_interval;
      dw_qos.deadline.period.nanosec  = 0;
    }
    logger.log_message("    DeadlinePeriod = " + std::to_string(dw_qos.deadline.period.sec), Verbosity::DEBUG);

    // options->history_depth < 0 means leave default value
    if ( options->history_depth > 0 )  {
      dw_qos.history.kind  = KEEP_LAST_HISTORY_QOS;
      dw_qos.history.depth = options->history_depth;
    }
    else if ( options->history_depth == 0 ) {
      dw_qos.history.kind  = KEEP_ALL_HISTORY_QOS;
    }
    logger.log_message("    History = " + QosUtils::to_string(dw_qos.history.kind), Verbosity::DEBUG);
    if (dw_qos.history.kind == KEEP_LAST_HISTORY_QOS){
      logger.log_message("    HistoryDepth = " + std::to_string(dw_qos.history.depth), Verbosity::DEBUG);
    }

    printf("Create writer for topic: %s type: %s\n", options->topic_name, options->type_name );

    dw = pub->create_datawriter( topic, dw_qos, NULL, 0);

    if (dw == NULL) {
      logger.log_message("failed to create datawriter", Verbosity::ERROR);
      return false;
    }

    logger.log_message("Data Writer created", Verbosity::DEBUG);

    return true;
  }

  //-------------------------------------------------------------
  bool init_subscriber(TestOptions *options)
  {
    SubscriberQos sub_qos;
    DataReaderQos dr_qos;

    dp->get_default_subscriber_qos( sub_qos );
    if ( options->partition != NULL ) {
      StringSeq_push(sub_qos.partition.name, options->partition);
    }

    sub = dp->create_subscriber( sub_qos, NULL, 0 );
    if (sub == NULL) {
      logger.log_message("failed to create subscriber", Verbosity::ERROR);
      return false;
    }
    logger.log_message("Subscriber created", Verbosity::DEBUG);
    logger.log_message("Data Reader QoS:", Verbosity::DEBUG);
    sub->get_default_datareader_qos( dr_qos );
    dr_qos.reliability.kind = options->reliability_kind;
    logger.log_message("    Reliability = " + QosUtils::to_string(dr_qos.reliability.kind), Verbosity::DEBUG);
    dr_qos.durability.kind  = options->durability_kind;
    logger.log_message("    Durability = " + QosUtils::to_string(dr_qos.durability.kind), Verbosity::DEBUG);

#if   defined(RTI_CONNEXT_DDS)
    DataRepresentationIdSeq data_representation_seq;
    data_representation_seq.ensure_length(1,1);
    data_representation_seq[0] = options->data_representation;
    dr_qos.representation.value = data_representation_seq;
#elif defined(TWINOAKS_COREDX) || defined(OPENDDS)
    dr_qos.representation.value.length(1);
    dr_qos.representation.value[0] = options->data_representation;
#endif
    logger.log_message("    DataRepresentation = " + QosUtils::to_string(dr_qos.representation.value[0]), Verbosity::DEBUG);
    if ( options->ownership_strength != -1 ) {
      dr_qos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
    }
    logger.log_message("    Ownership = " + QosUtils::to_string(dr_qos.ownership.kind), Verbosity::DEBUG);
    if ( options->timebasedfilter_interval > 0) {
      dr_qos.time_based_filter.minimum_separation.sec      = options->timebasedfilter_interval;
      dr_qos.time_based_filter.minimum_separation.nanosec  = 0;
    }
    logger.log_message("    TimeBasedFilter = " + std::to_string(dr_qos.time_based_filter.minimum_separation.sec), Verbosity::DEBUG);

    if ( options->deadline_interval > 0 ) {
      dr_qos.deadline.period.sec      = options->deadline_interval;
      dr_qos.deadline.period.nanosec  = 0;
    }
    logger.log_message("    DeadlinePeriod = " + std::to_string(dr_qos.deadline.period.sec), Verbosity::DEBUG);

    // options->history_depth < 0 means leave default value
    if ( options->history_depth > 0 )  {
      dr_qos.history.kind  = KEEP_LAST_HISTORY_QOS;
      dr_qos.history.depth = options->history_depth;
    }
    else if ( options->history_depth == 0 ) {
      dr_qos.history.kind  = KEEP_ALL_HISTORY_QOS;
    }
    logger.log_message("    History = " + QosUtils::to_string(dr_qos.history.kind), Verbosity::DEBUG);
    if (dr_qos.history.kind == KEEP_LAST_HISTORY_QOS){
      logger.log_message("    HistoryDepth = " + std::to_string(dr_qos.history.depth), Verbosity::DEBUG);
    }

    dr_qos.type_consistency = options->type_consistency;
    logger.log_message("    TypeConsistency * kind = "  + QosUtils::to_string(dr_qos.type_consistency.kind), Verbosity::DEBUG );
    logger.log_message("                    * ignore_sequence_bounds = "  + std::to_string(dr_qos.type_consistency.ignore_sequence_bounds ), Verbosity::DEBUG );
    logger.log_message("                    * ignore_string_bounds   = "  + std::to_string(dr_qos.type_consistency.ignore_string_bounds), Verbosity::DEBUG );
    logger.log_message("                    * ignore_member_names    = "  + std::to_string(dr_qos.type_consistency.ignore_member_names), Verbosity::DEBUG );
    logger.log_message("                    * prevent_type_widening  = "  + std::to_string(dr_qos.type_consistency.prevent_type_widening), Verbosity::DEBUG );
    logger.log_message("                    * force_type_validation  = "  + std::to_string(dr_qos.type_consistency.force_type_validation), Verbosity::DEBUG );

    printf("Create reader for topic: %s\n", options->topic_name );

    dr = sub->create_datareader(topic, dr_qos, NULL, LISTENER_STATUS_MASK_NONE);


    if (dr == NULL) {
      logger.log_message("failed to create datareader", Verbosity::ERROR);
      return false;
    }
    logger.log_message("Data Reader created", Verbosity::DEBUG);
    return true;
  }

  //-------------------------------------------------------------
  bool run_subscriber(TestOptions *options)
  {
    while ( ! all_done )  {
      ReturnCode_t     retval;
      DynamicDataSeq   samples;
      SampleInfoSeq    sample_infos;

      do {
        DynamicDataReader * ddr = dynamic_cast<DDS::DynamicDataReader*>(dr);
#if   defined(RTI_CONNEXT_DDS) || defined(OPENDDS) || defined(EPROSIMA_FAST_DDS) || defined(INTERCOM_DDS)
                    retval = ddr->take ( samples,
                            sample_infos,
                            LENGTH_UNLIMITED,
                            ANY_SAMPLE_STATE,
                            ANY_VIEW_STATE,
                            ANY_INSTANCE_STATE );
#elif defined(TWINOAKS_COREDX)
                    retval = ddr->take ( &samples,
                            &sample_infos,
                            LENGTH_UNLIMITED,
                            ANY_SAMPLE_STATE,
                            ANY_VIEW_STATE,
                            ANY_INSTANCE_STATE );
#endif
        if (retval == RETCODE_OK) {
          unsigned int i;
          for (i = 0; i < (unsigned int) samples.length(); i++)  {
#if   defined(RTI_CONNEXT_DDS)
                DynamicData        *sample      = &samples[i];
                SampleInfo         *sample_info = &sample_infos[i];
#elif defined(TWINOAKS_COREDX)
                DynamicData        *sample      = samples[i];
                SampleInfo         *sample_info = sample_infos[i];
#endif

            if (sample_info->valid_data)  {
              printf( "sample_received()\n" );
              PRINT_DATA(sample);
              if (CHECK_DATA(sample, options->xml_data_uri, options->json_data_uri)) {
                printf("Received sample is the same as loaded\n");
              } else {
                printf("Received sample is not the same as loaded\n");
              }
            }
          }

#if   defined(RTI_CONNEXT_DDS)
            ddr->return_loan(samples, sample_infos);
#elif defined(TWINOAKS_COREDX)
            ddr->return_loan( &samples, &sample_infos );
#endif
        }
      } while (retval == RETCODE_OK);

      usleep(100000);
    }

    return true;
  }


  //-------------------------------------------------------------
  bool run_publisher(TestOptions *options)
  {
    DDS::DynamicData * dd = CREATE_DATA(dt);
    if (dd == NULL) {
        logger.log_message("Error creating DynamicData", Verbosity::ERROR);
        return false;
    }

    if (INIT_DATA(dd, options->xml_data_uri, options->json_data_uri)
            != DDS_RETCODE_OK) {
        logger.log_message("Error initializing data", Verbosity::ERROR);
        return false;
    }

    while (!all_done) {
#if defined(RTI_CONNEXT_DDS)
      DynamicDataWriter *ddw = dynamic_cast<DynamicDataWriter *>(dw);
      ddw->write(*dd, HANDLE_NIL);
#else
      dw->write(*dd, HANDLE_NIL);
#endif
      if (options->print_writer_samples)
        {
          printf(" Wrote:\n" );
          PRINT_DATA( dd );
        }
      usleep(1000000);
    }

    return true;
  }

};

/*************************************************************/
int main( int argc, char * argv[] )
{
  install_sig_handlers();

  TestOptions options;
  bool parseResult = options.parse(argc, argv);
  if ( !parseResult  ) {
    exit(ERROR_PARSING_ARGUMENTS);
  }
  TestApplication testApp;
  if ( !testApp.initialize(&options) ) {
    exit(ERROR_INITIALIZING);
  }
  if ( !testApp.run(&options) ) {
    exit(ERROR_RUNNING);
  }

  printf("Done.\n");

  return 0;
}
