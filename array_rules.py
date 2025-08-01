from rtps_test_utilities import ReturnCode
import test_suite_functions as tsf

# In the notes, T1 refers to reader's type and T2 refers to writer's type

# TEST NAME (unique) : [ xml_filename, [PARAMS_1,  PARAMS_2, ... ],    [ EXPECTED OUTPUT_1, EXPECTED_OUTPUT_2, ... ] ]


xtypes_v2_array_rules = {
    'int32[10]' : {
        'common_args' : ['-X xml/types/arrays.xml'],
        'apps' : ['pub-exe -P -t test -y Test::int32x10 -V xml/data/arrays.xml -J json/data/array_num_10.json',
                  'sub-exe -S -t test -y Test::int32x10 -V xml/data/arrays.xml -J json/data/array_num_10.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : 'Communication between int32 arrays of the same size',
        'description' : ''
    },
    'int32[10]_int32[20]' : {
        'common_args' : ['-X xml/types/arrays.xml'],
        'apps' : ['pub-exe -P -t test -y Test::int32x10 -V xml/data/arrays.xml -J json/data/array_num_10.json',
                  'sub-exe -S -t test -y Test::int32x20 -V xml/data/arrays.xml -J json/data/array_num_20.json'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : 'No communication between int32 arrays of different sizes (publisher bigger)',
        'description' : ''
    },
    'int32[20]_int32[10]' : {
        'common_args' : ['-X xml/types/arrays.xml'],
        'apps' : ['pub-exe -P -t test -y Test::int32x10 -V xml/data/arrays.xml -J json/data/array_num_10.json',
                  'sub-exe -S -t test -y Test::int32x20 -V xml/data/arrays.xml -J json/data/array_num_20.json'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : 'No communication between int32 arrays of different sizes (subscriber bigger)',
        'description' : ''
    },
    'int32[10]_uint32[10]' : {
        'common_args' : ['-X xml/types/arrays.xml'],
        'apps' : ['pub-exe -P -t test -y Test::int32x10 -V xml/data/arrays.xml -J json/data/array_num_10.json',
                  'sub-exe -S -t test -y Test::uint32x10 -V xml/data/arrays.xml -J json/data/array_num_10.json'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : 'No communication between arrays of different types',
        'description' : ''
    },
    'int32[10][2]_int32[20]' : {
        'common_args' : ['-X xml/types/arrays.xml'],
        'apps' : ['pub-exe -P -t test -y Test::int32x10x2 -V xml/data/arrays.xml -J json/data/array_num_20.json',
                  'sub-exe -S -t test -y Test::int32x20 -V xml/data/arrays.xml -J json/data/array_num_20.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : 'No communication between arrays of different types',
        'description' : ''
    },
    'string10[10]_string20[10]' : {
        'common_args' : ['-X xml/types/arrays.xml'],
        'apps' : ['pub-exe -P -t test -y Test::string10x10 -V xml/data/arrays.xml -J json/data/array_string_10.json',
                  'sub-exe -S -t test -y Test::string20x10 -V xml/data/arrays.xml -J json/data/array_string_10.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'enum1[10]_enum2[10]' : {
        'common_args' : ['-X xml/types/arrays.xml'],
        'apps' : ['pub-exe -P -t test -y Test::enum1x10 -V xml/data/arrays.xml -J json/data/array_enum_10.json',
                  'sub-exe -S -t test -y Test::enum2x10 -V xml/data/arrays.xml -J json/data/array_enum_10.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'check' : {
        'common_args' : ['-X xml/types/arrays.xml'],
        'apps' : ['pub-exe -P -t test -y Test::enum1 -V xml/data/arrays.xml -J json/data/enum.json',
                  'sub-exe -S -t test -y Test::enum2 -V xml/data/arrays.xml -J json/data/enum.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'SFinal[10]_SFinalAlt[10]' : {
        'common_args' : ['-X xml/types/arrays.xml --ignore-member-names f'],
        'apps' : ['pub-exe -P -t test -y Test::sfinalx10 -V xml/data/arrays.xml -J json/data/array_array_num_10_20.json',
                  'sub-exe -S -t test -y Test::sfinalx10_alt -V xml/data/arrays.xml -J json/data/array_array_num_10_20_alt.json'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'SAppendable[10]_SAppendableAlt[10]' : {
        'common_args' : ['-X xml/types/arrays.xml'],
        'apps' : ['pub-exe -P -t test -y Test::sappendablex10 -V xml/data/arrays.xml -J json/data/array_array_num_10_20.json',
                  'sub-exe -S -t test -y Test::sappendablex10_alt -V xml/data/arrays.xml -J json/data/array_array_num_10_20_alt.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    # # basic array (strongly assignable element type) with various dimensions
    # 'int32[10]'                           : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::int32x10',       '-S -y Test::int32x10'],               [ReturnCode.OK, ReturnCode.OK] ],
    # 'int32[10]_int32[20]'                 : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::int32x10',       '-S -y Test::int32x20'],               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'int32[20]_int32[10]'                 : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::int32x20',       '-S -y Test::int32x10'],               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'int32[10]_uint32[10]'                : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::int32x20',       '-S -y Test::uint32x10'],              [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # # how does one verify 'bounds[] == bounds[]' (total count? dimension by dimension?)
    # 'int32[10][2]_int32[20]'              : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::int32x10x2',     '-S -y Test::int32x20'],               [ReturnCode.OK, ReturnCode.OK] ],

    # # some more arrays with 'strongly assignable' element types
    # 'string10[10]_string20[10]'           : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::string10x10',    '-S -y Test::string20x10'],            [ReturnCode.OK, ReturnCode.OK] ],

    # # enums
    # 'enum1[10]_enum2[10]'                 : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::enum1x10',       '-S -y Test::enum2x10'],               [ReturnCode.OK, ReturnCode.OK] ],
    # 'check'                               : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::enum1',          '-S -y Test::enum2'],                  [ReturnCode.OK, ReturnCode.OK] ],

    # # array with '!strongly_assignable' element type
    # 'SFinal[10]_SFinalAlt[10]'            : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::sfinalx10',      '-S -y Test::sfinalx10_alt'],          [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # # - struct element, appendable --> strongly assignable
    # 'SAppendable[10]_SAppendableAlt[10]'  : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::sappendablex10', '-S -y Test::sappendablex10_alt'],     [ReturnCode.OK, ReturnCode.OK] ],

}
