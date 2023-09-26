from test_util import ReturnCode, log_message

# In the notes, T1 refers to reader's type and T2 refers to writer's type

# TEST NAME (unique) : [ xml_filename, [PARAMS_1,  PARAMS_2, ... ],    [ EXPECTED OUTPUT_1, EXPECTED_OUTPUT_2, ... ] ]
    

xtypes_v2_array_rules = {

    # basic array (strongly assignable element type) with various dimensions
    'int32[10]'                           : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::int32x10',       '-S -y Test::int32x10'],               [ReturnCode.OK, ReturnCode.OK] ],
    'int32[10]_int32[20]'                 : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::int32x10',       '-S -y Test::int32x20'],               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'int32[20]_int32[10]'                 : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::int32x20',       '-S -y Test::int32x10'],               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'int32[10]_uint32[10]'                : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::int32x20',       '-S -y Test::uint32x10'],              [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # how does one verify 'bounds[] == bounds[]' (total count? dimension by dimension?)
    'int32[10][2]_int32[20]'              : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::int32x10x2',     '-S -y Test::int32x20'],               [ReturnCode.OK, ReturnCode.OK] ],

    # some more arrays with 'strongly assignable' element types
    'string10[10]_string20[10]'           : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::string10x10',    '-S -y Test::string20x10'],            [ReturnCode.OK, ReturnCode.OK] ],

    # enums
    'enum1[10]_enum2[10]'                 : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::enum1x10',       '-S -y Test::enum2x10'],               [ReturnCode.OK, ReturnCode.OK] ],
    'check'                               : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::enum1',          '-S -y Test::enum2'],                  [ReturnCode.OK, ReturnCode.OK] ],
    
    # array with '!strongly_assignable' element type
    'SFinal[10]_SFinalAlt[10]'            : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::sfinalx10',      '-S -y Test::sfinalx10_alt'],          [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # - struct element, appendable --> strongly assignable
    'SAppendable[10]_SAppendableAlt[10]'  : [ 'xml/types/arrays.xml', 'xml/data/arrays.xml', ['-P -y Test::sappendablex10', '-S -y Test::sappendablex10_alt'],     [ReturnCode.OK, ReturnCode.OK] ],
    
}
