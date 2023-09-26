from test_util import ReturnCode, log_message

# In the notes, T1 refers to reader's type and T2 refers to writer's type

# TEST NAME (unique) : [ xml_filename, [PARAMS_1,  PARAMS_2, ... ],    [ EXPECTED OUTPUT_1, EXPECTED_OUTPUT_2, ... ] ]
    

xtypes_v2_string_rules = {
    # string to string
    'string_string'            : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::string',    '-S -y Test::string'],                                 [ReturnCode.OK, ReturnCode.OK] ],

    # string to string10 [ ignore_string_bounds ]
    'string_string10'          : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::string',    '-S -y Test::string10'],                               [ReturnCode.OK, ReturnCode.OK] ],
    
    # string to string10 [ check_string_bounds ]
    'string_string10_check'    : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::string',    '-S -y Test::string10 --check-str-bounds'],            [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # string10 to string20 [ check_string_bounds ]
    'string10_string20'        : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::string10',  '-S -y Test::string20 --check-str-bounds'],            [ReturnCode.OK, ReturnCode.OK] ],

    # string20 to string10 [ check_string_bounds ]
    'string20_string10'        : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::string20',  '-S -y Test::string10 --check-str-bounds'],            [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
 

    # wstring to wstring
    'wstring_wstring'          : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::wstring',    '-S -y Test::wstring'],                               [ReturnCode.OK, ReturnCode.OK] ],
    # wstring to wstring10 [ ignore_wstring_bounds ]
    'wstring_wstring10'        : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::wstring',    '-S -y Test::wstring10'],                             [ReturnCode.OK, ReturnCode.OK] ],
    
    # wstring to wstring10 [ check_wstring_bounds ]
    'wstring_wstring10_check'  : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::wstring',    '-S -y Test::wstring10 --check-str-bounds'],          [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # wstring10 to wstring20 [ check_wstring_bounds ]
    'wstring10_wstring20'      : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::wstring10',  '-S -y Test::wstring20 --check-str-bounds'],          [ReturnCode.OK, ReturnCode.OK] ],
  
    # wstring20 to wstring10 [ check_wstring_bounds ]
    'wstring20_wstring10'      : [ 'xml/types/strings.xml', 'xml/data/strings.xml', ['-P -y Test::wstring20',  '-S -y Test::wstring10 --check-str-bounds'],          [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
}
    
