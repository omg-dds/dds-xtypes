from rtps_test_utilities import ReturnCode
import test_suite_functions as tsf

# In the notes, T1 refers to reader's type and T2 refers to writer's type

# TEST NAME (unique) : [ xml_filename, [PARAMS_1,  PARAMS_2, ... ],    [ EXPECTED OUTPUT_1, EXPECTED_OUTPUT_2, ... ] ]
    

xtypes_v2_string_rules = {
    'string_string' : {
        'common_args' : ['-X xml/types/strings.xml'],
        'apps' : ['pub-exe -P -t test -y Test::string -V xml/data/strings.xml -J json/data/strings.json',
                  'sub-exe -S -t test -y Test::string -V xml/data/strings.xml -J json/data/strings.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'string_string10' : {
        'common_args' : ['-X xml/types/strings.xml'],
        'apps' : ['pub-exe -P -t test -y Test::string -V xml/data/strings.xml -J json/data/strings.json',
                  'sub-exe -S -t test -y Test::string10 -V xml/data/strings.xml -J json/data/strings.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'string_string10_check' : {
        'common_args' : ['-X xml/types/strings.xml'],
        'apps' : ['pub-exe -P -t test -y Test::string -V xml/data/strings.xml -J json/data/strings.json',
                  'sub-exe -S -t test -y Test::string10 -V xml/data/strings.xml -J json/data/strings.json --ignore-str-bounds f'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'string10_string20_check' : {
        'common_args' : ['-X xml/types/strings.xml'],
        'apps' : ['pub-exe -P -t test -y Test::string10 -V xml/data/strings.xml -J json/data/strings.json',
                  'sub-exe -S -t test -y Test::string20 -V xml/data/strings.xml -J json/data/strings.json --ignore-str-bounds f'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'string10_string20_check' : {
        'common_args' : ['-X xml/types/strings.xml'],
        'apps' : ['pub-exe -P -t test -y Test::string20 -V xml/data/strings.xml -J json/data/strings.json',
                  'sub-exe -S -t test -y Test::string10 -V xml/data/strings.xml -J json/data/strings.json --ignore-str-bounds f'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },

    'wstring_wstring' : {
        'common_args' : ['-X xml/types/strings.xml'],
        'apps' : ['pub-exe -P -t test -y Test::wstring -V xml/data/wstrings.xml -J json/data/wstrings.json',
                  'sub-exe -S -t test -y Test::wstring -V xml/data/wstrings.xml -J json/data/wstrings.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'wstring_wstring10' : {
        'common_args' : ['-X xml/types/strings.xml'],
        'apps' : ['pub-exe -P -t test -y Test::wstring -V xml/data/wstrings.xml -J json/data/wstrings.json',
                  'sub-exe -S -t test -y Test::wstring10 -V xml/data/wstrings.xml -J json/data/wstrings.json'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'wstring_wstring10_check' : {
        'common_args' : ['-X xml/types/strings.xml'],
        'apps' : ['pub-exe -P -t test -y Test::wstring -V xml/data/wstrings.xml -J json/data/wstrings.json',
                  'sub-exe -S -t test -y Test::wstring10 -V xml/data/wstrings.xml -J json/data/wstrings.json --ignore-str-bounds f'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'wstring10_wstring20_check' : {
        'common_args' : ['-X xml/types/strings.xml'],
        'apps' : ['pub-exe -P -t test -y Test::wstring10 -V xml/data/wstrings.xml -J json/data/wstrings.json',
                  'sub-exe -S -t test -y Test::wstring20 -V xml/data/wstrings.xml -J json/data/wstrings.json --ignore-str-bounds f'],
        'expected_codes' : [ReturnCode.OK, ReturnCode.OK],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },
    'wstring10_wstring20_check' : {
        'common_args' : ['-X xml/types/strings.xml'],
        'apps' : ['pub-exe -P -t test -y Test::wstring20 -V xml/data/wstrings.xml -J json/data/wstrings.json',
                  'sub-exe -S -t test -y Test::wstring10 -V xml/data/wstrings.xml -J json/data/wstrings.json --ignore-str-bounds f'],
        'expected_codes' : [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function' : tsf.data_is_correct,
        'title' : '',
        'description' : ''
    },

    # # string to string
    # 'string_string'            : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::string',    '-S -y Test::string'],                                 [ReturnCode.OK, ReturnCode.OK] ],

    # # string to string10 [ ignore_string_bounds ]
    # 'string_string10'          : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::string',    '-S -y Test::string10'],                               [ReturnCode.OK, ReturnCode.OK] ],

    # # string to string10 [ check_string_bounds ]
    # 'string_string10_check'    : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::string',    '-S -y Test::string10 --check-str-bounds'],            [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # # string10 to string20 [ check_string_bounds ]
    # 'string10_string20'        : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::string10',  '-S -y Test::string20 --check-str-bounds'],            [ReturnCode.OK, ReturnCode.OK] ],

    # # string20 to string10 [ check_string_bounds ]
    # 'string20_string10'        : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::string20',  '-S -y Test::string10 --check-str-bounds'],            [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],


    # # wstring to wstring
    # 'wstring_wstring'          : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::wstring',    '-S -y Test::wstring'],                               [ReturnCode.OK, ReturnCode.OK] ],
    # # wstring to wstring10 [ ignore_wstring_bounds ]
    # 'wstring_wstring10'        : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::wstring',    '-S -y Test::wstring10'],                             [ReturnCode.OK, ReturnCode.OK] ],

    # # wstring to wstring10 [ check_wstring_bounds ]
    # 'wstring_wstring10_check'  : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::wstring',    '-S -y Test::wstring10 --check-str-bounds'],          [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # # wstring10 to wstring20 [ check_wstring_bounds ]
    # 'wstring10_wstring20'      : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::wstring10',  '-S -y Test::wstring20 --check-str-bounds'],          [ReturnCode.OK, ReturnCode.OK] ],

    # # wstring20 to wstring10 [ check_wstring_bounds ]
    # 'wstring20_wstring10'      : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::wstring20',  '-S -y Test::wstring10 --check-str-bounds'],          [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
}

