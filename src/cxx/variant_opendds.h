#ifndef VARIANT_OPENDDS_H
#define VARIANT_OPENDDS_H

#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/RTPS/RtpsDiscovery.h>
#include <dds/DCPS/transport/framework/TransportRegistry.h>
#include <dds/DCPS/transport/rtps_udp/RtpsUdp.h>
#include <dds/DdsDynamicDataC.h>
#include <dds/DCPS/XTypes/DynamicTypeSupport.h>
#include <dds/DCPS/XTypes/DynamicDataFactory.h>
#include <dds/DCPS/XTypes/DynamicDataJson.h>
#include <dds/DCPS/XTypes/TypeObject.h>
#include <dds/DCPS/XTypes/XmlTypeProvider.h>
#include <ace/Init_ACE.h>

#include <cstdio>
#include <cstring>
#include <string>

#define LISTENER_STATUS_MASK_ALL (0xFFFFFFFF)
#define CONFIGURE_PARTICIPANT_FACTORY configure_opendds();

#define DDS_BOOLEAN_TRUE true
#define DDS_BOOLEAN_FALSE false
#define DDS_RETCODE_OK 0

namespace DDS {
    typedef DynamicType* DynamicType_ptr_xtypes;
}

inline OpenDDS::RTPS::RtpsDiscovery_rch& opendds_rtps_discovery() {
    static OpenDDS::RTPS::RtpsDiscovery_rch disc;
    return disc;
}

inline bool& opendds_type_information_disabled() {
    static bool disabled = false;
    return disabled;
}

inline void opendds_set_xtypes(OpenDDS::RTPS::RtpsDiscoveryConfig::UseXTypes use_xtypes) {
    OpenDDS::RTPS::RtpsDiscovery_rch& disc = opendds_rtps_discovery();
    if (!disc.is_nil()) {
        disc->use_xtypes(use_xtypes);
    }
}

inline void configure_opendds() {
    using namespace OpenDDS::DCPS;
    using namespace OpenDDS::RTPS;

    TheServiceParticipant->config_store()->set(
        "RTPS_DISCOVERY_RTPS_DISCOVERY_SPDP_PORT_MODE", "system");

    TransportInst_rch inst = TransportRegistry::instance()->get_inst("rtps_transport");
    if (inst.is_nil()) {
        inst = TransportRegistry::instance()->create_inst("rtps_transport", "rtps_udp");
    }
    TransportConfig_rch config = TransportRegistry::instance()->get_config("rtps_interop_demo");
    if (config.is_nil()) {
        config = TransportRegistry::instance()->create_config("rtps_interop_demo");
        if (!inst.is_nil()) {
            config->instances_.push_back(inst);
        }
    }
    TransportRegistry::instance()->global_config(config);

    RtpsDiscovery_rch& disc = opendds_rtps_discovery();
    if (disc.is_nil()) {
        disc = make_rch<RtpsDiscovery>("RtpsDiscovery");
        TheServiceParticipant->add_discovery(static_rchandle_cast<Discovery>(disc));
    }
    TheServiceParticipant->set_default_discovery(disc->key());
}

inline void StringSeq_push(DDS::StringSeq &string_seq, const char *elem) {
    CORBA::ULong len = string_seq.length();
    string_seq.length(len + 1);
    string_seq[len] = CORBA::string_dup(elem);
}

inline DDS::TypeConsistencyEnforcementQosPolicy TypeConsistency_get_default() {
    DDS::TypeConsistencyEnforcementQosPolicy policy;
    policy.kind = DDS::ALLOW_TYPE_COERCION;
    policy.ignore_member_names = false;
    policy.ignore_sequence_bounds = true;
    policy.ignore_string_bounds = true;
    policy.prevent_type_widening = false;
    policy.force_type_validation = false;
    return policy;
}

inline DDS::DynamicTypeSupport_ptr get_type_support(DDS::DynamicType_ptr dt) {
    return new ::DDS::DynamicTypeSupport(dt);
}

inline const char* get_qos_policy_name(DDS::QosPolicyId_t policy_id) {
    return "Unknown Policy";
}

inline void disable_type_information(DDS::DomainParticipantQos &dp_qos) {
    (void)dp_qos;
    opendds_type_information_disabled() = true;
    opendds_set_xtypes(OpenDDS::RTPS::RtpsDiscoveryConfig::XTYPES_NONE);
}

inline void set_type_object_version(DDS::DomainParticipantQos &dp_qos, int version) {
    (void)dp_qos;
    if (opendds_type_information_disabled()) {
        opendds_set_xtypes(OpenDDS::RTPS::RtpsDiscoveryConfig::XTYPES_NONE);
        return;
    }
    if (version != 2) {
        fprintf(stderr, "OpenDDS does not support selecting TypeObject version %d; using XTypes complete TypeInformation.\n", version);
    }
    opendds_set_xtypes(OpenDDS::RTPS::RtpsDiscoveryConfig::XTYPES_COMPLETE);
}

inline void print_type_objectV1(DDS::DynamicType_ptr dt) {
}

inline void print_type_objectV2(DDS::DynamicType_ptr dt) {
}

inline void print_typeid(DDS::DynamicType_ptr dt, int version) {
    (void)dt;
    (void)version;
}

inline std::string make_xtypes_path(const char* folder, const char* file, const char* subdir, const char* ext) {
    if (folder && file) {
        return std::string(folder) + "/" + subdir + "/" + file + ext;
    }
    if (file) {
        return std::string(file);
    }
    if (folder) {
        return std::string(folder);
    }
    return std::string();
}

inline DDS::DynamicType_ptr create_type(DDS::DomainParticipant_ptr dp, const char* type_folder, const char* type_file, const char* type_name) {
    (void)dp;
    if (!type_name) {
        return 0;
    }

    const std::string file_path = make_xtypes_path(type_folder, type_file, "xml", ".xml");
    if (file_path.empty()) {
        return 0;
    }

    DDS::DynamicType_var type;
    const DDS::ReturnCode_t rc = OpenDDS::XTypes::load_xml_type(
        type, ACE_TEXT_CHAR_TO_TCHAR(file_path.c_str()), type_name);
    return rc == DDS::RETCODE_OK ? type._retn() : 0;
}

inline DDS::DynamicType_ptr CREATE_TYPE(DDS::DomainParticipant_ptr dp, const char* type_folder, const char* type_file, const char* type_name) {
    return create_type(dp, type_folder, type_file, type_name);
}

inline DDS::ReturnCode_t register_type(DDS::DomainParticipant_ptr dp, DDS::DynamicType_ptr dt, const char* type_name) {
    if (!dp || !dt) return DDS::RETCODE_ERROR;
    DDS::DynamicTypeSupport_var ts = new ::DDS::DynamicTypeSupport(dt);
    return ts->register_type(dp, type_name);
}

inline DDS::ReturnCode_t REGISTER_TYPE(DDS::DomainParticipant_ptr dp, DDS::DynamicType_ptr dt, const char* type_name) {
    return register_type(dp, dt, type_name);
}

inline void cleanup_type(DDS::DomainParticipant_ptr dp, DDS::DynamicType_ptr dt) {
    (void)dp;
    (void)dt;
}

inline void CLEANUP_TYPE(DDS::DomainParticipant_ptr dp, DDS::DynamicType_ptr dt) {
    cleanup_type(dp, dt);
}

inline DDS::DynamicData_ptr create_data(DDS::DynamicType_ptr dt) {
    return ::DDS::DynamicDataFactory::get_instance()->create_data(dt);
}

inline DDS::DynamicData_ptr CREATE_DATA(DDS::DynamicType_ptr dt) {
    return create_data(dt);
}

inline DDS::ReturnCode_t init_data(DDS::DynamicData_ptr dd, const char* data_folder, const char* data_file) {
    if (!dd || (!data_folder && !data_file)) return DDS::RETCODE_OK;

    const std::string file_path = make_xtypes_path(data_folder, data_file, "json", ".json");
    OpenDDS::XTypes::DynamicDataJsonOptions options;
    options.discriminator_format = OpenDDS::XTypes::DYNAMIC_DATA_JSON_DISCRIMINATOR_AUTO;
    return OpenDDS::XTypes::dynamic_data_from_json_file(
        dd, file_path, options);
}

inline DDS::ReturnCode_t INIT_DATA(DDS::DynamicData_ptr dd, const char* data_folder, const char* data_file) {
    return init_data(dd, data_folder, data_file);
}

inline void print_data(DDS::DynamicData_ptr dd) {
    OpenDDS::XTypes::DynamicDataJsonOptions options;
    options.discriminator_format = OpenDDS::XTypes::DYNAMIC_DATA_JSON_DISCRIMINATOR_ACTIVE_MEMBER;
    std::string json;
    if (OpenDDS::XTypes::dynamic_data_to_json(json, dd, options) == DDS::RETCODE_OK) {
        printf("%s\n", json.c_str());
    } else {
        printf("{\"<dynamic_data_json_error>\":true}\n");
    }
}

inline void PRINT_DATA(DDS::DynamicData_ptr dd) {
    print_data(dd);
}

inline void cleanup_data(DDS::DynamicData_ptr dd) {
    (void)dd;
}

inline void CLEANUP_DATA(DDS::DynamicData_ptr dd) {
    cleanup_data(dd);
}

inline bool check_data(DDS::DynamicData_ptr dd, const char* data_folder, const char* data_file) {
    if (!dd || (!data_folder && !data_file)) return true;
    DDS::DynamicData_var expected = create_data(dd->type());
    if (init_data(expected, data_folder, data_file) != DDS_RETCODE_OK) return false;
    const bool equal = OpenDDS::XTypes::dynamic_data_equal(dd, expected);
    if (!equal) {
        printf("Expected:\n");
        print_data(expected);
    }
    return equal;
}

inline bool CHECK_DATA(DDS::DynamicData_ptr dd, const char* data_folder, const char* data_file) {
    return check_data(dd, data_folder, data_file);
}

#define SECONDS_FIELD_NAME sec
#define LISTENER_STATUS_MASK_NONE 0
#define GET_TOPIC_DESCRIPTION(dr) dr->get_topicdescription()
#define OBTAIN_DOMAIN_PARTICIPANT_FACTORY TheParticipantFactory

struct TypeConsistencyKind_O {
    CORBA::Short value;
    TypeConsistencyKind_O(CORBA::Short v) : value(v) {}
    operator CORBA::Short() const { return value; }
};
#define TypeConsistencyKind TypeConsistencyKind_O

#define sample_info (&sample_infos[i])
#define sample samples[i]

#endif
