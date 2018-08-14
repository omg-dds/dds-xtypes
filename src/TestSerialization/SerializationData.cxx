
//
//  SerializationData.cxx
//  xtypes-interop
//
//  Created by Kelvin Zhang on 6/2/18.
//

#include "SerializationData.h"

void T5_2_10_set_union(Parameter7Appendable &data)
{
    data._d = LongIntKind::LONGLONG_PARAM;
    data._u.val_l = 128;
}

void T5_2_10_set_union(Parameter7Mutable &data)
{
    data._d = LongIntKind::LONGLONG_PARAM;
    data._u.val_l = 128;
}

void T5_2_10_set_union(Parameter1Appendable &data)
{
    data._d = IntKind::USHORT_PARAM;
    data._u.val_us = 34;
}

void T5_2_10_set_union(Parameter1Mutable &data)
{
    data._d = IntKind::USHORT_PARAM;
    data._u.val_us = 34;
}
