/****************************************************************
 * Use and redistribution is source and binary forms is permitted
 * subject to the OMG-DDS INTEROPERABILITY TESTING LICENSE found
 * at the following URL:
 *
 * https://github.com/omg-dds/dds-xtypes/blob/master/LICENSE.md
 */
/****************************************************************/
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <getopt.h>

#define CYCLONEDDS

#if defined(CYCLONEDDS)
#include "variant_eclipse_cyclone_dds.h"
#else
#error "Must define the DDS vendor"
#endif

#define ERROR_PARSING_ARGUMENTS 1
#define ERROR_INITIALIZING 2
#define ERROR_RUNNING 3

/*************************************************************/
int all_done = 0;
/*************************************************************/
void handle_sig(int sig)
{
    if (sig == SIGINT)
    {
        all_done = 1;
    }
}

/*************************************************************/
int install_sig_handlers()
{
    struct sigaction int_action;
    int_action.sa_handler = handle_sig;
    sigemptyset(&int_action.sa_mask);
    sigaddset(&int_action.sa_mask, SIGINT);
    int_action.sa_flags = 0;
    sigaction(SIGINT, &int_action, NULL);
    return 0;
}

typedef enum Verbosity_e
{
    ERROR = 1,
    DEBUG = 2,
} Verbosity;

static char* to_string_reliability(dds_reliability_kind_t reliability_value)
{
    if (reliability_value == DDS_RELIABILITY_BEST_EFFORT)
    {
        return "BEST_EFFORT";
    }
    else if (reliability_value == DDS_RELIABILITY_RELIABLE)
    {
        return "RELIABLE";
    }
    return "Error stringifying Reliability kind.";
}

static char* to_string_durability(dds_durability_kind_t durability_value)
{
    if (durability_value == DDS_DURABILITY_VOLATILE)
    {
        return "VOLATILE";
    }
    else if (durability_value == DDS_DURABILITY_TRANSIENT_LOCAL)
    {
        return "TRANSIENT_LOCAL";
    }
    else if (durability_value == DDS_DURABILITY_TRANSIENT)
    {
        return "TRANSIENT";
    }
    else if (durability_value == DDS_DURABILITY_PERSISTENT)
    {
        return "PERSISTENT";
    }
    return "Error stringifying Durability kind.";
}

static char* to_string_data_representation(dds_data_representation_id_t data_representation_value)
{
    if (data_representation_value == DDS_DATA_REPRESENTATION_XCDR1)
    {
        return "XCDR";
    }
    else if (data_representation_value == DDS_DATA_REPRESENTATION_XCDR2)
    {
        return "XCDR2";
    }
    return "Error stringifying DataRepresentation.";
}

static char* to_string_verbosity(Verbosity verbosity_value)
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

static char* to_string_ownership(dds_ownership_kind_t ownership_kind_value)
{
    if (ownership_kind_value == DDS_OWNERSHIP_SHARED)
    {
        return "SHARED";
    }
    else if (ownership_kind_value == DDS_OWNERSHIP_EXCLUSIVE)
    {
        return "EXCLUSIVE";
    }
    return "Error stringifying Ownership kind.";
}

static char* to_string_history(dds_history_kind_t history_kind_value)
{
    if (history_kind_value == DDS_HISTORY_KEEP_ALL)
    {
        return "KEEP_ALL";
    }
    else if (history_kind_value == DDS_HISTORY_KEEP_LAST)
    {
        return "KEEP_LAST";
    }
    return "Error stringifying History kind.";
}

static char* to_string_type_consistency(dds_type_consistency_kind_t consistency_kind_value)
{
    if (consistency_kind_value == DDS_TYPE_CONSISTENCY_DISALLOW_TYPE_COERCION)
    {
        return "DISALLOW_TYPE_COERCION";
    }
    else if (consistency_kind_value == DDS_TYPE_CONSISTENCY_ALLOW_TYPE_COERCION)
    {
        return "ALLOW_TYPE_COERCION";
    }
    return "Error stringifying TypeConsistency kind.";
}

/*************************************************************/
Verbosity verbosity = ERROR;

void log_message(Verbosity level_verbosity, const char *format, ...)
{
    if (level_verbosity <= verbosity)
    {
        va_list arglist;
        va_start(arglist, format);
        vprintf(format, arglist);
        va_end(arglist);
        printf("\n");
    }
}

/*************************************************************/

#define OPT_FORCE_TYPE_VALIDATION    0x1000
#define OPT_CHECK_MEMBER_NAMES       0x1001
#define OPT_CHECK_SEQUENCE_BOUNDS    0x1002
#define OPT_CHECK_STRING_BOUNDS      0x1003
#define OPT_PREVENT_TYPE_WIDENING    0x1004
#define OPT_ALLOW_TYPE_COERCION      0x1005
#define OPT_DISABLE_TYPE_INFO        0x1006
#define OPT_TYPE_OBJECT_VERSION      0x1007
#define OPT_PRINT_TYPEID             0x1008
#define OPT_TYPE_FOLDER              0x1009
#define OPT_TYPE_FILE                0x100A
#define OPT_DATA_FOLDER              0x100B
#define OPT_DATA_FILE                0x100C

static struct option long_opts[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"force-type-validation", required_argument, NULL, OPT_FORCE_TYPE_VALIDATION},
        {"ignore-member-names", required_argument, NULL, OPT_CHECK_MEMBER_NAMES},
        {"ignore-seq-bounds", required_argument, NULL, OPT_CHECK_SEQUENCE_BOUNDS},
        {"ignore-str-bounds", required_argument, NULL, OPT_CHECK_STRING_BOUNDS},
        {"prevent-type-widening", required_argument, NULL, OPT_PREVENT_TYPE_WIDENING},
        {"allow-type-coercion", required_argument, NULL, OPT_ALLOW_TYPE_COERCION},
        {"disable-type-info", no_argument, NULL, OPT_DISABLE_TYPE_INFO},
        {"type-object-version", required_argument, NULL, OPT_TYPE_OBJECT_VERSION},
        {"print-typeid", no_argument, NULL, OPT_PRINT_TYPEID},
        { "type-folder", required_argument, NULL, OPT_TYPE_FOLDER},
        { "type-file", required_argument, NULL, OPT_TYPE_FILE},
        { "data-folder", required_argument, NULL, OPT_DATA_FOLDER},
        { "data-file", required_argument, NULL, OPT_DATA_FILE},
        {NULL, 0, NULL, 0}};

/*************************************************************/
typedef struct TestOptions_s
{
    dds_domainid_t domain_id;
    dds_reliability_kind_t reliability_kind;
    dds_durability_kind_t durability_kind;
    dds_data_representation_id_t data_representation;
    int32_t history_depth;
    int32_t ownership_strength;

    dds_type_consistency_kind_t type_consistency_kind;
    bool ignore_sequence_bounds;
    bool ignore_string_bounds;
    bool ignore_member_names;
    bool prevent_type_widening;
    bool force_type_validation;

    char *topic_name;
    char *type_name;
    char *type_folder; /* folder containing type definitions */
    char *type_file;   /* local file with type definitions */

    char *data_folder; /* folder containing data samples */
    char *data_file;   /* local file with data sample */

    char *partition;

    bool publish;
    bool subscribe;

    int timebasedfilter_interval;
    int deadline_interval;

    bool print_writer_samples;

    bool disable_type_info;

    int type_object_version;
    bool print_typeid;
} TestOptions;

//-------------------------------------------------------------
TestOptions TestOptions_create()
{
    TestOptions options;

    options.domain_id = 0;
    options.reliability_kind = DDS_RELIABILITY_RELIABLE;
    options.durability_kind = DDS_DURABILITY_VOLATILE;
    options.data_representation = DDS_DATA_REPRESENTATION_XCDR1;
    options.history_depth = -1;      /* means default */
    options.ownership_strength = -1; /* means shared */

    options.force_type_validation = DDS_BOOLEAN_FALSE;
    options.ignore_member_names = DDS_BOOLEAN_TRUE;
    options.ignore_sequence_bounds = DDS_BOOLEAN_TRUE;
    options.ignore_string_bounds = DDS_BOOLEAN_TRUE;
    options.prevent_type_widening = DDS_BOOLEAN_FALSE;
    options.type_consistency_kind = DDS_TYPE_CONSISTENCY_ALLOW_TYPE_COERCION;
#if defined(RTI_CONNEXT_DDS)
    type_consistency.ignore_enum_literal_names =
        TypeConsistency_get_default().ignore_enum_literal_names;
#endif

    options.disable_type_info = false;

    options.topic_name = NULL;
    options.type_name = NULL;
    options.partition = NULL;
    options.type_folder = NULL;
    options.type_file = NULL;
    options.data_folder = NULL;
    options.data_file = NULL;

    options.publish = false;
    options.subscribe = false;

    options.timebasedfilter_interval = 0; /* off */
    options.deadline_interval = 0;        /* off */

    options.print_writer_samples = false;

    options.disable_type_info = false;
    options.type_object_version = 2;
    options.print_typeid = false;

    return options;
}

//-------------------------------------------------------------
void TestOptions_free(TestOptions *options)
{
    if (options->topic_name)
        free(options->topic_name);
    if (options->type_name)
        free(options->type_name);
    if (options->type_folder)
        free(options->type_folder);
    if (options->type_file)
        free(options->type_file);
    if (options->data_folder)
        free(options->data_folder);
    if (options->data_file)
        free(options->data_file);
    if (options->partition)
        free(options->partition);
}

//-------------------------------------------------------------
void print_usage(const char *prog)
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
    printf("   --type-folder <folder>: folder containing type definitions (eg: types)\n");
    printf("   --type-file <file>    : type definition file name without extension\n");
    printf("   --data-folder <folder>: folder containing data samples (eg: data)\n");
    printf("   --data-file <file>    : data sample file name without extension\n");
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
    printf("   --disable-type-info: disable sending the type info for type\n");
    printf("                        assignability\n");
    printf("   --type-object-version [1|2]: set the Type Object version to use.\n");
    printf("                                Default: 2.\n");
    printf("   --print-typeid: print typeid (TypeObjectV1) or equivalence hash ");
    printf("                   (TypeObjectV2)\n");
    printf("                          print_typeid\n");
    printf("   -v [e|d]        : set log message verbosity [e: ERROR, d: DEBUG]\n");
}

//-------------------------------------------------------------
bool TestOptions_validate(TestOptions *options)
{
    if (options->topic_name == NULL)
    {
        log_message(ERROR, "topic name unspecified [-t], using \"test\" by default");
        return false;
    }
    if (options->type_name == NULL)
    {
        log_message(ERROR, "please specify type name [-y]");
        return false;
    }
    if ((!options->publish) && (!options->subscribe))
    {
        log_message(ERROR, "please specify publish [-P] or subscribe [-S]");
        return false;
    }
    if (options->publish && options->subscribe)
    {
        log_message(ERROR, "please specify only one of: publish [-P] or subscribe [-S]");
        return false;
    }
#if 0 /* allow publishing an empty, unpopulated sample */
    if ( options->data_folder == NULL && options->data_file == NULL ) {
      log_message(ERROR, "please provide the data via --data-folder or --data-file");
      return false;
    }
#endif
    if (options->type_folder == NULL && options->type_file == NULL)
    {
        log_message(ERROR, "please provide the types via --type-folder or --type-file");
        return false;
    }
    if (   options->type_consistency_kind != DDS_TYPE_CONSISTENCY_ALLOW_TYPE_COERCION 
        && options->ignore_sequence_bounds == DDS_BOOLEAN_TRUE 
        && options->ignore_string_bounds == DDS_BOOLEAN_TRUE 
        && options->ignore_member_names == DDS_BOOLEAN_TRUE 
        && options->prevent_type_widening == DDS_BOOLEAN_TRUE)
    {
        log_message(ERROR,
                            "warning: prevent_type_widening, ignore_sequence_bounds, ",
                           "ignore_string_bounds, and ignore_member_names only apply when the "
                           "type consistency kind is ALLOW_TYPE_COERCION"
                           );
    }
    return true;
}

//-------------------------------------------------------------
bool TestOptions_parse(TestOptions *options, int argc, char *argv[])
{
    int opt;
    bool parse_ok = true;
    // double d;
    while ((opt = getopt_long(argc, argv,
                              "hbrd:D:f:i:k:p:s:x:t:v:wy:PS",
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
                    verbosity = DEBUG;
                    break;
                }
                case 'e':
                {
                    verbosity = ERROR;
                    break;
                }
                default:
                {
                    log_message(ERROR, "unrecognized value for verbosity %c", optarg[0]);
                    parse_ok = false;
                }
                }
            }
            break;
        }
        case 'w':
        {
            options->print_writer_samples = true;
            break;
        }
        case 'b':
        {
            options->reliability_kind = DDS_RELIABILITY_BEST_EFFORT;
            break;
        }
        case 'd':
        {
            int converted_param = sscanf(optarg, "%d", &options->domain_id);
            if (converted_param == 0)
            {
                log_message(ERROR, "unrecognized value for domain_id %c", optarg[0]);
                parse_ok = false;
            }
            else if (options->domain_id < 0)
            {
                log_message(ERROR, "incorrect value for domain_id %u", options->domain_id);
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
                    options->durability_kind = DDS_DURABILITY_VOLATILE;
                    break;
                }
                case 'l':
                {
                    options->durability_kind = DDS_DURABILITY_TRANSIENT_LOCAL;
                    break;
                }
                case 't':
                {
                    options->durability_kind = DDS_DURABILITY_TRANSIENT;
                    break;
                }
                case 'p':
                {
                    options->durability_kind = DDS_DURABILITY_PERSISTENT;
                    break;
                }
                default:
                {
                    log_message(ERROR, "unrecognized value for durability %c", optarg[0]),
                    parse_ok = false;
                }
                }
            }
            break;
        }
        case 'i':
        {
            int converted_param = sscanf(optarg, "%d", &options->timebasedfilter_interval);
            if (converted_param == 0)
            {
                log_message(ERROR, "unrecognized value for timebasedfilter_interval %c", optarg[0]);
                parse_ok = false;
            }
            else if (options->timebasedfilter_interval < 0)
            {
                log_message(ERROR, "incorrect value for timebasedfilter_interval %d", options->timebasedfilter_interval);
                parse_ok = false;
            }
            break;
        }
        case 'f':
        {
            int converted_param = sscanf(optarg, "%d", &options->deadline_interval);
            if (converted_param == 0)
            {
                log_message(ERROR, "unrecognized value for deadline_interval %c", optarg[0]);
                parse_ok = false;
            }
            else if (options->deadline_interval < 0)
            {
                log_message(ERROR, "incorrect value for deadline_interval %d", options->deadline_interval);
                parse_ok = false;
            }
            break;
        }
        case 'k':
        {
            int converted_param = sscanf(optarg, "%d", &options->history_depth);
            if (converted_param == 0)
            {
                log_message(ERROR, "unrecognized value for history_depth %c", optarg[0]);
                parse_ok = false;
            }
            else if (options->history_depth < 0)
            {
                log_message(ERROR, "incorrect value for history_depth %d", options->history_depth);
                parse_ok = false;
            }
            break;
        }
        case 'p':
        {
            options->partition = strdup(optarg);
            break;
        }
        case 'r':
        {
            options->reliability_kind = DDS_RELIABILITY_RELIABLE;
            break;
        }
        case 's':
        {
            int converted_param = sscanf(optarg, "%d", &options->ownership_strength);
            if (converted_param == 0)
            {
                log_message(ERROR, "unrecognized value for ownership_strength %c", optarg[0]);
                parse_ok = false;
            }
            else if (options->ownership_strength < -1)
            {
                log_message(ERROR, "incorrect value for ownership_strength ", options->ownership_strength);
                parse_ok = false;
            }
            break;
        }
        case 't':
        {
            options->topic_name = strdup(optarg);
            break;
        }
        case 'P':
        {
            //printf("Publishing samples: %c\n", optarg[0]);
            options->publish = true;
            break;
        }
        case 'S':
        {
            //printf("Subscribing samples: %c\n", optarg[0]);
            options->subscribe = true;
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
                    options->data_representation = DDS_DATA_REPRESENTATION_XCDR1;
                    break;
                }
                case '2':
                {
                    options->data_representation = DDS_DATA_REPRESENTATION_XCDR2;
                    break;
                }
                default:
                {
                    log_message(ERROR, "unrecognized value for data representation %c", optarg[0]);
                    parse_ok = false;
                }
                }
            }
            break;
        }
        case 'y':
        {
            options->type_name = strdup(optarg);
            break;
        }
        case OPT_TYPE_FOLDER:
        {
            options->type_folder = strdup(optarg);
            break;
        }
        case OPT_TYPE_FILE:
        {
            options->type_file = strdup(optarg);
            break;
        }
        case OPT_DATA_FOLDER:
        {
            options->data_folder = strdup(optarg);
            break;
        }
        case OPT_DATA_FILE:
        {
            options->data_file = strdup(optarg);
            break;
        }
        case OPT_FORCE_TYPE_VALIDATION:
            if (optarg[0] != '\0')
            {
                switch (optarg[0])
                {
                case 't':
                    options->force_type_validation = DDS_BOOLEAN_TRUE;
                    break;
                case 'f':
                    options->force_type_validation = DDS_BOOLEAN_FALSE;
                    break;
                case 'd':
                    options->force_type_validation = DDS_BOOLEAN_FALSE;
                    break;
                default:
                    log_message(ERROR, "unrecognized value for type_consistency."
                                "force_type_validation %c",
                                    optarg[0]);
                    parse_ok = false;
                    break;
                }
            }
            break;

        case OPT_CHECK_MEMBER_NAMES:
            if (optarg[0] != '\0')
            {
                switch (optarg[0])
                {
                case 't':
                    options->ignore_member_names = DDS_BOOLEAN_TRUE;
                    break;
                case 'f':
                    options->ignore_member_names = DDS_BOOLEAN_FALSE;
                    break;
                case 'd':
                    options->ignore_member_names = DDS_BOOLEAN_FALSE;
                    break;
                default:
                    log_message(ERROR, "unrecognized value for type_consistency."
                                       "ignore_member_names %c",
                                optarg[0]);
                    parse_ok = false;
                    break;
                }
            }
            break;

        case OPT_CHECK_SEQUENCE_BOUNDS:
            if (optarg[0] != '\0')
            {
                switch (optarg[0])
                {
                case 't':
                    options->ignore_sequence_bounds = DDS_BOOLEAN_TRUE;
                    break;
                case 'f':
                    options->ignore_sequence_bounds = DDS_BOOLEAN_FALSE;
                    break;
                case 'd':
                    options->ignore_sequence_bounds = DDS_BOOLEAN_TRUE;
                    break;
                default:
                    log_message(ERROR, "unrecognized value for type_consistency."
                                       "ignore_sequence_bounds %c",
                                           optarg[0]);
                    parse_ok = false;
                    break;
                }
            }
            break;

        case OPT_CHECK_STRING_BOUNDS:
            if (optarg[0] != '\0')
            {
                switch (optarg[0])
                {
                case 't':
                    options->ignore_string_bounds = DDS_BOOLEAN_TRUE;
                    break;
                case 'f':
                    options->ignore_string_bounds = DDS_BOOLEAN_FALSE;
                    break;
                case 'd':
                    options->ignore_string_bounds = DDS_BOOLEAN_TRUE;
                    break;
                default:
                    log_message(ERROR, "unrecognized value for type_consistency."
                                       "ignore_string_bounds %c",
                                           optarg[0]);
                    parse_ok = false;
                    break;
                }
            }
            break;

        case OPT_PREVENT_TYPE_WIDENING:
            if (optarg[0] != '\0')
            {
                switch (optarg[0])
                {
                case 't':
                    options->prevent_type_widening = DDS_BOOLEAN_TRUE;
                    break;
                case 'f':
                    options->prevent_type_widening = DDS_BOOLEAN_FALSE;
                    break;
                case 'd':
                    options->prevent_type_widening = DDS_BOOLEAN_FALSE;
                    break;
                default:
                    log_message(ERROR, "unrecognized value for type_consistency."
                                       "prevent_type_widening %c",
                                           optarg[0]);
                    parse_ok = false;
                    break;
                }
            }
            break;

        case OPT_ALLOW_TYPE_COERCION:
            if (optarg[0] != '\0')
            {
                switch (optarg[0])
                {
                case 't':
                    options->type_consistency_kind = DDS_TYPE_CONSISTENCY_ALLOW_TYPE_COERCION;
                    break;
                case 'f':
                    options->type_consistency_kind = DDS_TYPE_CONSISTENCY_DISALLOW_TYPE_COERCION;
                    break;
                case 'd':
                    options->type_consistency_kind = DDS_TYPE_CONSISTENCY_ALLOW_TYPE_COERCION;
                    break;
                default:
                    log_message(ERROR, "unrecognized value for type_consistency."
                                       "kind %c", optarg[0]);
                    parse_ok = false;
                    break;
                }
            }
            break;

        case OPT_DISABLE_TYPE_INFO:
            options->disable_type_info = true;
            break;

        case OPT_TYPE_OBJECT_VERSION:
            if (optarg[0] != '\0')
            {
                switch (optarg[0])
                {
                case '1':
                    options->type_object_version = 1;
                    break;
                case '2':
                    options->type_object_version = 2;
                    break;
                default:
                    log_message(ERROR, "unrecognized value for type object version %c", optarg[0]);
                    parse_ok = false;
                    break;
                }
            }
            break;

        case OPT_PRINT_TYPEID:
            options->print_typeid = true;
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

    if (parse_ok)
    {
        if (options->topic_name == NULL)
        {
            options->topic_name = strdup("test"); /* default.. */
        }
        parse_ok = TestOptions_validate(options);
    }
    if (!parse_ok)
    {
        print_usage(argv[0]);
    }
    else
    {
        char* app_kind = options->publish ? "publisher" : "subscriber";
        log_message(DEBUG, "Test Options: ");
        log_message(DEBUG, "    This application is a %s", app_kind);
        log_message(DEBUG, "    DomainId = %u", options->domain_id);
        log_message(DEBUG, "    ReliabilityKind = %s", to_string_reliability(options->reliability_kind));
        log_message(DEBUG, "    DurabilityKind = %s", to_string_durability(options->durability_kind));
        log_message(DEBUG, "    DataRepresentation = %s", to_string_data_representation(options->data_representation));
        log_message(DEBUG, "    HistoryDepth = %d", options->history_depth);
        log_message(DEBUG, "    OwnershipStrength = %d", options->ownership_strength);
        log_message(DEBUG, "    TimeBasedFilterInterval = %d", options->timebasedfilter_interval);
        log_message(DEBUG, "    DeadlineInterval = %d", options->deadline_interval);
        log_message(DEBUG, "    Type consistency kind = %s", to_string_type_consistency(options->type_consistency_kind));
        log_message(DEBUG, "    Disable type info = %d", options->disable_type_info);
        log_message(DEBUG, "    TypeObject version = %d", options->type_object_version);
        log_message(DEBUG, "    Print Type ID = %d", options->print_typeid);
        log_message(DEBUG, "    Verbosity = %s", to_string_verbosity(verbosity));
        if (!options->publish)
        {
            log_message(DEBUG, "    Force type validation = %d", options->force_type_validation);
            log_message(DEBUG, "    Ignore member names = %d", options->ignore_member_names);
            log_message(DEBUG, "    Ignore sequence bounds = %d", options->ignore_sequence_bounds);
            log_message(DEBUG, "    Ignore string bounds = %d", options->ignore_string_bounds);
            log_message(DEBUG, "    Prevent type widening = %d", options->prevent_type_widening);
        }

        if (options->topic_name != NULL)
        {
            log_message(DEBUG, "    Topic = %s", options->topic_name);
        }
        if (options->type_name != NULL)
        {
            log_message(DEBUG, "    Type  = %s", options->type_name);
        }
        if (options->type_folder != NULL)
        {
            log_message(DEBUG, "    Type Folder = %s",  options->type_folder);
        }
        if (options->type_file != NULL)
        {
            log_message(DEBUG, "    Type File = %s",  options->type_file);
        }
        if (options->data_folder != NULL)
        {
            log_message(DEBUG, "    Data Folder = %s",  options->data_folder);
        }
        if (options->data_file != NULL)
        {
            log_message(DEBUG, "    Data File = %s",  options->data_file);
        }
        if (options->partition != NULL)
        {
            log_message(DEBUG, "    Partition = ", options->partition);
        }
    }
    return parse_ok;
}

char* helper_get_topic_name(dds_entity_t topic)
{
    char temp;
    dds_return_t buf_len = dds_get_name(topic, &temp, 1) + 1;
    char* topic_name = malloc(buf_len);
    dds_get_name(topic, topic_name, buf_len);
    return topic_name;
}
char* helper_get_type_name(dds_entity_t topic)
{
    char temp;
    dds_return_t buf_len = dds_get_type_name(topic, &temp, 1) + 1;
    char* type_name = malloc(buf_len);
    dds_get_type_name(topic, type_name, buf_len);
    return type_name;
}

/*************************************************************/
void on_inconsistent_topic(dds_entity_t topic, const dds_inconsistent_topic_status_t status, void* args)
{
    char* topic_name = helper_get_topic_name(topic);
    char* type_name = helper_get_type_name(topic);
    printf("%s() topic: '%s'  type: '%s'\n", __FUNCTION__, topic_name, type_name);
    free(topic_name);
    free(type_name);
}

void on_offered_incompatible_qos(dds_entity_t dw, const dds_offered_incompatible_qos_status_t status, void* args)
{
    if (status.last_policy_id == DDS_TYPE_CONSISTENCY_ENFORCEMENT_QOS_POLICY_ID){
        dds_inconsistent_topic_status_t topic_status;
        topic_status.total_count = 0;
        topic_status.total_count_change = 0;
        on_inconsistent_topic(dds_get_topic(dw), topic_status, args);
        return;
    }
    dds_entity_t topic = dds_get_topic(dw);
    char* topic_name = helper_get_topic_name(topic);
    char* type_name = helper_get_type_name(topic);
    const char *policy_name = NULL;
    policy_name = get_qos_policy_name(status.last_policy_id);
    printf("%s() topic: '%s'  type: '%s' : %d (%s)\n", __FUNCTION__,
           topic_name, type_name,
           status.last_policy_id,
           policy_name);
    free(topic_name);
    free(type_name);
}

void on_publication_matched(dds_entity_t dw, const dds_publication_matched_status_t status, void* args)
{
    dds_entity_t topic = dds_get_topic(dw);
    char* topic_name = helper_get_topic_name(topic);
    char* type_name = helper_get_type_name(topic);
    printf("%s() topic: '%s'  type: '%s' : matched readers %d (change = %d)\n", __FUNCTION__,
           topic_name, type_name, status.current_count, status.current_count_change);
    free(topic_name);
    free(type_name);
}

void on_offered_deadline_missed(dds_entity_t dw, const dds_offered_deadline_missed_status_t status, void* args)
{
    dds_entity_t topic = dds_get_topic(dw);
    char* topic_name = helper_get_topic_name(topic);
    char* type_name = helper_get_type_name(topic);
    printf("%s() topic: '%s'  type: '%s' : (total = %d, change = %d)\n", __FUNCTION__,
           topic_name, type_name, status.total_count, status.total_count_change);
    free(topic_name);
    free(type_name);
}

void on_liveliness_lost(dds_entity_t dw, const dds_liveliness_lost_status_t status, void* args)
{
    dds_entity_t topic = dds_get_topic(dw);
    char* topic_name = helper_get_topic_name(topic);
    char* type_name = helper_get_type_name(topic);
    printf("%s() topic: '%s'  type: '%s' : (total = %d, change = %d)\n", __FUNCTION__,
           topic_name, type_name, status.total_count, status.total_count_change);
    free(topic_name);
    free(type_name);
}

void on_requested_incompatible_qos(dds_entity_t dr, const dds_requested_incompatible_qos_status_t status, void* args)
{
    if (status.last_policy_id == DDS_TYPE_CONSISTENCY_ENFORCEMENT_QOS_POLICY_ID){
        dds_inconsistent_topic_status_t topic_status;
        topic_status.total_count = 0;
        topic_status.total_count_change = 0;
        on_inconsistent_topic(dds_get_topic(dr), topic_status, args);
        return;
    }
    dds_entity_t topic = dds_get_topic(dr);
    char* topic_name = helper_get_topic_name(topic);
    char* type_name = helper_get_type_name(topic);
    const char *policy_name = NULL;
    policy_name = get_qos_policy_name(status.last_policy_id);
    printf("%s() topic: '%s'  type: '%s' : %d (%s)\n", __FUNCTION__,
           topic_name, type_name, status.last_policy_id,
           policy_name);
    free(topic_name);
    free(type_name);
}

void on_subscription_matched(dds_entity_t dr, const dds_subscription_matched_status_t status, void* args)
{
    dds_entity_t topic = dds_get_topic(dr);
    char* topic_name = helper_get_topic_name(topic);
    char* type_name = helper_get_type_name(topic);
    printf("%s() topic: '%s'  type: '%s' : matched writers %d (change = %d)\n", __FUNCTION__,
           topic_name, type_name, status.current_count, status.current_count_change);
    free(topic_name);
    free(type_name);
}

void on_requested_deadline_missed(dds_entity_t dr, const dds_requested_deadline_missed_status_t status, void* args)
{
    dds_entity_t topic = dds_get_topic(dr);
    char* topic_name = helper_get_topic_name(topic);
    char* type_name = helper_get_type_name(topic);
    printf("%s() topic: '%s'  type: '%s' : (total = %d, change = %d)\n", __FUNCTION__,
           topic_name, type_name, status.total_count, status.total_count_change);
    free(topic_name);
    free(type_name);
}

void on_liveliness_changed(dds_entity_t dr, const dds_liveliness_changed_status_t status, void* args)
{
    dds_entity_t topic = dds_get_topic(dr);
    char* topic_name = helper_get_topic_name(topic);
    char* type_name = helper_get_type_name(topic);
    printf("%s() topic: '%s'  type: '%s' : (alive = %d, not_alive = %d)\n", __FUNCTION__,
           topic_name, type_name, status.alive_count, status.not_alive_count);
    free(topic_name);
    free(type_name);
}

void on_sample_rejected(dds_entity_t , const dds_sample_rejected_status_t status, void* args) {}
void on_data_available(dds_entity_t ) {}
void on_sample_lost(dds_entity_t , const dds_sample_lost_status_t status, void* args) {}
void on_data_on_readers(dds_entity_t ) {}

/*************************************************************/
typedef struct TestApplication_s
{
    dds_listener_t* dp_listener;

    dds_entity_t dp;
    dds_entity_t pub;
    dds_entity_t sub;
    dds_entity_t topic;
    dds_entity_t dr;
    dds_entity_t dw;

    struct dyntypelib *dtl;
    struct dyntypelib_error err;
    struct dyntype* dt;
    dds_topic_descriptor_t* td;

} TestApplication;

bool TestApplication_init_publisher(TestApplication *app, TestOptions *options);
bool TestApplication_init_subscriber(TestApplication *app, TestOptions *options);
bool TestApplication_run_publisher(TestApplication *app, TestOptions *options);
bool TestApplication_run_subscriber(TestApplication *app, TestOptions *options);

//-------------------------------------------------------------
TestApplication TestApplication_create()
{
    TestApplication app;
    app.dp_listener = NULL;
    app.dp = 0;
    app.pub = 0;
    app.sub = 0;
    app.topic = 0;
    app.dr = 0;
    app.dw = 0;
    app.dtl = NULL;
    app.dt = NULL;
    app.td = NULL;

    return app;
}

//-------------------------------------------------------------
void TestApplication_free(TestApplication *app)
{
    if (app->dp_listener)
        dds_delete_listener(app->dp_listener);
    if (app->dt)
        dtl_free(app->dtl);
    if (app->dp)
        dds_delete(app->dp);
}

//-------------------------------------------------------------
bool TestApplication_initialize(TestApplication *app, TestOptions *options)
{
    app->dp_listener = dds_create_listener(NULL);
    dds_lset_inconsistent_topic(app->dp_listener, on_inconsistent_topic);
    dds_lset_offered_incompatible_qos(app->dp_listener, on_offered_incompatible_qos);
    dds_lset_publication_matched(app->dp_listener, on_publication_matched);
    dds_lset_offered_deadline_missed(app->dp_listener, on_offered_deadline_missed);
    dds_lset_liveliness_lost(app->dp_listener, on_liveliness_lost);
    dds_lset_requested_incompatible_qos(app->dp_listener, on_requested_incompatible_qos);
    dds_lset_subscription_matched(app->dp_listener, on_subscription_matched);
    dds_lset_requested_deadline_missed(app->dp_listener, on_requested_deadline_missed);
    dds_lset_liveliness_changed(app->dp_listener, on_liveliness_changed);
    dds_lset_sample_rejected(app->dp_listener, on_sample_rejected);
    dds_lset_data_available(app->dp_listener, on_data_available);
    dds_lset_sample_lost(app->dp_listener,on_sample_lost);
    dds_lset_data_on_readers(app->dp_listener, on_data_on_readers);


    dds_qos_t* dp_qos = dds_create_qos();
    dds_return_t retcode;

    if (options->disable_type_info)
    {
        //disable_type_information(dp_qos);
    }

    //set_type_object_version(dp_qos, options->type_object_version);

    app->dp = dds_create_participant(options->domain_id, dp_qos, app->dp_listener);
    if (app->dp == 0)
    {
        log_message(ERROR, "failed to create participant (missing license?).");
        return false;
    }
    log_message(DEBUG, "Participant created");
    
    app->dtl = dtl_new(app->dp);
    dtl_set_print_types (app->dtl, true);
    char* types_uri = NULL;
    if (options->type_folder != NULL && options->type_file != NULL)
    {
        types_uri = malloc(strlen(options->type_folder) + strlen("/xml/") + strlen(options->type_file) + strlen(".xml") + 1);
        sprintf(types_uri, "%s%s%s%s", options->type_folder, "/xml/", options->type_file, ".xml");
    }
    if (dtl_add_xml_type_library (app->dtl, types_uri, &app->err) != DDS_RETCODE_OK)
    {
        log_message(ERROR, "failed to create type");
        log_message(DEBUG, "errmsg: %s", app->err.errmsg);
        return false;
    }

    app->dt = dtl_lookup_typename(app->dtl, options->type_name);
    if (app->dt == NULL)
    {
        log_message(ERROR, "failed to create type");
        return false;
    }

    if (options->print_typeid)
    {
        print_typeid(app->dt, app->dtl, options->type_object_version);
    }

    //if ((retcode = dds_dynamic_type_register(app->dt->dtype, &app->dt->typeinfo)) != DDS_RETCODE_OK)
    //{
    //    log_message(ERROR, "failed to register type: %s", dds_strretcode(retcode));
    //    return false;
    //}

    printf("Create topic: %s\n", options->topic_name);

    if (dds_create_topic_descriptor(DDS_FIND_SCOPE_LOCAL_DOMAIN, app->dp, app->dt->typeinfo, 0, &app->td) != DDS_RETCODE_OK)
    {
        log_message(ERROR, "failed to create topic descriptor");
        return false;
    }

    app->topic = dds_create_topic(app->dp, app->td, options->topic_name, NULL, NULL);
    if (app->topic < 0)
    {
        log_message(ERROR, "failed to create topic");
        return false;
    }

    if (options->publish)
    {
        return TestApplication_init_publisher(app, options);
    }
    else
    {
        return TestApplication_init_subscriber(app, options);
    }
}

//-------------------------------------------------------------
bool TestApplication_run(TestApplication *app, TestOptions *options)
{
    if (app->pub != 0)
    {
        return TestApplication_run_publisher(app, options);
    }
    else if (app->sub != 0)
    {
        return TestApplication_run_subscriber(app, options);
    }
    return false;
}

//-------------------------------------------------------------
bool TestApplication_init_publisher(TestApplication *app, TestOptions *options)
{
    log_message(DEBUG, "Initializing Publisher");
    dds_return_t retcode;
    dds_qos_t* pub_qos;
    dds_qos_t* dw_qos;

    pub_qos = dds_create_qos();
    if (options->partition != NULL)
    {
        dds_qset_partition1(pub_qos, options->partition);
    }

    app->pub = dds_create_publisher(app->dp, pub_qos, NULL);
    if (app->pub == 0)
    {
        log_message(ERROR, "failed to create publisher");
        return false;
    }

    log_message(DEBUG, "Publisher created");
    log_message(DEBUG, "Data Writer QoS:");

    dw_qos = dds_create_qos();
    dds_qset_reliability(dw_qos, options->reliability_kind, 0);
    {
        dds_reliability_kind_t reliability;
        dds_qget_reliability(dw_qos, &reliability, NULL);
        log_message(DEBUG, "    Reliability = %s", to_string_reliability(reliability));
    }

    dds_qset_durability(dw_qos, options->durability_kind);
    {
        dds_durability_kind_t durability;
        dds_qget_durability(dw_qos, &durability);
        log_message(DEBUG, "    Durability = %s", to_string_durability(durability));
    }

    dds_qset_data_representation(dw_qos, 1, &options->data_representation);
    {
        dds_data_representation_id_t* data_representation;
        uint32_t length;
        dds_qget_data_representation(dw_qos, &length, &data_representation);
        log_message(DEBUG, "    Data_Representation = %s", to_string_data_representation(data_representation[0]));
    }

    if (options->ownership_strength != -1)
    {
        dds_qset_ownership(dw_qos, DDS_OWNERSHIP_EXCLUSIVE);
        dds_qset_ownership_strength(dw_qos, options->ownership_strength);
    }

    if (options->ownership_strength == -1)
    {
        dds_qset_ownership(dw_qos, DDS_OWNERSHIP_SHARED);
    }
    {
        dds_ownership_kind_t ownership;
        dds_qget_ownership(dw_qos, &ownership);
        log_message(DEBUG, "    Ownership = %s", to_string_ownership(ownership));
    }
    if (options->ownership_strength != -1)
    {
        int32_t ownership_strength;
        dds_qget_ownership_strength(dw_qos, &ownership_strength);
        log_message(DEBUG, "    OwnershipStrength = %d", ownership_strength);
    }

    if (options->deadline_interval > 0)
    {
        dds_qset_deadline(dw_qos, DDS_SECS(options->deadline_interval));
    }
    {
        dds_duration_t deadline;
        dds_qget_deadline(dw_qos, &deadline);
        log_message(DEBUG, "    DeadlinePeriod = %d", deadline / DDS_SECS(1));
    }

    // options->history_depth < 0 means leave default value
    if (options->history_depth > 0)
    {
        dds_qset_history(dw_qos, DDS_HISTORY_KEEP_LAST, options->history_depth);
    }
    else if (options->history_depth == 0)
    {
        dds_qset_history(dw_qos, DDS_HISTORY_KEEP_ALL, 0);
    }
    {
        dds_history_kind_t history_kind;
        int32_t history_depth;
        dds_qget_history(dw_qos, &history_kind, &history_depth);
        log_message(DEBUG, "    History = %s", to_string_history(history_kind));
        if (history_kind == DDS_HISTORY_KEEP_LAST)
        {
            log_message(DEBUG, "    HistoryDepth = %d", history_depth);
        }
    }

    dds_qset_type_consistency(
        dw_qos, 
        options->type_consistency_kind,
        options->ignore_sequence_bounds,
        options->ignore_string_bounds,
        options->ignore_member_names,
        options->prevent_type_widening,
        options->force_type_validation);
    {
        dds_type_consistency_kind_t type_consistency_kind;
        bool ignore_sequence_bounds;
        bool ignore_string_bounds;
        bool ignore_member_names;
        bool prevent_type_widening;
        bool force_type_validation;

        dds_qget_type_consistency(
            dw_qos,
            &type_consistency_kind,
            &ignore_sequence_bounds,
            &ignore_string_bounds,
            &ignore_member_names,
            &prevent_type_widening,
            &force_type_validation);
        log_message(DEBUG, "    TypeConsistency * kind = %s", to_string_type_consistency(type_consistency_kind));
        log_message(DEBUG, "                    * ignore_sequence_bounds = %d", ignore_sequence_bounds);
        log_message(DEBUG, "                    * ignore_string_bounds   = %d", ignore_string_bounds);
        log_message(DEBUG, "                    * ignore_member_names    = %d", ignore_member_names);
        log_message(DEBUG, "                    * prevent_type_widening  = %d", prevent_type_widening);
        log_message(DEBUG, "                    * force_type_validation  = %d", force_type_validation);
    }    

    printf("Create writer for topic: %s type: %s\n", options->topic_name, options->type_name);

    app->dw = dds_create_writer(app->pub, app->topic, dw_qos, NULL);

    if (app->dw < 0)
    {
        log_message(ERROR, "failed to create datawriter");
        return false;
    }

    size_t align, size;
    build_typecache_to (app->dtl->typecache, &app->dt->typeobj->_u.complete, &align, &size);

    log_message(DEBUG, "Data Writer created");

    return true;
}

//-------------------------------------------------------------
bool TestApplication_init_subscriber(TestApplication *app, TestOptions *options)
{
    dds_return_t retcode;
    dds_qos_t* sub_qos;
    dds_qos_t* dr_qos;

    sub_qos = dds_create_qos();
    if (options->partition != NULL)
    {
        dds_qset_partition1(sub_qos, options->partition);
    }

    app->sub = dds_create_subscriber(app->dp, sub_qos, NULL);
    if (app->sub == 0)
    {
        log_message(ERROR, "failed to create subscriber");
        return false;
    }

    log_message(DEBUG, "Subscriber created");
    log_message(DEBUG, "Data Reader QoS:");

    dr_qos = dds_create_qos();

    dds_qset_reliability(dr_qos, options->reliability_kind, 0);
    {
        dds_reliability_kind_t reliability;
        dds_qget_reliability(dr_qos, &reliability, NULL);
        log_message(DEBUG, "    Reliability = ", to_string_reliability(reliability));
    }

    dds_qset_durability(dr_qos, options->durability_kind);
    {
        dds_durability_kind_t durability;
        dds_qget_durability(dr_qos, &durability);
        log_message(DEBUG, "    Durability = %s", to_string_durability(durability));
    }

    dds_qset_data_representation(dr_qos, 1, &options->data_representation);
    {
        dds_data_representation_id_t* data_representation;
        uint32_t length;
        dds_qget_data_representation(dr_qos, &length, &data_representation);
        log_message(DEBUG, "    Data_Representation = %s", to_string_data_representation(data_representation[0]));
    }

    if (options->ownership_strength != -1)
    {
        dds_qset_ownership(dr_qos, DDS_OWNERSHIP_EXCLUSIVE);
        dds_qset_ownership_strength(dr_qos, options->ownership_strength);
    } else
    {
        dds_qset_ownership(dr_qos, DDS_OWNERSHIP_SHARED);
    }
    {
        dds_ownership_kind_t ownership;
        dds_qget_ownership(dr_qos, &ownership);
        log_message(DEBUG, "    Ownership = %s", to_string_ownership(ownership));
    }

    if (options->timebasedfilter_interval > 0)
    {
        dds_qset_time_based_filter(dr_qos, DDS_SECS(options->timebasedfilter_interval));
    }
    {
        dds_duration_t time_based_filter;
        dds_qget_time_based_filter(dr_qos, &time_based_filter);
        log_message(DEBUG, "    TimeBasedFilter = %d", time_based_filter / DDS_SECS(1));
    }

    if (options->deadline_interval > 0)
    {
        dds_qset_deadline(dr_qos, DDS_SECS(options->deadline_interval));
    }
    {
        dds_duration_t deadline;
        dds_qget_deadline(dr_qos, &deadline);
        log_message(DEBUG, "    DeadlinePeriod = %d", deadline / DDS_SECS(1));
    }

    // options->history_depth < 0 means leave default value
    if (options->history_depth > 0)
    {
        dds_qset_history(dr_qos, DDS_HISTORY_KEEP_LAST, options->history_depth);
    }
    else if (options->history_depth == 0)
    {
        dds_qset_history(dr_qos, DDS_HISTORY_KEEP_ALL, 0);
    }
    {
        dds_history_kind_t history_kind;
        int32_t history_depth;
        dds_qget_history(dr_qos, &history_kind, &history_depth);
        log_message(DEBUG, "    History = %s", to_string_history(history_kind));
        if (history_kind == DDS_HISTORY_KEEP_LAST)
        {
            log_message(DEBUG, "    HistoryDepth = %d", history_depth);
        }
    }

    dds_qset_type_consistency(
        dr_qos, 
        options->type_consistency_kind,
        options->ignore_sequence_bounds,
        options->ignore_string_bounds,
        options->ignore_member_names,
        options->prevent_type_widening,
        options->force_type_validation);
    {
        dds_type_consistency_kind_t type_consistency_kind;
        bool ignore_sequence_bounds;
        bool ignore_string_bounds;
        bool ignore_member_names;
        bool prevent_type_widening;
        bool force_type_validation;

        dds_qget_type_consistency(
            dr_qos,
            &type_consistency_kind,
            &ignore_sequence_bounds,
            &ignore_string_bounds,
            &ignore_member_names,
            &prevent_type_widening,
            &force_type_validation);
        log_message(DEBUG, "    TypeConsistency * kind = %s", to_string_type_consistency(type_consistency_kind));
        log_message(DEBUG, "                    * ignore_sequence_bounds = %d", ignore_sequence_bounds);
        log_message(DEBUG, "                    * ignore_string_bounds   = %d", ignore_string_bounds);
        log_message(DEBUG, "                    * ignore_member_names    = %d", ignore_member_names);
        log_message(DEBUG, "                    * prevent_type_widening  = %d", prevent_type_widening);
        log_message(DEBUG, "                    * force_type_validation  = %d", force_type_validation);
    }    

    printf("Create reader for topic: %s\n", options->topic_name);

    app->dr = dds_create_reader(app->sub, app->topic, dr_qos, NULL);
    if (app->dr == 0)
    {
        log_message(ERROR, "failed to create datareader");
        return false;
    }

    size_t align, size;
    build_typecache_to (app->dtl->typecache, &app->dt->typeobj->_u.complete, &align, &size);

    log_message(DEBUG, "Data Reader created");
    return true;
}

//-------------------------------------------------------------
bool TestApplication_run_subscriber(TestApplication *app, TestOptions *options)
{
    log_message(DEBUG, "Run subscriber");
    while (!all_done)
    {
        dds_return_t retval;
        void* samples[10000];
        for(int i = 0; i < 10000; i++)
        {
            samples[i] = NULL;
        }
        dds_sample_info_t* sample_infos = malloc(sizeof(dds_sample_info_t) * 10000);

        do
        {
            retval = dds_take(app->dr, 
                              samples, 
                              sample_infos, 
                              10000, 
                              10000);
            if (retval > 0)
            {
                unsigned int i;
                for (i = 0; i < retval; i++)
                {
                    void* sample = samples[i];
                    dds_sample_info_t *sample_info = &sample_infos[i];

                    if (sample_info->valid_data)
                    {
                        printf("sample_received()\n");
                        dtl_print_sample(app->dtl, sample_info->valid_data, sample, &app->dt->typeobj->_u.complete);
                        if (check_data(sample, app->dtl, app->dt, options->data_folder, options->data_file))
                        {
                            printf("Received sample is the same as loaded\n");
                        }
                        else
                        {
                            printf("Received sample is not the same as loaded\n");
                        }
                    }
                    dds_return_loan(app->dr, &samples[i], 1);
                }
            }
        } while (retval > 0);
        dds_sleepfor(DDS_USECS(100000));
    }

    return true;
}

//-------------------------------------------------------------
bool TestApplication_run_publisher(TestApplication *app, TestOptions *options)
{
    void* dynamic_sample = NULL;
    if (init_data(&dynamic_sample, app->dtl, app->dt, options->data_folder, options->data_file) != DDS_RETCODE_OK)
    {
        log_message(ERROR, "Error initializing data");
        return false;
    }

    while (!all_done)
    {
        dds_write(app->dw, dynamic_sample);
        if (options->print_writer_samples)
        {
            printf(" Wrote:\n");
            dtl_print_sample(app->dtl, true, dynamic_sample, &app->dt->typeobj->_u.complete);
        }
        dds_sleepfor(DDS_USECS(1000000));
    }
    ddsrt_free(dynamic_sample);
    return true;
}

/*************************************************************/
int main(int argc, char *argv[])
{
    install_sig_handlers();

    TestOptions options = TestOptions_create();
    bool parseResult = TestOptions_parse(&options, argc, argv);
    if (!parseResult)
    {
        exit(ERROR_PARSING_ARGUMENTS);
    }
    TestApplication testApp = TestApplication_create();
    if (!TestApplication_initialize(&testApp, &options))
    {
        exit(ERROR_INITIALIZING);
    }
    if (!TestApplication_run(&testApp, &options))
    {
        exit(ERROR_RUNNING);
    }

    printf("Done.\n");

    return 0;
}
