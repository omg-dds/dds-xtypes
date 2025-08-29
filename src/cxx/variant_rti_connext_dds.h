#include <iostream>
#include <fstream>
#include <sstream>
#include "ndds/ndds_namespace_cpp.h"

#define LISTENER_STATUS_MASK_ALL (DDS_STATUS_MASK_ALL)
namespace DDS{
    typedef TypeCode DynamicType;
    typedef TypeCodeFactory DynamicTypeFactory;
}

DDS::DynamicDataTypeSupport * g_type_support = NULL;

DDS::TypeConsistencyEnforcementQosPolicy
TypeConsistency_get_default(void) {
    return DDS_TYPE_CONSISTENCY_ENFORCEMENT_QOS_POLICY_DEFAULT;
}

void
PRINT_DATA_JSON(DDS::DynamicData *dd)
{
    unsigned int str_size = 0;

    DDS::PrintFormatProperty property = DDS_PRINT_FORMAT_PROPERTY_DEFAULT;
    property.kind = DDS_JSON_PRINT_FORMAT;

    if (dd->to_string(NULL, str_size, property) != DDS::RETCODE_OK) {
        std::cerr << "Failed to get size" << std::endl;
        return;
    }

    char* json_str = new char[str_size];

    if (dd->to_string(json_str, str_size, property) != DDS::RETCODE_OK) {
        std::cerr << "Failed to get string" << std::endl;
        delete[] json_str;
        return;
    }

    printf("%s\n", json_str);
    delete[] json_str;
}

void StringSeq_push(DDS::StringSeq  &string_seq, const char *elem)
{
    string_seq.ensure_length(string_seq.length()+1, string_seq.length()+1);
    string_seq[string_seq.length()-1] = DDS_String_dup(elem);
}

DDS::DynamicDataTypeSupport * get_type_support(DDS::DynamicType* dt) {
    if (g_type_support == NULL) {
        g_type_support = new DDS::DynamicDataTypeSupport(
                dt,
                DDS_DYNAMIC_DATA_TYPE_PROPERTY_DEFAULT);
    }
    return g_type_support;
}

const char *get_qos_policy_name(DDS_QosPolicyId_t policy_id)
{
    return DDS_QosPolicyId_to_string(policy_id); // not standard...
}

void disable_type_info(DDS::DomainParticipantQos &dp_qos)
{
    dp_qos.resource_limits.type_code_max_serialized_length = 0;
    dp_qos.resource_limits.type_object_max_serialized_length = 0;
}

DDS::DynamicType * CREATE_TYPE(
    DDS::DomainParticipant * dp,
    const char * types_uri,
    const char * type_name )
{
    const DDS::DynamicType *dt = NULL;
    DDS::DomainParticipantFactoryQos factory_qos;

    if (dp && types_uri && type_name) {
        if (TheParticipantFactory->get_qos(factory_qos) != DDS_RETCODE_OK) {
            return NULL;
        }
        if (!factory_qos.profile.url_profile.ensure_length(1, 1)) {
            return NULL;
        }

        factory_qos.profile.url_profile[0] = DDS_String_dup(types_uri);
        if (factory_qos.profile.url_profile[0] == NULL) {
            return NULL;
        }
        if (DDSTheParticipantFactory->set_qos(factory_qos) != DDS_RETCODE_OK) {
            return NULL;
        }
        dt = DDSTheParticipantFactory->get_typecode_from_config(type_name);
    }
  return (DDS::DynamicType *) dt;
}

DDS::ReturnCode_t
REGISTER_TYPE(
        DDS::DomainParticipant * dp,
        DDS::DynamicType * dt,
        const char * type_name )
{
    DDS::ReturnCode_t retval = DDS::RETCODE_ERROR;
    if (dp != NULL && dt != NULL && type_name != NULL) {
        DDS::DynamicDataTypeSupport* type_support = new DDS::DynamicDataTypeSupport(
                dt,
                DDS_DYNAMIC_DATA_TYPE_PROPERTY_DEFAULT);
        if (type_support != NULL) {
            retval = type_support->register_type(dp, type_name);
        }
    }

    return retval;
}

void CLEANUP_TYPE(DDS::DomainParticipant *dp, DDS::DynamicType *dt )
{
    if (dp != NULL && dt != NULL) {
        DDS::DynamicTypeFactory* dt_factory =
                DDS::DynamicTypeFactory::get_instance();

        if (dt_factory != NULL) {
            DDS_ExceptionCode_t ex = DDS_NO_EXCEPTION_CODE;
            dt_factory->delete_tc(dt, ex);
        }
    }

    if (g_type_support != NULL) {
        delete g_type_support;
    }
}

DDS::DynamicData * CREATE_DATA(DDS::DynamicType *dt)
{
    DDS::DynamicData * retval = NULL;

    DDS::DynamicDataTypeSupport* type_support = get_type_support(dt);

    retval = type_support->create_data();

    return retval;
}

DDS::ReturnCode_t INIT_DATA(
        DDS::DynamicData *dd,
        const char *xml_data_uri,
        const char *json_data_uri)
{
    DDS::ReturnCode_t retval = DDS::RETCODE_ERROR;

    (void) xml_data_uri;

    if (dd != NULL && json_data_uri != NULL) {
        PRINT_DATA_JSON(dd);
        std::ifstream file(json_data_uri);
        if (!file) {
            std::cerr << "Failed to open the file: " << std::string(json_data_uri)
                    << std::endl;
            return retval;
        }

        // Read the entire file into a stream
        std::ostringstream buffer;
        buffer << file.rdbuf();

        if (dd != NULL && json_data_uri != NULL) {
            retval = dd->from_string(buffer.str().c_str(), DDS_JSON_PRINT_FORMAT);
        }
    }
    return retval;
}

void
PRINT_DATA(DDS::DynamicData *dd)
{
    // unsigned int str_size = 0;

    // DDS::PrintFormatProperty property = DDS_PRINT_FORMAT_PROPERTY_DEFAULT;
    // property.kind = DDS_JSON_PRINT_FORMAT;

    // if (dd->to_string(NULL, str_size, property) != DDS::RETCODE_OK) {
    //     std::cerr << "Failed to get size" << std::endl;
    //     return;
    // }

    // char* json_str = new char[str_size];

    // if (dd->to_string(json_str, str_size, property) != DDS::RETCODE_OK) {
    //     std::cerr << "Failed to get string" << std::endl;
    //     delete[] json_str;
    //     return;
    // }

    // printf("%s\n", json_str);
    // delete[] json_str;

    //dd->print(stdout, 0);
    PRINT_DATA_JSON(dd);

}

void CLEANUP_DATA(DDS::DynamicData *dd)
{
    DDS::DynamicDataTypeSupport* type_support = get_type_support(
            (DDS::DynamicType *)dd->get_type());

    type_support->delete_data(dd);
}

bool
CHECK_DATA(DDS::DynamicData *dd, const char *xml_data_uri, const char *json_data_uri)
{
    bool retval = false;

    (void) xml_data_uri;

    if (dd == NULL && json_data_uri == NULL) {
        return retval;
    }

    DDS::DynamicData *data_check = CREATE_DATA(
            (DDS::DynamicType *) dd->get_type());
    if (data_check == NULL) {
        retval = false;
        goto done;
    }
    if (INIT_DATA(data_check, xml_data_uri, json_data_uri) != DDS_RETCODE_OK) {
        retval = false;
        goto done;
    }

    retval = *dd == *data_check;

done:
    if (data_check != NULL) {
        CLEANUP_DATA(data_check);
    }
    return retval;
}
