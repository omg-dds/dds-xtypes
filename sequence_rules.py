from test_util import ReturnCode, log_message

# In the notes, T1 refers to reader's type and T2 refers to writer's type

# TEST NAME (unique) : [ xml_types_file, xml_data_file, [PARAMS_1,  PARAMS_2, ... ],    [ EXPECTED OUTPUT_1, EXPECTED_OUTPUT_2, ... ] ]
    

xtypes_v2_sequence_rules = {

    # basic array (strongly assignable element type) with various dimensions
    'seq(int32)_seq(int32,10)'                   : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::int32',           '-S -y Test::int32x10'],                           [ReturnCode.OK, ReturnCode.OK] ],
    'seq(int32)_seq(int32,10)_check_bounds'      : [ 'xml/types/sequences.xml', 'xml/sequences.xml',      ['-P -y Test::int32',           '-S -y Test::int32x10 --check-seq-bounds'],        [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    'seq(int32,20)_seq(int32,10)'                : [ 'xml/types/sequences.xml', 'xml/sequences.xml',      ['-P -y Test::int32x20',        '-S -y Test::int32x10'],                           [ReturnCode.OK, ReturnCode.OK] ],
    'seq(int32,20)_seq(int32,10)_check_bounds'   : [ 'xml/types/sequences.xml', 'xml/sequences.xml',      ['-P -y Test::int32x20',        '-S -y Test::int32x10 --check-seq-bounds'],        [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    'seq(int32,10)_seq(int32,20)'                : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::int32x10',        '-S -y Test::int32x20'],                           [ReturnCode.OK, ReturnCode.OK] ],
    'seq(int32,10)_seq(int32,20)_check_bounds'   : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::int32x10',        '-S -y Test::int32x20 --check-seq-bounds'],        [ReturnCode.OK, ReturnCode.OK] ],
    
    # some more sequences with 'strongly assignable' element types
    'seq(str10,10)_seq(str20,10)'                : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::string10x10',     '-S -y Test::string20x10'],                        [ReturnCode.OK, ReturnCode.OK] ],
    'seq(str20,10)_seq(str10,10)'                : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::string20x10',     '-S -y Test::string10x10'],                        [ReturnCode.OK, ReturnCode.OK] ],
    'seq(str20,10)_seq(str10,10)_check'          : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::string20x10',     '-S -y Test::string10x10 --check-str-bounds'],     [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # seq<enum>
    'seq(enum1)_seq(enum2)'                      : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::enum1',           '-S -y Test::enum2'],                              [ReturnCode.OK, ReturnCode.OK] ],
    
    # seq with '!strongly_assignable' element type
    'seq(SFinal,10)_seq(SFinalAlt,10)'           : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::sfinalx10',       '-S -y Test::sfinalx10_alt'],                      [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # - struct element, appendable --> strongly assignable
    'seq(SAppendable,10)_seq(SAppendableAlt,10)' : [ 'xml/types/sequences.xml', 'xml/data/sequences.xml', ['-P -y Test::sappendablex10',  '-S -y Test::sappendablex10_alt'],                 [ReturnCode.OK, ReturnCode.OK] ],

}
    
