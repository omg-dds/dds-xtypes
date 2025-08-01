from rtps_test_utilities import ReturnCode
import test_suite_functions as tsf

# TEST NAME (unique) : [ type_filename, data_filename, [ 'PARAMS_1',  'PARAMS_2', ... ],    [ EXPECTED OUTPUT_1, EXPECTED_OUTPUT_2, ... ] ]



xtypes_v2_primitive_test_suite = {

    'struct_uint8_uint8' : {
        'common_args' : ['-X xml/types/primitives.xml'],
        'apps' : ['pub-exe -P -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                  'sub-exe -S -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'struct_uint8_uint16' : {
        'common_args' : ['-X xml/types/primitives.xml'],
        'apps' : ['pub-exe -P -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                  'sub-exe -S -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'struct_uint8_uint32' : {
        'common_args' : ['-X xml/types/primitives.xml'],
        'apps' : ['pub-exe -P -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                  'sub-exe -S -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'struct_uint8_uint64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint8_int8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint8_int16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint8_int32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint8_int64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint8_float32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint8_float64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint8_float128': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint16_uint8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint16_uint16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint16_uint32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint16_uint64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint16_int8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint16_int16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint16_int32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint16_int64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint16_float32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint16_float64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint16_float128': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint32_uint8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint32_uint16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint32_uint32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint32_uint64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint32_int8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint32_int16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint32_int32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint32_int64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint32_float32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint32_float64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint32_float128': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint64_uint8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint64_uint16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint64_uint32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint64_uint64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint64_int8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint64_int16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint64_int32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint64_int64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint64_float32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint64_float64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_uint64_float128': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int8_uint8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int8_uint16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int8_uint32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int8_uint64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int8_int8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int8_int16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int8_int32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int8_int64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int8_float32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int8_float64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int8_float128': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int16_uint8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int16_uint16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int16_uint32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int16_uint64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int16_int8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int16_int16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int16_int32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int16_int64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int16_float32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int16_float64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int16_float128': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int32_uint8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int32_uint16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int32_uint32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int32_uint64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int32_int8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int32_int16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int32_int32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int32_int64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int32_float32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int32_float64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int32_float128': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int64_uint8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int64_uint16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int64_uint32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int64_uint64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int64_int8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int64_int16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int64_int32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int64_int64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int64_float32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int64_float64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_int64_float128': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float32_uint8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float32_uint16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float32_uint32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float32_uint64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float32_int8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float32_int16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float32_int32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float32_int64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float32_float32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float32_float64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float32_float128': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float64_uint8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float64_uint16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float64_uint32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float64_uint64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float64_int8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float64_int16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float64_int32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float64_int64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float64_float32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float64_float64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float64_float128': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float128_uint8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float128_uint16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float128_uint32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float128_uint64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_uint64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float128_int8': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int8 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float128_int16': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int16 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float128_int32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float128_int64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_int64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float128_float32': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float32 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float128_float64': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float64 -V xml/data/struct_primitives.xml -J json/data/struct_num_x1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'struct_float128_float128': {
        'common_args': ['-X xml/types/primitives.xml'],
        'apps': ['pub-exe -P -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json',
                 'sub-exe -S -t test -y Test::struct_primitive_float128 -V xml/data/struct_primitives.xml -J json/data/struct_float128_1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    }


    # # PRIMITIVES - struct members not assignable (various combinations)
    # 'struct_uint8_uint8'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint8', '-S -y Test::struct_primitive_uint8'     ], [ReturnCode.OK, ReturnCode.OK ] ],
    # 'struct_uint8_uint16' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint8', '-S -y Test::struct_primitive_uint16'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint8_uint32' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint8', '-S -y Test::struct_primitive_uint32'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint8_uint64' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint8', '-S -y Test::struct_primitive_uint64'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint8_int8'   :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint8', '-S -y Test::struct_primitive_int8'      ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint8_int16'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint8', '-S -y Test::struct_primitive_int16'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint8_int32'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint8', '-S -y Test::struct_primitive_int32'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint8_int64'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint8', '-S -y Test::struct_primitive_int64'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint8_float32':   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint8', '-S -y Test::struct_primitive_float32'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint8_float64':   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint8', '-S -y Test::struct_primitive_float64'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint8_float128':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint8', '-S -y Test::struct_primitive_float128'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # 'struct_uint16_uint8'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint16', '-S -y Test::struct_primitive_uint8'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint16_uint16' :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint16', '-S -y Test::struct_primitive_uint16'   ], [ReturnCode.OK, ReturnCode.OK] ],
    # 'struct_uint16_uint32' :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint16', '-S -y Test::struct_primitive_uint32'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint16_uint64' :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint16', '-S -y Test::struct_primitive_uint64'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint16_int8'   :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint16', '-S -y Test::struct_primitive_int8'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint16_int16'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint16', '-S -y Test::struct_primitive_int16'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint16_int32'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint16', '-S -y Test::struct_primitive_int32'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint16_int64'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint16', '-S -y Test::struct_primitive_int64'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint16_float32':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint16', '-S -y Test::struct_primitive_float32'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint16_float64':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint16', '-S -y Test::struct_primitive_float64'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint16_float128': [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint16', '-S -y Test::struct_primitive_float128' ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # 'struct_uint32_uint8'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint32', '-S -y Test::struct_primitive_uint8'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint32_uint16' :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint32', '-S -y Test::struct_primitive_uint16'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint32_uint32' :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint32', '-S -y Test::struct_primitive_uint32'   ], [ReturnCode.OK, ReturnCode.OK] ],
    # 'struct_uint32_uint64' :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint32', '-S -y Test::struct_primitive_uint64'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint32_int8'   :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint32', '-S -y Test::struct_primitive_int8'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint32_int16'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint32', '-S -y Test::struct_primitive_int16'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint32_int32'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint32', '-S -y Test::struct_primitive_int32'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint32_int64'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint32', '-S -y Test::struct_primitive_int64'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint32_float32':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint32', '-S -y Test::struct_primitive_float32'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint32_float64':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint32', '-S -y Test::struct_primitive_float64'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint32_float128': [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint32', '-S -y Test::struct_primitive_float128' ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # 'struct_uint64_uint8'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint64', '-S -y Test::struct_primitive_uint8'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint64_uint16' :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint64', '-S -y Test::struct_primitive_uint16'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint64_uint32' :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint64', '-S -y Test::struct_primitive_uint32'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint64_uint64' :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint64', '-S -y Test::struct_primitive_uint64'   ], [ReturnCode.OK, ReturnCode.OK] ],
    # 'struct_uint64_int8'   :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint64', '-S -y Test::struct_primitive_int8'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint64_int16'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint64', '-S -y Test::struct_primitive_int16'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint64_int32'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint64', '-S -y Test::struct_primitive_int32'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint64_int64'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint64', '-S -y Test::struct_primitive_int64'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint64_float32':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint64', '-S -y Test::struct_primitive_float32'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint64_float64':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint64', '-S -y Test::struct_primitive_float64'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_uint64_float128': [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_uint64', '-S -y Test::struct_primitive_float128' ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # 'struct_int8_uint8'  :    [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int8', '-S -y Test::struct_primitive_uint8'      ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int8_uint16' :    [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int8', '-S -y Test::struct_primitive_uint16'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int8_uint32' :    [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int8', '-S -y Test::struct_primitive_uint32'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int8_uint64' :    [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int8', '-S -y Test::struct_primitive_uint64'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int8_int8'   :    [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int8', '-S -y Test::struct_primitive_int8'       ], [ReturnCode.OK, ReturnCode.OK] ],
    # 'struct_int8_int16'  :    [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int8', '-S -y Test::struct_primitive_int16'      ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int8_int32'  :    [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int8', '-S -y Test::struct_primitive_int32'      ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int8_int64'  :    [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int8', '-S -y Test::struct_primitive_int64'      ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int8_float32':    [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int8', '-S -y Test::struct_primitive_float32'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int8_float64':    [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int8', '-S -y Test::struct_primitive_float64'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int8_float128':   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int8', '-S -y Test::struct_primitive_float128'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # 'struct_int16_uint8'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int16', '-S -y Test::struct_primitive_uint8'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int16_uint16' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int16', '-S -y Test::struct_primitive_uint16'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int16_uint32' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int16', '-S -y Test::struct_primitive_uint32'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int16_uint64' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int16', '-S -y Test::struct_primitive_uint64'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int16_int8'   :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int16', '-S -y Test::struct_primitive_int8'      ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int16_int16'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int16', '-S -y Test::struct_primitive_int16'     ], [ReturnCode.OK, ReturnCode.OK] ],
    # 'struct_int16_int32'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int16', '-S -y Test::struct_primitive_int32'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int16_int64'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int16', '-S -y Test::struct_primitive_int64'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int16_float32':   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int16', '-S -y Test::struct_primitive_float32'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int16_float64':   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int16', '-S -y Test::struct_primitive_float64'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int16_float128':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int16', '-S -y Test::struct_primitive_float128'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # 'struct_int32_uint8'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int32', '-S -y Test::struct_primitive_uint8'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int32_uint16' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int32', '-S -y Test::struct_primitive_uint16'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int32_uint32' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int32', '-S -y Test::struct_primitive_uint32'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int32_uint64' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int32', '-S -y Test::struct_primitive_uint64'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int32_int8'   :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int32', '-S -y Test::struct_primitive_int8'      ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int32_int16'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int32', '-S -y Test::struct_primitive_int16'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int32_int32'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int32', '-S -y Test::struct_primitive_int32'     ], [ReturnCode.OK, ReturnCode.OK] ],
    # 'struct_int32_int64'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int32', '-S -y Test::struct_primitive_int64'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int32_float32':   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int32', '-S -y Test::struct_primitive_float32'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int32_float64':   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int32', '-S -y Test::struct_primitive_float64'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int32_float128':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int32', '-S -y Test::struct_primitive_float128'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # 'struct_int64_uint8'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int64', '-S -y Test::struct_primitive_uint8'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int64_uint16' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int64', '-S -y Test::struct_primitive_uint16'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int64_uint32' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int64', '-S -y Test::struct_primitive_uint32'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int64_uint64' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int64', '-S -y Test::struct_primitive_uint64'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int64_int8'   :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int64', '-S -y Test::struct_primitive_int8'      ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int64_int16'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int64', '-S -y Test::struct_primitive_int16'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int64_int32'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int64', '-S -y Test::struct_primitive_int32'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int64_int64'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int64', '-S -y Test::struct_primitive_int64'     ], [ReturnCode.OK, ReturnCode.OK] ],
    # 'struct_int64_float32':   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int64', '-S -y Test::struct_primitive_float32'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int64_float64':   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int64', '-S -y Test::struct_primitive_float64'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_int64_float128':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_int64', '-S -y Test::struct_primitive_float128'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # 'struct_float32_uint8'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float32', '-S -y Test::struct_primitive_uint8'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float32_uint16' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float32', '-S -y Test::struct_primitive_uint16'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float32_uint32' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float32', '-S -y Test::struct_primitive_uint32'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float32_uint64' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float32', '-S -y Test::struct_primitive_uint64'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float32_int8'   :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float32', '-S -y Test::struct_primitive_int8'      ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float32_int16'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float32', '-S -y Test::struct_primitive_int16'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float32_int32'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float32', '-S -y Test::struct_primitive_int32'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float32_int64'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float32', '-S -y Test::struct_primitive_int64'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float32_float32':   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float32', '-S -y Test::struct_primitive_float32'   ], [ReturnCode.OK, ReturnCode.OK] ],
    # 'struct_float32_float64':   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float32', '-S -y Test::struct_primitive_float64'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float32_float128':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float32', '-S -y Test::struct_primitive_float128'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # 'struct_float64_uint8'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float64', '-S -y Test::struct_primitive_uint8'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float64_uint16' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float64', '-S -y Test::struct_primitive_uint16'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float64_uint32' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float64', '-S -y Test::struct_primitive_uint32'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float64_uint64' :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float64', '-S -y Test::struct_primitive_uint64'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float64_int8'   :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float64', '-S -y Test::struct_primitive_int8'      ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float64_int16'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float64', '-S -y Test::struct_primitive_int16'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float64_int32'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float64', '-S -y Test::struct_primitive_int32'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float64_int64'  :   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float64', '-S -y Test::struct_primitive_int64'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float64_float32':   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float64', '-S -y Test::struct_primitive_float32'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float64_float64':   [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float64', '-S -y Test::struct_primitive_float64'   ], [ReturnCode.OK, ReturnCode.OK] ],
    # 'struct_float64_float128':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float64', '-S -y Test::struct_primitive_float128'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # 'struct_float128_uint8'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float128', '-S -y Test::struct_primitive_uint8'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float128_uint16' :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float128', '-S -y Test::struct_primitive_uint16'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float128_uint32' :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float128', '-S -y Test::struct_primitive_uint32'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float128_uint64' :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float128', '-S -y Test::struct_primitive_uint64'   ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float128_int8'   :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float128', '-S -y Test::struct_primitive_int8'     ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float128_int16'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float128', '-S -y Test::struct_primitive_int16'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float128_int32'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float128', '-S -y Test::struct_primitive_int32'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float128_int64'  :  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float128', '-S -y Test::struct_primitive_int64'    ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float128_float32':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float128', '-S -y Test::struct_primitive_float32'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float128_float64':  [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float128', '-S -y Test::struct_primitive_float64'  ], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'struct_float128_float128': [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitive_float128', '-S -y Test::struct_primitive_float128' ], [ReturnCode.OK, ReturnCode.OK] ],
}

