
#pragma once

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilderFactory.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicDataFactory.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeMember.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicData.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicPubSubType.hpp>

#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <atomic>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstdio>

namespace eprosima::fastdds::dds {

using DynamicDataWriter = DataWriter;
using DynamicDataReader = DataReader;

/* Real Fast DDS type, kept accessible even after the macro below renames it. */
using FDDS_TypeConsistency_Real = TypeConsistencyEnforcementQosPolicy;

/*
 * Compatibility shim: test_main.cxx uses RTI-style field names (no m_ prefix)
 * and an RTI-only field (ignore_enum_literal_names). Map them to Fast DDS m_ fields
 * via the implicit conversion operator used when assigning to dr_qos.type_consistency().
 */
struct FDDSTypeConsistency {
    TypeConsistencyKind kind                   = ALLOW_TYPE_COERCION;
    bool ignore_sequence_bounds                = true;
    bool ignore_string_bounds                  = true;
    bool ignore_member_names                   = false;
    bool prevent_type_widening                 = false;
    bool force_type_validation                 = false;
    bool ignore_enum_literal_names             = false;   // RTI-only, silently ignored

    operator FDDS_TypeConsistency_Real() const {
        FDDS_TypeConsistency_Real q;
        q.m_kind                   = kind;
        q.m_ignore_sequence_bounds = ignore_sequence_bounds;
        q.m_ignore_string_bounds   = ignore_string_bounds;
        q.m_ignore_member_names    = ignore_member_names;
        q.m_prevent_type_widening  = prevent_type_widening;
        q.m_force_type_validation  = force_type_validation;
        return q;
    }
};



/* Container of dynamically received samples, owns shared_ptr lifetimes. */
class DynamicDataSeq {
public:
    std::vector<traits<DynamicData>::ref_type> _data;
    std::vector<SampleInfo>                    _infos;

    uint32_t    length()          const { return (uint32_t)_data.size(); }
    DynamicData& operator[](uint32_t i)  { return *_data[i]; }
    SampleInfo*  get_info(uint32_t i)    { return &_infos[i]; }
    void         clear()                  { _data.clear(); _infos.clear(); }
};

/* QoS proxy wrappers */
/* Fast DDS 3.x exposes sub-policies via methods; these add public reference  */
/* members so test_main.cxx field-syntax compiles unchanged.                  */

struct FDDS_DataWriterQos : public DataWriterQos {
    ReliabilityQosPolicy&        reliability;
    DurabilityQosPolicy&         durability;
    HistoryQosPolicy&            history;
    OwnershipQosPolicy&          ownership;
    OwnershipStrengthQosPolicy&  ownership_strength;
    DeadlineQosPolicy&           deadline;
    DataRepresentationQosPolicy& representation;

    FDDS_DataWriterQos()
        : DataWriterQos()
        , reliability(DataWriterQos::reliability())
        , durability(DataWriterQos::durability())
        , history(DataWriterQos::history())
        , ownership(DataWriterQos::ownership())
        , ownership_strength(DataWriterQos::ownership_strength())
        , deadline(DataWriterQos::deadline())
        , representation(DataWriterQos::representation())
    {}
};

struct FDDS_DataReaderQos : public DataReaderQos {
    ReliabilityQosPolicy&        reliability;
    DurabilityQosPolicy&         durability;
    HistoryQosPolicy&            history;
    OwnershipQosPolicy&          ownership;
    DeadlineQosPolicy&           deadline;
    TimeBasedFilterQosPolicy&    time_based_filter;
    DataRepresentationQosPolicy& representation;

    FDDS_DataReaderQos()
        : DataReaderQos()
        , reliability(DataReaderQos::reliability())
        , durability(DataReaderQos::durability())
        , history(DataReaderQos::history())
        , ownership(DataReaderQos::ownership())
        , deadline(DataReaderQos::deadline())
        , time_based_filter(DataReaderQos::time_based_filter())
        , representation(DataReaderQos::representation())
    {}
};

} // namespace eprosima::fastdds::dds

namespace DDS = eprosima::fastdds::dds;

/* Shadow the QoS types so test_main.cxx field-access syntax works. */
#define DataWriterQos                      FDDS_DataWriterQos
#define DataReaderQos                      FDDS_DataReaderQos

/* Make the shim struct the visible TypeConsistencyEnforcementQosPolicy.      */
/* Must come before TypeConsistency_get_default() so it returns FDDSTypeConsistency. */
#define TypeConsistencyEnforcementQosPolicy FDDSTypeConsistency

/* RTI boolean macros */
#define DDS_BOOLEAN_FALSE false
#define DDS_BOOLEAN_TRUE  true

/* RTI return-code macro */
#define DDS_RETCODE_OK    RETCODE_OK

/* Duration_t uses 'seconds' in Fast DDS, not 'sec' (RTI-ism). */
#define sec seconds

/* StatusMask aliases */
#define LISTENER_STATUS_MASK_ALL  DDS::StatusMask::all()
#define LISTENER_STATUS_MASK_NONE DDS::StatusMask::none()

/* Fast DDS fires on_inconsistent_topic only when the same type NAME is
 * registered with different TypeObjects (hash mismatch), not when type
 * names differ.  By registering every dynamic type under one fixed name
 * we let the TypeObject-based assignability check determine compatibility,
 * which causes on_inconsistent_topic to fire for truly incompatible types. */
#define FDDS_REGISTERED_TYPE_NAME "XTypesTestType"

/* Keep the dynamic type and publisher data alive across function boundaries. */
static DDS::traits<DDS::DynamicType>::ref_type g_dynamic_type_ref;
static DDS::traits<DDS::DynamicData>::ref_type g_publisher_data_ref;

/* StringSeq_push */
inline void StringSeq_push(DDS::PartitionQosPolicy& partition, const char* elem)
{
    partition.push_back(elem);
}

/* ── FDDS_DECLARE_NAMES: declare topic_name/type_name as const char* from  ── */
/* entity->get_name()/get_type_name() which return std::string in Fast DDS.    */
#define FDDS_DECLARE_NAMES(entity) \
    const std::string& _fdds_sn_ = (entity)->get_name(); \
    const std::string& _fdds_st_ = (entity)->get_type_name(); \
    const char* topic_name = _fdds_sn_.c_str(); \
    const char* type_name  = _fdds_st_.c_str()

/* TypeConsistency default */
inline DDS::TypeConsistencyEnforcementQosPolicy TypeConsistency_get_default()
{
    DDS::TypeConsistencyEnforcementQosPolicy qos;
    return qos;
}

/* Participant-factory / type-object-version stubs */
inline void _fdds_set_type_propagation(
    DDS::DomainParticipantQos& dp_qos,
    const char* value)
{
    auto& props = dp_qos.properties().properties();
    for (auto& prop : props) {
        if (prop.name() == DDS::parameter_policy_type_propagation) {
            prop.value(value);
            return;
        }
    }
    props.emplace_back(DDS::parameter_policy_type_propagation, value);
}

inline void disable_type_information(DDS::DomainParticipantQos& dp_qos)
{
    _fdds_set_type_propagation(dp_qos, "disabled");
}

inline void set_type_object_version(DDS::DomainParticipantQos& /*dp_qos*/, int /*version*/) {}

inline void print_typeid(DDS::DynamicType* /*dt*/, int /*version*/) {}

inline const char* get_qos_policy_name(DDS::QosPolicyId_t /*policy_id*/) { return "unknown"; }

/* create_type */
inline DDS::DynamicType* create_type(
    DDS::DomainParticipant* /*dp*/,
    const char* type_folder,
    const char* type_file,
    const char* type_name)
{
    if ((!type_folder && !type_file) || !type_name) return nullptr;

    std::string idl_path = std::string(type_folder ? type_folder : "")
                         + "/idl/"
                         + std::string(type_file ? type_file : "")
                         + ".idl";

    DDS::traits<DDS::DynamicTypeBuilder>::ref_type builder;
    builder = DDS::DynamicTypeBuilderFactory::get_instance()->create_type_w_uri(
            idl_path.c_str(), type_name, {});
    if (!builder) {
        fprintf(stderr, "create_type: cannot load type '%s' from IDL '%s'\n",
                type_name, idl_path.c_str());
        return nullptr;
    }
    g_dynamic_type_ref = builder->build();
    return g_dynamic_type_ref.get();
}

/* register_type */
inline DDS::ReturnCode_t register_type(
    DDS::DomainParticipant* dp,
    DDS::DynamicType*       /*dt*/,
    const char*             /*type_name*/)
{
    if (!dp || !g_dynamic_type_ref)
        return DDS::RETCODE_BAD_PARAMETER;
    DDS::TypeSupport ts(new DDS::DynamicPubSubType(g_dynamic_type_ref));
    return dp->register_type(ts, FDDS_REGISTERED_TYPE_NAME);
}

/* cleanup_type */
inline void cleanup_type(DDS::DomainParticipant* /*dp*/, DDS::DynamicType* /*dt*/)
{
    g_dynamic_type_ref.reset();
}

/* create_data */
inline DDS::DynamicData* create_data(DDS::DynamicType* /*dt*/)
{
    if (!g_dynamic_type_ref) return nullptr;
    g_publisher_data_ref =
        DDS::DynamicDataFactory::get_instance()->create_data(g_dynamic_type_ref);
    return g_publisher_data_ref.get();
}

struct FddsJsonValue {
    enum class Kind { NIL, INT, DOUBLE, STRING, BOOL, ARRAY, OBJECT } kind = Kind::NIL;
    int64_t i = 0;
    double d = 0.0;
    std::string s;
    bool b = false;
    std::vector<FddsJsonValue> a;
    std::vector<std::pair<std::string, FddsJsonValue>> o;
};

static void _fdds_json_skip_ws(const std::string& s, size_t& pos)
{
    while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) {
        ++pos;
    }
}

static bool _fdds_parse_json_string(
    const std::string& s,
    size_t& pos,
    std::string& out)
{
    if (pos >= s.size() || s[pos] != '"') {
        return false;
    }
    ++pos;
    out.clear();
    while (pos < s.size()) {
        char c = s[pos++];
        if (c == '"') {
            return true;
        }
        if (c == '\\') {
            if (pos >= s.size()) {
                return false;
            }
            char esc = s[pos++];
            switch (esc) {
                case '"': out += '"'; break;
                case '\\': out += '\\'; break;
                case '/': out += '/'; break;
                case 'b': out += '\b'; break;
                case 'f': out += '\f'; break;
                case 'n': out += '\n'; break;
                case 'r': out += '\r'; break;
                case 't': out += '\t'; break;
                case 'u':
                {
                    if (pos + 4 > s.size()) {
                        return false;
                    }
                    unsigned code = 0;
                    for (int i = 0; i < 4; ++i) {
                        char h = s[pos++];
                        code <<= 4;
                        if (h >= '0' && h <= '9') {
                            code += static_cast<unsigned>(h - '0');
                        } else if (h >= 'a' && h <= 'f') {
                            code += static_cast<unsigned>(h - 'a' + 10);
                        } else if (h >= 'A' && h <= 'F') {
                            code += static_cast<unsigned>(h - 'A' + 10);
                        } else {
                            return false;
                        }
                    }
                    if (code <= 0x7f) {
                        out += static_cast<char>(code);
                    } else if (code <= 0x7ff) {
                        out += static_cast<char>(0xc0 | (code >> 6));
                        out += static_cast<char>(0x80 | (code & 0x3f));
                    } else {
                        out += static_cast<char>(0xe0 | (code >> 12));
                        out += static_cast<char>(0x80 | ((code >> 6) & 0x3f));
                        out += static_cast<char>(0x80 | (code & 0x3f));
                    }
                    break;
                }
                default: out += esc; break;
            }
        } else {
            out += c;
        }
    }
    return false;
}

static bool _fdds_parse_json_value(
    const std::string& s,
    size_t& pos,
    FddsJsonValue& out);

static bool _fdds_parse_json_array(
    const std::string& s,
    size_t& pos,
    FddsJsonValue& out)
{
    if (pos >= s.size() || s[pos] != '[') {
        return false;
    }
    ++pos;
    out = FddsJsonValue{};
    out.kind = FddsJsonValue::Kind::ARRAY;
    _fdds_json_skip_ws(s, pos);
    if (pos < s.size() && s[pos] == ']') {
        ++pos;
        return true;
    }
    while (pos < s.size()) {
        FddsJsonValue elem;
        if (!_fdds_parse_json_value(s, pos, elem)) {
            return false;
        }
        out.a.push_back(std::move(elem));
        _fdds_json_skip_ws(s, pos);
        if (pos < s.size() && s[pos] == ',') {
            ++pos;
            continue;
        }
        if (pos < s.size() && s[pos] == ']') {
            ++pos;
            return true;
        }
        return false;
    }
    return false;
}

static bool _fdds_parse_json_object(
    const std::string& s,
    size_t& pos,
    FddsJsonValue& out)
{
    if (pos >= s.size() || s[pos] != '{') {
        return false;
    }
    ++pos;
    out = FddsJsonValue{};
    out.kind = FddsJsonValue::Kind::OBJECT;
    _fdds_json_skip_ws(s, pos);
    if (pos < s.size() && s[pos] == '}') {
        ++pos;
        return true;
    }
    while (pos < s.size()) {
        std::string key;
        if (!_fdds_parse_json_string(s, pos, key)) {
            return false;
        }
        _fdds_json_skip_ws(s, pos);
        if (pos >= s.size() || s[pos] != ':') {
            return false;
        }
        ++pos;
        FddsJsonValue value;
        if (!_fdds_parse_json_value(s, pos, value)) {
            return false;
        }
        out.o.push_back({std::move(key), std::move(value)});
        _fdds_json_skip_ws(s, pos);
        if (pos < s.size() && s[pos] == ',') {
            ++pos;
            _fdds_json_skip_ws(s, pos);
            continue;
        }
        if (pos < s.size() && s[pos] == '}') {
            ++pos;
            return true;
        }
        return false;
    }
    return false;
}

static bool _fdds_parse_json_number(
    const std::string& s,
    size_t& pos,
    FddsJsonValue& out)
{
    char* end = nullptr;
    const char* start = s.c_str() + pos;
    double d = std::strtod(start, &end);
    if (end == start) {
        return false;
    }
    std::string token(start, static_cast<size_t>(end - start));
    if (token.find('.') != std::string::npos ||
            token.find('e') != std::string::npos ||
            token.find('E') != std::string::npos) {
        out.kind = FddsJsonValue::Kind::DOUBLE;
        out.d = d;
    } else {
        out.kind = FddsJsonValue::Kind::INT;
        out.i = std::strtoll(start, nullptr, 10);
    }
    pos = static_cast<size_t>(end - s.c_str());
    return true;
}

static bool _fdds_parse_json_value(
    const std::string& s,
    size_t& pos,
    FddsJsonValue& out)
{
    _fdds_json_skip_ws(s, pos);
    if (pos >= s.size()) {
        return false;
    }
    if (s[pos] == '{') {
        return _fdds_parse_json_object(s, pos, out);
    }
    if (s[pos] == '[') {
        return _fdds_parse_json_array(s, pos, out);
    }
    if (s[pos] == '"') {
        out = FddsJsonValue{};
        out.kind = FddsJsonValue::Kind::STRING;
        return _fdds_parse_json_string(s, pos, out.s);
    }
    if (s.compare(pos, 4, "true") == 0) {
        out = FddsJsonValue{};
        out.kind = FddsJsonValue::Kind::BOOL;
        out.b = true;
        pos += 4;
        return true;
    }
    if (s.compare(pos, 5, "false") == 0) {
        out = FddsJsonValue{};
        out.kind = FddsJsonValue::Kind::BOOL;
        out.b = false;
        pos += 5;
        return true;
    }
    if (s.compare(pos, 4, "null") == 0) {
        out = FddsJsonValue{};
        out.kind = FddsJsonValue::Kind::NIL;
        pos += 4;
        return true;
    }
    return _fdds_parse_json_number(s, pos, out);
}

/* JSON parser for the XTypes sample files */
static bool _fdds_parse_json(
    const std::string& json,
    std::vector<std::pair<std::string,FddsJsonValue>>& out)
{
    size_t pos = 0;
    FddsJsonValue root;
    if (!_fdds_parse_json_value(json, pos, root)) {
        return false;
    }
    _fdds_json_skip_ws(json, pos);
    if (pos != json.size() || root.kind != FddsJsonValue::Kind::OBJECT) {
        return false;
    }
    out = std::move(root.o);
    return true;
}

static bool _fdds_enum_literal_to_int(
    const std::string& literal,
    int32_t& value)
{
    /* JSON data files use VAL0, VAL1, VAL2, ... naming convention. */
    if (literal.size() > 3 && literal.rfind("VAL", 0) == 0) {
        char* end = nullptr;
        long parsed = std::strtol(literal.c_str() + 3, &end, 10);
        if (end && *end == '\0') {
            value = static_cast<int32_t>(parsed);
            return true;
        }
    }
    return false;
}

static bool _fdds_value_to_long_double(
    const FddsJsonValue& value,
    long double& out)
{
    if (value.kind == FddsJsonValue::Kind::INT) {
        out = static_cast<long double>(value.i);
        return true;
    }
    if (value.kind == FddsJsonValue::Kind::DOUBLE) {
        out = static_cast<long double>(value.d);
        return true;
    }
    if (value.kind == FddsJsonValue::Kind::BOOL) {
        out = value.b ? 1.0L : 0.0L;
        return true;
    }
    if (value.kind == FddsJsonValue::Kind::STRING) {
        if (value.s.rfind("0x", 0) == 0 || value.s.rfind("0X", 0) == 0) {
            out = 1.0L;
            return true;
        }
        char* end = nullptr;
        long double parsed = std::strtold(value.s.c_str(), &end);
        if (end != value.s.c_str() && *end == '\0') {
            out = parsed;
            return true;
        }
    }
    return false;
}

template<typename T>
static bool _fdds_array_to_integral_seq(
    const FddsJsonValue& value,
    std::vector<T>& out)
{
    if (value.kind != FddsJsonValue::Kind::ARRAY) {
        return false;
    }
    out.clear();
    out.reserve(value.a.size());
    for (const auto& elem : value.a) {
        int64_t v = 0;
        if (elem.kind == FddsJsonValue::Kind::INT) {
            v = elem.i;
        } else if (elem.kind == FddsJsonValue::Kind::BOOL) {
            v = elem.b ? 1 : 0;
        } else if (elem.kind == FddsJsonValue::Kind::STRING) {
            int32_t enum_value = 0;
            if (!_fdds_enum_literal_to_int(elem.s, enum_value)) {
                return false;
            }
            v = enum_value;
        } else {
            return false;
        }
        out.push_back(static_cast<T>(v));
    }
    return true;
}

template<typename T>
static bool _fdds_array_to_float_seq(
    const FddsJsonValue& value,
    std::vector<T>& out)
{
    if (value.kind != FddsJsonValue::Kind::ARRAY) {
        return false;
    }
    out.clear();
    out.reserve(value.a.size());
    for (const auto& elem : value.a) {
        long double v = 0.0L;
        if (!_fdds_value_to_long_double(elem, v)) {
            return false;
        }
        out.push_back(static_cast<T>(v));
    }
    return true;
}

static bool _fdds_array_to_string_seq(
    const FddsJsonValue& value,
    DDS::StringSeq& out)
{
    if (value.kind != FddsJsonValue::Kind::ARRAY) {
        return false;
    }
    out.clear();
    out.reserve(value.a.size());
    for (const auto& elem : value.a) {
        if (elem.kind != FddsJsonValue::Kind::STRING) {
            return false;
        }
        out.push_back(elem.s);
    }
    return true;
}

static bool _fdds_array_to_wstring_seq(
    const FddsJsonValue& value,
    DDS::WstringSeq& out)
{
    DDS::StringSeq strings;
    if (!_fdds_array_to_string_seq(value, strings)) {
        return false;
    }
    out.clear();
    out.reserve(strings.size());
    for (const auto& s : strings) {
        out.emplace_back(s.begin(), s.end());
    }
    return true;
}

static DDS::ReturnCode_t _fdds_set_json_value(
    DDS::DynamicData* dd,
    DDS::MemberId id,
    const FddsJsonValue& value);

static DDS::ReturnCode_t _fdds_set_json_object(
    DDS::DynamicData* dd,
    const FddsJsonValue& value)
{
    if (!dd || value.kind != FddsJsonValue::Kind::OBJECT) {
        return DDS::RETCODE_BAD_PARAMETER;
    }
    for (const auto& kv : value.o) {
        DDS::MemberId id = dd->get_member_id_by_name(kv.first);
        if (id == DDS::MEMBER_ID_INVALID) {
            fprintf(stderr, "init_data: unknown member '%s'\n", kv.first.c_str());
            continue;
        }
        DDS::ReturnCode_t ret = _fdds_set_json_value(dd, id, kv.second);
        if (ret != DDS::RETCODE_OK) {
            fprintf(stderr, "init_data: cannot set member '%s'\n", kv.first.c_str());
            return ret;
        }
    }
    return DDS::RETCODE_OK;
}

static DDS::ReturnCode_t _fdds_set_json_array_by_loan(
    DDS::DynamicData* dd,
    DDS::MemberId id,
    const FddsJsonValue& value)
{
    auto collection = dd->loan_value(id);
    if (!collection) {
        return DDS::RETCODE_BAD_PARAMETER;
    }

    DDS::ReturnCode_t ret = DDS::RETCODE_OK;
    for (size_t i = 0; i < value.a.size(); ++i) {
        ret = _fdds_set_json_value(collection.get(),
                static_cast<DDS::MemberId>(i), value.a[i]);
        if (ret != DDS::RETCODE_OK) {
            break;
        }
    }
    DDS::ReturnCode_t loan_ret = dd->return_loaned_value(collection);
    return ret == DDS::RETCODE_OK ? loan_ret : ret;
}

static DDS::ReturnCode_t _fdds_set_json_array_value(
    DDS::DynamicData* dd,
    DDS::MemberId id,
    const FddsJsonValue& value)
{
    DDS::Int32Seq i32;
    if (_fdds_array_to_integral_seq(value, i32)) {
        DDS::ReturnCode_t ret = dd->set_int32_values(id, i32);
        if (ret == DDS::RETCODE_OK) return ret;
    }
    DDS::UInt32Seq u32;
    if (_fdds_array_to_integral_seq(value, u32)) {
        DDS::ReturnCode_t ret = dd->set_uint32_values(id, u32);
        if (ret == DDS::RETCODE_OK) return ret;
    }
    DDS::Int8Seq i8;
    if (_fdds_array_to_integral_seq(value, i8)) {
        DDS::ReturnCode_t ret = dd->set_int8_values(id, i8);
        if (ret == DDS::RETCODE_OK) return ret;
    }
    DDS::UInt8Seq u8;
    if (_fdds_array_to_integral_seq(value, u8)) {
        DDS::ReturnCode_t ret = dd->set_uint8_values(id, u8);
        if (ret == DDS::RETCODE_OK) return ret;
    }
    DDS::Int16Seq i16;
    if (_fdds_array_to_integral_seq(value, i16)) {
        DDS::ReturnCode_t ret = dd->set_int16_values(id, i16);
        if (ret == DDS::RETCODE_OK) return ret;
    }
    DDS::UInt16Seq u16;
    if (_fdds_array_to_integral_seq(value, u16)) {
        DDS::ReturnCode_t ret = dd->set_uint16_values(id, u16);
        if (ret == DDS::RETCODE_OK) return ret;
    }
    DDS::Int64Seq i64;
    if (_fdds_array_to_integral_seq(value, i64)) {
        DDS::ReturnCode_t ret = dd->set_int64_values(id, i64);
        if (ret == DDS::RETCODE_OK) return ret;
    }
    DDS::UInt64Seq u64;
    if (_fdds_array_to_integral_seq(value, u64)) {
        DDS::ReturnCode_t ret = dd->set_uint64_values(id, u64);
        if (ret == DDS::RETCODE_OK) return ret;
    }
    DDS::Float32Seq f32;
    if (_fdds_array_to_float_seq(value, f32)) {
        DDS::ReturnCode_t ret = dd->set_float32_values(id, f32);
        if (ret == DDS::RETCODE_OK) return ret;
    }
    DDS::Float64Seq f64;
    if (_fdds_array_to_float_seq(value, f64)) {
        DDS::ReturnCode_t ret = dd->set_float64_values(id, f64);
        if (ret == DDS::RETCODE_OK) return ret;
    }
    DDS::Float128Seq f128;
    if (_fdds_array_to_float_seq(value, f128)) {
        DDS::ReturnCode_t ret = dd->set_float128_values(id, f128);
        if (ret == DDS::RETCODE_OK) return ret;
    }
    DDS::StringSeq strings;
    if (_fdds_array_to_string_seq(value, strings)) {
        DDS::ReturnCode_t ret = dd->set_string_values(id, strings);
        if (ret == DDS::RETCODE_OK) return ret;

        DDS::CharSeq chars;
        bool all_chars = true;
        for (const auto& s : strings) {
            if (s.size() != 1) {
                all_chars = false;
                break;
            }
            chars.push_back(s[0]);
        }
        if (all_chars) {
            ret = dd->set_char8_values(id, chars);
            if (ret == DDS::RETCODE_OK) return ret;
        }

        DDS::WstringSeq wstrings;
        if (_fdds_array_to_wstring_seq(value, wstrings)) {
            ret = dd->set_wstring_values(id, wstrings);
            if (ret == DDS::RETCODE_OK) return ret;
        }
    }

    return _fdds_set_json_array_by_loan(dd, id, value);
}

static DDS::ReturnCode_t _fdds_set_json_value(
    DDS::DynamicData* dd,
    DDS::MemberId id,
    const FddsJsonValue& value)
{
    switch (value.kind) {
        case FddsJsonValue::Kind::ARRAY:
            return _fdds_set_json_array_value(dd, id, value);
        case FddsJsonValue::Kind::OBJECT:
        {
            auto nested = dd->loan_value(id);
            if (!nested) {
                return DDS::RETCODE_BAD_PARAMETER;
            }
            DDS::ReturnCode_t ret = _fdds_set_json_object(nested.get(), value);
            DDS::ReturnCode_t loan_ret = dd->return_loaned_value(nested);
            return ret == DDS::RETCODE_OK ? loan_ret : ret;
        }
        case FddsJsonValue::Kind::STRING:
        {
            DDS::ReturnCode_t ret = dd->set_string_value(id, value.s);
            if (ret == DDS::RETCODE_OK) return ret;
            if (value.s.size() == 1) {
                ret = dd->set_char8_value(id, value.s[0]);
                if (ret == DDS::RETCODE_OK) return ret;
            }
            int32_t enum_value = 0;
            if (_fdds_enum_literal_to_int(value.s, enum_value)) {
                ret = dd->set_int32_value(id, enum_value);
                if (ret == DDS::RETCODE_OK) return ret;
            }
            long double float128_value = 0.0L;
            if (_fdds_value_to_long_double(value, float128_value)) {
                ret = dd->set_float128_value(id, float128_value);
                if (ret == DDS::RETCODE_OK) return ret;
            }
            /* Decode UTF-8 string to wstring (proper code-point conversion). */
            std::wstring ws;
            const unsigned char* u = reinterpret_cast<const unsigned char*>(value.s.c_str());
            const unsigned char* end_u = u + value.s.size();
            while (u < end_u) {
                uint32_t cp;
                if (*u < 0x80)        { cp = *u++; }
                else if (*u < 0xE0)   { cp = (*u++ & 0x1F) << 6;  cp |= (*u++ & 0x3F); }
                else if (*u < 0xF0)   { cp = (*u++ & 0x0F) << 12; cp |= (*u++ & 0x3F) << 6; cp |= (*u++ & 0x3F); }
                else                  { cp = (*u++ & 0x07) << 18; cp |= (*u++ & 0x3F) << 12; cp |= (*u++ & 0x3F) << 6; cp |= (*u++ & 0x3F); }
                ws += static_cast<wchar_t>(cp);
            }
            return dd->set_wstring_value(id, ws);
        }
        case FddsJsonValue::Kind::BOOL:
            return dd->set_boolean_value(id, value.b);
        case FddsJsonValue::Kind::DOUBLE:
        {
            DDS::ReturnCode_t ret = dd->set_float64_value(id, value.d);
            if (ret == DDS::RETCODE_OK) return ret;
            ret = dd->set_float32_value(id, static_cast<float>(value.d));
            if (ret == DDS::RETCODE_OK) return ret;
            return dd->set_float128_value(id, static_cast<long double>(value.d));
        }
        case FddsJsonValue::Kind::INT:
        {
            DDS::ReturnCode_t ret = dd->set_int32_value(id, static_cast<int32_t>(value.i));
            if (ret == DDS::RETCODE_OK) return ret;
            ret = dd->set_uint32_value(id, static_cast<uint32_t>(value.i));
            if (ret == DDS::RETCODE_OK) return ret;
            ret = dd->set_int64_value(id, static_cast<int64_t>(value.i));
            if (ret == DDS::RETCODE_OK) return ret;
            ret = dd->set_uint64_value(id, static_cast<uint64_t>(value.i));
            if (ret == DDS::RETCODE_OK) return ret;
            ret = dd->set_int16_value(id, static_cast<int16_t>(value.i));
            if (ret == DDS::RETCODE_OK) return ret;
            ret = dd->set_uint16_value(id, static_cast<uint16_t>(value.i));
            if (ret == DDS::RETCODE_OK) return ret;
            ret = dd->set_int8_value(id, static_cast<int8_t>(value.i));
            if (ret == DDS::RETCODE_OK) return ret;
            ret = dd->set_uint8_value(id, static_cast<uint8_t>(value.i));
            if (ret == DDS::RETCODE_OK) return ret;
            ret = dd->set_float64_value(id, static_cast<double>(value.i));
            if (ret == DDS::RETCODE_OK) return ret;
            return dd->set_boolean_value(id, value.i != 0);
        }
        case FddsJsonValue::Kind::NIL:
        default:
            return DDS::RETCODE_OK;
    }
}

/* init_data */
inline DDS::ReturnCode_t init_data(
    DDS::DynamicData* dd,
    const char*       data_folder,
    const char*       data_file)
{
    if (!dd || (!data_folder && !data_file)) return DDS::RETCODE_BAD_PARAMETER;

    std::string json_path = std::string(data_folder ? data_folder : "")
                          + "/json/"
                          + std::string(data_file ? data_file : "")
                          + ".json";

    std::ifstream f(json_path);
    if (!f) {
        fprintf(stderr, "init_data: cannot open '%s'\n", json_path.c_str());
        return DDS::RETCODE_ERROR;
    }
    std::string content((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());

    std::vector<std::pair<std::string,FddsJsonValue>> fields;
    if (!_fdds_parse_json(content, fields)) {
        fprintf(stderr, "init_data: failed to parse JSON '%s'\n", json_path.c_str());
        return DDS::RETCODE_ERROR;
    }

    FddsJsonValue root;
    root.kind = FddsJsonValue::Kind::OBJECT;
    root.o = std::move(fields);
    return _fdds_set_json_object(dd, root);
}

/* print_data */
inline void print_data(DDS::DynamicData* dd)
{
    if (!dd || !g_dynamic_type_ref) return;
    uint32_t n = g_dynamic_type_ref->get_member_count();
    printf("{");
    bool first = true;
    for (uint32_t i = 0; i < n; ++i) {
        DDS::traits<DDS::DynamicTypeMember>::ref_type mbr;
        if (g_dynamic_type_ref->get_member_by_index(mbr, i) != DDS::RETCODE_OK) continue;
        DDS::MemberId id = mbr->get_id();
        std::string   nm = mbr->get_name().to_string();
        int32_t val = 0;
        if (dd->get_int32_value(val, id) == DDS::RETCODE_OK) {
            if (!first) printf(", ");
            printf("\"%s\": %d", nm.c_str(), val);
            first = false;
            continue;
        }
        uint32_t uval = 0;
        if (dd->get_uint32_value(uval, id) == DDS::RETCODE_OK) {
            if (!first) printf(", ");
            printf("\"%s\": %u", nm.c_str(), uval);
            first = false;
            continue;
        }
        int16_t i16 = 0;
        if (dd->get_int16_value(i16, id) == DDS::RETCODE_OK) {
            if (!first) printf(", ");
            printf("\"%s\": %d", nm.c_str(), (int)i16);
            first = false;
            continue;
        }
        uint16_t u16 = 0;
        if (dd->get_uint16_value(u16, id) == DDS::RETCODE_OK) {
            if (!first) printf(", ");
            printf("\"%s\": %u", nm.c_str(), (unsigned)u16);
            first = false;
            continue;
        }
        int8_t i8 = 0;
        if (dd->get_int8_value(i8, id) == DDS::RETCODE_OK) {
            if (!first) printf(", ");
            printf("\"%s\": %d", nm.c_str(), (int)i8);
            first = false;
            continue;
        }
        uint8_t u8 = 0;
        if (dd->get_uint8_value(u8, id) == DDS::RETCODE_OK) {
            if (!first) printf(", ");
            printf("\"%s\": %u", nm.c_str(), (unsigned)u8);
            first = false;
            continue;
        }
        int64_t i64 = 0;
        if (dd->get_int64_value(i64, id) == DDS::RETCODE_OK) {
            if (!first) printf(", ");
            printf("\"%s\": %lld", nm.c_str(), (long long)i64);
            first = false;
            continue;
        }
        std::string str;
        if (dd->get_string_value(str, id) == DDS::RETCODE_OK) {
            if (!first) printf(", ");
            printf("\"%s\": \"%s\"", nm.c_str(), str.c_str());
            first = false;
            continue;
        }
        std::wstring wstr;
        if (dd->get_wstring_value(wstr, id) == DDS::RETCODE_OK) {
            if (!first) printf(", ");
            /* Convert wstring to UTF-8 for printing */
            std::string utf8;
            for (wchar_t wc : wstr) {
                uint32_t cp = static_cast<uint32_t>(wc);
                if (cp < 0x80) {
                    utf8 += static_cast<char>(cp);
                } else if (cp < 0x800) {
                    utf8 += static_cast<char>(0xC0 | (cp >> 6));
                    utf8 += static_cast<char>(0x80 | (cp & 0x3F));
                } else if (cp < 0x10000) {
                    utf8 += static_cast<char>(0xE0 | (cp >> 12));
                    utf8 += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
                    utf8 += static_cast<char>(0x80 | (cp & 0x3F));
                } else {
                    utf8 += static_cast<char>(0xF0 | (cp >> 18));
                    utf8 += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
                    utf8 += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
                    utf8 += static_cast<char>(0x80 | (cp & 0x3F));
                }
            }
            printf("\"%s\": \"%s\"", nm.c_str(), utf8.c_str());
            first = false;
            continue;
        }
        bool b = false;
        if (dd->get_boolean_value(b, id) == DDS::RETCODE_OK) {
            if (!first) printf(", ");
            printf("\"%s\": %s", nm.c_str(), b ? "true" : "false");
            first = false;
            continue;
        }
        double d = 0.0;
        if (dd->get_float64_value(d, id) == DDS::RETCODE_OK) {
            if (!first) printf(", ");
            printf("\"%s\": %g", nm.c_str(), d);
            first = false;
        }
    }
    printf("}\n");
}

/* check_data */
inline bool check_data(
    DDS::DynamicData* dd,
    const char*       data_folder,
    const char*       data_file)
{
    if (!dd || (!data_folder && !data_file) || !g_dynamic_type_ref) return false;

    auto ref = DDS::DynamicDataFactory::get_instance()->create_data(g_dynamic_type_ref);
    if (!ref) return false;
    if (init_data(ref.get(), data_folder, data_file) != DDS::RETCODE_OK)
        return false;

    return dd->equals(ref);
}

/* cleanup_data */
inline void cleanup_data(DDS::DynamicData* /*dd*/)
{
    g_publisher_data_ref.reset();
}

/* ── fdds_write: DynamicPubSubType expects a pointer to the DynamicData ref ─ */
inline DDS::ReturnCode_t fdds_write(
    DDS::DataWriter* writer,
    DDS::DynamicData* /*data*/,
    const DDS::InstanceHandle_t& handle)
{
    return writer->write(&g_publisher_data_ref, handle);
}

/* fdds_take: wraps take_next_sample into our DynamicDataSeq */
inline DDS::ReturnCode_t fdds_take(
    DDS::DataReader*     reader,
    DDS::DynamicDataSeq& samples,
    DDS::SampleInfoSeq&  /*infos*/)
{
    auto data = DDS::DynamicDataFactory::get_instance()->create_data(g_dynamic_type_ref);
    DDS::SampleInfo info;
    DDS::ReturnCode_t ret = reader->take_next_sample(&data, &info);
    if (ret == DDS::RETCODE_OK) {
        samples._data.push_back(data);
        samples._infos.push_back(info);
    }
    return ret;
}
