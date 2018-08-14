
//
//  SerializationData.h
//  xtypes-interop
//
//  Created by Kelvin Zhang on 6/2/18.
//

#include <shapes.h>
#include <initializer_list>
#include <vector>
#include "Utils.h"

namespace test_versions {
    struct T5_2_1 {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {};
    };
    struct T5_2_2 {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {PREVENT_TYPE_WIDENING};
    };
    struct T5_2_3 {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {PREVENT_TYPE_WIDENING};
    };
    struct T5_2_4 {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {PREVENT_TYPE_WIDENING};
    };
    struct T5_2_5 {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {};
    };
    struct T5_2_6 {
        const std::vector<QosProperty> set_to_one = {IGNORE_STRING_BOUNDS};
        const std::vector<QosProperty> set_to_zero = {IGNORE_SEQUENCE_BOUNDS};
    };
    struct T5_2_7 {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {};
    };
    struct T5_2_8 {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {};
    };
    struct T5_2_9 {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {};
    };
    struct T5_2_10 {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {};
    };
    struct T5_2_11 {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {};
    };
    struct T5_2_12 {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {};
    };
    struct T5_2_13_A {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {};
    };
    struct T5_2_13_B {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {};
    };
    struct T5_2_13_C {
        const std::vector<QosProperty> set_to_one = {};
        const std::vector<QosProperty> set_to_zero = {};
    };
};


template <typename T>
void fill_data(T &data, test_versions::T5_2_1)
{
    data.color = DDS_String_dup("RED");
    data.description = DDS_String_dup("RED SHAPE DESC");
    data.tags.maximum(1);
    data.tags.length(1);
    data.tags[0] = DDS_String_dup("Tag_1");
    data.points[0] = {0, 0};
    data.points[1] = {10, 15};
    data.points[2] = {20, 25};
    data.shapesize = 10;
}

struct general_ {};
struct special_ : general_ {};
template<typename> struct int_ { typedef int type; };

template <typename T, typename int_<decltype(T::angle)>::type = 0>
void T5_2_2_set_angle(T &data, double angle, struct special_)
{
    data.angle = angle;
}

template <typename T>
void T5_2_2_set_angle(T &data, double, struct general_)
{
    
}

template <typename T>
void fill_data(T &data, test_versions::T5_2_2)
{
    fill_data(data, test_versions::T5_2_1());
    T5_2_2_set_angle(data, 45.0, special_());
}

template <typename T, typename int_<decltype(T::z)>::type = 0>
void T5_2_3_set_z(T &data, int z, struct special_)
{
    data.z = z;
}

template <typename T>
void T5_2_3_set_z(T &data, int z, struct general_)
{
    
}

template <typename T>
void fill_data(T &data, test_versions::T5_2_3)
{
    fill_data(data, test_versions::T5_2_1());
    T5_2_3_set_z(data.points[0], 20, special_());
    T5_2_3_set_z(data.points[1], 45, special_());
    T5_2_3_set_z(data.points[2], 33, special_());
}

template <typename T>
void fill_data(T &data, test_versions::T5_2_4)
{
    fill_data(data, test_versions::T5_2_1());
    T5_2_2_set_angle(data, 22.0, special_());
}

template <typename T>
void fill_data(T &data, test_versions::T5_2_5)
{
    fill_data(data, test_versions::T5_2_1());
    T5_2_2_set_angle(data, 45.0, special_());
}

template <typename T>
void fill_data(T &data, test_versions::T5_2_6)
{
    fill_data(data, test_versions::T5_2_1());
}

template <typename T>
void fill_data(T &data, test_versions::T5_2_7)
{
    data.color = DDS_String_dup("RED");
    data.description = DDS_String_dup("RED SHAPE DESC");
    data.tags.maximum(3);
    data.tags.length(3);
    data.tags[0] = DDS_String_dup("Tag_1");
    data.tags[1] = DDS_String_dup("Tag_2");
    data.tags[2] = DDS_String_dup("Tag_3");
    data.points[0] = {0, 0};
    data.points[1] = {10, 15};
    data.points[2] = {20, 25};
    data.shapesize = 10;
}

template <typename T>
void fill_data(T &data, test_versions::T5_2_8)
{
    data._d = IntKind::USHORT_PARAM;
    data._u.val_us = 34;
}


template <typename T, typename int_<decltype(T::_u::val_l)>::type = 0>
void T5_2_9_set_union(T &data, int val, struct special_)
{
    data._d = IntKind::LONG_PARAM;
    data._u.val_l = val;
}

template <typename T>
void T5_2_9_set_union(T &data, int val, struct general_)
{
    data._d = IntKind::USHORT_PARAM;
    data._u.val_us = val;
}

template <typename T>
void fill_data(T &data, test_versions::T5_2_9)
{
    T5_2_9_set_union(data, 34, special_());
}


void T5_2_10_set_union(Parameter7Appendable &data);
void T5_2_10_set_union(Parameter7Mutable &data);
void T5_2_10_set_union(Parameter1Appendable &data);
void T5_2_10_set_union(Parameter1Mutable &data);

template <typename T>
void fill_data(T &data, test_versions::T5_2_10)
{
    T5_2_10_set_union(data);
}

template <typename T>
void fill_data(T &data, test_versions::T5_2_11)
{
    T5_2_9_set_union(data, 34, special_());
}

template <typename T>
void fill_data(T &data, test_versions::T5_2_12)
{
    data.color = DDS_String_dup("RED");
    data.description = NULL;
    data.tags.maximum(1);
    data.tags.length(1);
    data.tags[0] = DDS_String_dup("Tag_1");
    data.points[0] = {0, 0};
    data.points[1] = {10, 15};
    data.points[2] = {20, 25};
    data.shapesize = 10;
}

template <typename T>
void fill_data(T &data, test_versions::T5_2_13_A)
{
    data.color = DDS_String_dup("RED");
    data.description = NULL;
    data.tags.maximum(1);
    data.tags.length(1);
    data.tags[0] = DDS_String_dup("Tag_1");
    data.points[0] = {0, 0};
    data.points[1] = {10, 15};
    data.points[2] = {20, 25};
    data.shapesize = 10;
}

template <typename T>
void fill_data(T &data, test_versions::T5_2_13_B)
{
    data.color = DDS_String_dup("RED");
    data.description = DDS_String_dup("RED SHAPE DESC");
    data.tags.maximum(1);
    data.tags.length(1);
    data.tags[0] = DDS_String_dup("Tag_1");
    data.points[0] = {0, 0};
    data.points[1] = {10, 15};
    data.points[2] = {20, 25};
    data.shapesize = 10;
}

template <typename T>
void fill_data(T &data, test_versions::T5_2_13_C)
{
    fill_data(data, test_versions::T5_2_13_B());
}
