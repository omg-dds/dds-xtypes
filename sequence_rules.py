from rtps_test_utilities import ReturnCode
import test_suite_functions as tsf

# In the notes, T1 refers to reader's type and T2 refers to writer's type

# TEST NAME (unique) : [ xml_types_file, xml_data_file, [PARAMS_1,  PARAMS_2, ... ],    [ EXPECTED OUTPUT_1, EXPECTED_OUTPUT_2, ... ] ]


xtypes_v2_sequence_rules = {

    'seq(int32)_seq(int32,10)' : {
        'common_args' : ['-X xml/types/sequences.xml'],
        'apps' : ['pub-exe -P -t test -y Test::int32 -V xml/data/sequences.xml -J json/data/array_num_10.json',
                  'sub-exe -S -t test -y Test::int32x10 -V xml/data/sequences.xml -J json/data/array_num_10.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'seq(int32)_seq(int32,10)_check_bounds' : {
        'common_args' : ['-X xml/types/sequences.xml'],
        'apps' : ['pub-exe -P -t test -y Test::int32 -V xml/data/sequences.xml -J json/data/array_num_10.json',
                  'sub-exe -S -t test -y Test::int32x10 -V xml/data/sequences.xml -J json/data/array_num_10.json --ignore-seq-bounds f'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'seq(int32,20)_seq(int32,10)' : {
        'common_args' : ['-X xml/types/sequences.xml'],
        'apps' : ['pub-exe -P -t test -y Test::int32x20 -V xml/data/sequences.xml -J json/data/array_num_20.json',
                  'sub-exe -S -t test -y Test::int32x10 -V xml/data/sequences.xml -J json/data/array_num_10.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'seq(int32,20)_seq(int32,10)_check_bounds' : {
        'common_args' : ['-X xml/types/sequences.xml'],
        'apps' : ['pub-exe -P -t test -y Test::int32x20 -V xml/data/sequences.xml -J json/data/array_num_20.json',
                  'sub-exe -S -t test -y Test::int32x10 -V xml/data/sequences.xml -J json/data/array_num_10.json --ignore-seq-bounds f'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'seq(int32,10)_seq(int32,20)' : {
        'common_args' : ['-X xml/types/sequences.xml'],
        'apps' : ['pub-exe -P -t test -y Test::int32x10 -V xml/data/sequences.xml -J json/data/array_num_10.json',
                  'sub-exe -S -t test -y Test::int32x20 -V xml/data/sequences.xml -J json/data/array_num_10.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'seq(int32,10)_seq(int32,20)_check_bounds' : {
        'common_args' : ['-X xml/types/sequences.xml'],
        'apps' : ['pub-exe -P -t test -y Test::int32x10 -V xml/data/sequences.xml -J json/data/array_num_10.json',
                  'sub-exe -S -t test -y Test::int32x20 -V xml/data/sequences.xml -J json/data/array_num_10.json --ignore-seq-bounds f'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'seq(str10,10)_seq(str20,10)' : {
        'common_args' : ['-X xml/types/sequences.xml'],
        'apps' : ['pub-exe -P -t test -y Test::string10x10 -V xml/data/sequences.xml -J json/data/array_string_10.json',
                  'sub-exe -S -t test -y Test::string20x10 -V xml/data/sequences.xml -J json/data/array_string_10.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'seq(str20,10)_seq(str10,10)' : {
        'common_args' : ['-X xml/types/sequences.xml'],
        'apps' : ['pub-exe -P -t test -y Test::string20x10 -V xml/data/sequences.xml -J json/data/array_string_10.json',
                  'sub-exe -S -t test -y Test::string10x10 -V xml/data/sequences.xml -J json/data/array_string_10.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'seq(str20,10)_seq(str10,10)_check' : {
        'common_args' : ['-X xml/types/sequences.xml'],
        'apps' : ['pub-exe -P -t test -y Test::string20x10 -V xml/data/sequences.xml -J json/data/array_string_10.json',
                  'sub-exe -S -t test -y Test::string10x10 -V xml/data/sequences.xml -J json/data/array_string_10.json --ignore-str-bounds f'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'seq(enum1)_seq(enum2)' : {
        'common_args' : ['-X xml/types/sequences.xml'],
        'apps' : ['pub-exe -P -t test -y Test::enum1 -V xml/data/sequences.xml -J json/data/array_enum_10.json',
                  'sub-exe -S -t test -y Test::enum2 -V xml/data/sequences.xml -J json/data/array_enum_10.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'seq(SFinal,10)_seq(SFinalAlt,10)' : {
        'common_args' : ['-X xml/types/sequences.xml'],
        'apps' : ['pub-exe -P -t test -y Test::sfinalx10 -V xml/data/sequences.xml -J json/data/array_array_num_10_20.json',
                  'sub-exe -S -t test -y Test::sfinalx10_alt -V xml/data/sequences.xml -J json/data/array_array_num_10_20_alt.json'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'seq(SAppendable,10)_seq(SAppendableAlt,10)' : {
        'common_args' : ['-X xml/types/sequences.xml'],
        'apps' : ['pub-exe -P -t test -y Test::sappendablex10 -V xml/data/sequences.xml -J json/data/array_array_num_10_20.json',
                  'sub-exe -S -t test -y Test::sappendablex10_alt -V xml/data/sequences.xml -J json/data/array_array_num_10_20_alt.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },

    # # basic array (strongly assignable element type) with various dimensions
    # 'seq(int32)_seq(int32,10)'                   : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::int32',           '-S -y Test::int32x10'],                           [ReturnCode.OK, ReturnCode.OK] ],
    # 'seq(int32)_seq(int32,10)_check_bounds'      : [ 'xml/types/sequences.xml', 'xml/sequences.xml',      ['-P -y Test::int32',           '-S -y Test::int32x10 --check-seq-bounds'],        [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # 'seq(int32,20)_seq(int32,10)'                : [ 'xml/types/sequences.xml', 'xml/sequences.xml',      ['-P -y Test::int32x20',        '-S -y Test::int32x10'],                           [ReturnCode.OK, ReturnCode.OK] ],
    # 'seq(int32,20)_seq(int32,10)_check_bounds'   : [ 'xml/types/sequences.xml', 'xml/sequences.xml',      ['-P -y Test::int32x20',        '-S -y Test::int32x10 --check-seq-bounds'],        [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # 'seq(int32,10)_seq(int32,20)'                : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::int32x10',        '-S -y Test::int32x20'],                           [ReturnCode.OK, ReturnCode.OK] ],
    # 'seq(int32,10)_seq(int32,20)_check_bounds'   : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::int32x10',        '-S -y Test::int32x20 --check-seq-bounds'],        [ReturnCode.OK, ReturnCode.OK] ],

    # # some more sequences with 'strongly assignable' element types
    # 'seq(str10,10)_seq(str20,10)'                : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::string10x10',     '-S -y Test::string20x10'],                        [ReturnCode.OK, ReturnCode.OK] ],
    # 'seq(str20,10)_seq(str10,10)'                : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::string20x10',     '-S -y Test::string10x10'],                        [ReturnCode.OK, ReturnCode.OK] ],
    # 'seq(str20,10)_seq(str10,10)_check'          : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::string20x10',     '-S -y Test::string10x10 --check-str-bounds'],     [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # # seq<enum>
    # 'seq(enum1)_seq(enum2)'                      : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::enum1',           '-S -y Test::enum2'],                              [ReturnCode.OK, ReturnCode.OK] ],

    # # seq with '!strongly_assignable' element type
    # 'seq(SFinal,10)_seq(SFinalAlt,10)'           : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::sfinalx10',       '-S -y Test::sfinalx10_alt'],                      [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # # - struct element, appendable --> strongly assignable
    # 'seq(SAppendable,10)_seq(SAppendableAlt,10)' : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::sappendablex10',  '-S -y Test::sappendablex10_alt'],                 [ReturnCode.OK, ReturnCode.OK] ],

}

