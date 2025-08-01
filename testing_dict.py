#################################################################
# Use and redistribution is source and binary forms is permitted
# subject to the OMG-DDS INTEROPERABILITY TESTING LICENSE found
# at the following URL:
#
# https://github.com/omg-dds/dds-xtypes/blob/master/LICENSE.md
#
#################################################################

from rtps_test_utilities import ReturnCode
import test_suite_functions as tsf

rtps_test_suite_1 = {
    'PrimitiveAppendable' : {
        'common_args' : ['-X xml/types/primitives.xml -y Test::struct_primitives_appendable '
                         '-V xml/data/struct_primitives.xml -J json/data/struct_primitives.json'],
        'apps' : ['pub-exe -P -t Square',
                  'sub-exe -S -t Square'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : 'Communication using an appendable struct with primitive types',
        'description' : ''
    },
    'PrimitiveFinal' : {
        'common_args' : ['-X xml/types/primitives.xml -y Test::struct_primitives_appendable '
                         '-V xml/data/struct_primitives.xml -J json/data/struct_primitives.json'],
        'apps' : ['pub-exe -P -t Square',
                  'sub-exe -S -t Square'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : 'Communication using an final struct with primitive types',
        'description' : ''
    },
    'PrimitiveMutable' : {
        'common_args' : ['-X xml/types/primitives.xml -y Test::struct_primitives_appendable '
                         '-V xml/data/struct_primitives.xml -J json/data/struct_primitives.json'],
        'apps' : ['pub-exe -P -t Square',
                  'sub-exe -S -t Square'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : 'Communication using an mutable struct with primitive types',
        'description' : ''
    }
}

xtypes_v2_union_rules = {

    'union_primitives_final' : {
        'common_args' : ['-X xml/types/unions.xml'],
        'apps' : ['pub-exe -P -t test -y Test::union_primitives_final -V xml/data/union_primitive.xml -J json/data/union_primitive.json',
                  'sub-exe -S -t test -y Test::union_primitives_final -V xml/data/union_primitive.xml -J json/data/union_primitive.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'union_primitives_appendable': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_primitives_appendable -V xml/data/union_primitive.xml -J json/data/union_primitive.json',
                 'sub-exe -S -t test -y Test::union_primitives_appendable -V xml/data/union_primitive.xml -J json/data/union_primitive.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
}
