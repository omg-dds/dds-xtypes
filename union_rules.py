from test_util import ReturnCode, log_message

# In the notes, T1 refers to reader's type and T2 refers to writer's type

# TEST NAME (unique) : [ xml_filename, [PARAMS_1,  PARAMS_2, ... ],    [ EXPECTED OUTPUT_1, EXPECTED_OUTPUT_2, ... ] ]
    

xtypes_v2_union_rules = {

    # PRIMITIVES - struct primitive members assignable 
    'union_primitives_final'      : [ 'xml/types/unions.xml', 'xml/data/union_primitive.xml',   ['-P -y Test::union_primitives_final',      '-S -y Test::union_primitives_final'     ], [ReturnCode.OK, ReturnCode.OK] ],
    'union_primitives_appendable' : [ 'xml/types/unions.xml', 'xml/data/union_primitive.xml',   ['-P -y Test::union_primitives_appendable', '-S -y Test::union_primitives_appendable'], [ReturnCode.OK, ReturnCode.OK] ],
    'union_primitives_mutable'    : [ 'xml/types/unions.xml', 'xml/data/union_primitive.xml',   ['-P -y Test::union_primitives_mutable',    '-S -y Test::union_primitives_mutable'   ], [ReturnCode.OK, ReturnCode.OK] ],

    # EXTENSIBILITY MISMATCH
    'union_final_appendable'      : [ 'xml/types/unions.xml', 'xml/data/union_primitive.xml',   ['-P -y Test::union_primitives_final',      '-S -y Test::union_primitives_appendable'], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'union_final_mutable'         : [ 'xml/types/unions.xml', 'xml/data/union_primitive.xml',   ['-P -y Test::union_primitives_final',      '-S -y Test::union_primitives_mutable'],    [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'union_appendable_mutable'    : [ 'xml/types/unions.xml', 'xml/data/union_primitive.xml',   ['-P -y Test::union_primitives_appendable', '-S -y Test::union_primitives_mutable'],    [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # DISCRIMINATORs STRONGLY ASSIGNABLE
    'union_uint32_bitmask32'      : [ 'xml/types/unions.xml', 'xml/data/union_uint32.xml',   ['-P -y Test::union_uint32',                   '-S -y Test::union_bitmask32'],          [ReturnCode.OK, ReturnCode.OK] ],
    'union_uint32_bitmask16'      : [ 'xml/types/unions.xml', 'xml/data/union_uint32.xml',   ['-P -y Test::union_uint32',                   '-S -y Test::union_bitmask16'],          [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # DISCRIMINATOR KEY IN ONE BUT NOT OTHER
    'union_uint32_one_key'        : [ 'xml/types/unions.xml', 'xml/data/union_uint32.xml',   ['-P -y Test::union_uint32',                    '-S -y Test::union_uint32_key'],        [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # IF MEMBER NAME MATCHES, MEMBER ID MUST MATCH [can be bypassed with TypeConsistency.ignore_member_names] 
    'union_different_ids_ok'      : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_1',      '-S -y Test::union_2'],                                    [ReturnCode.OK, ReturnCode.OK] ],
    'union_different_ids'         : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_1',      '-S -y Test::union_2 --check-member-names'],               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # IF MEMBER ID MATCHES, MEMBER NAME MUST MATCH [can be bypassed with TypeConsistency.ignore_member_names]
    'union_different_names_ok'    : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_3',      '-S -y Test::union_4'],                                    [ReturnCode.OK, ReturnCode.OK] ],
    'union_different_names'       : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_3',      '-S -y Test::union_4 --check-member-names'],               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # LABELS IN T2 SELECT A T1 MEMBER, THEN T1.m1 IS ASSIGNABLE FROM T2's MEMBER
    'union_int16_int32'           : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_int16',      '-S -y Test::union_int32'],                            [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'union_int16_int32_default'   : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_int16',      '-S -y Test::union_int32_default'],                    [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'union_int32_int32_default'   : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_int32',      '-S -y Test::union_int32_default'],                    [ReturnCode.OK, ReturnCode.OK] ],

    # LABELS IN T1 SELECT 'default' in T2 THEN T1.m1 IS ASSIGNABLE FROM T2.default
    'union_int32_default_int16'   : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_int32_default',    '-S -y Test::union_int16'],                      [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'union_int32_default_int32'   : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_int32_default',    '-S -y Test::union_int32'],                      [ReturnCode.OK, ReturnCode.OK] ],
    
    # BOTH HAVE 'default' THEN T1.default IS ASSIGNABLE FROM T2.default
    'union_int32_default_int16_default'  : [ 'xml/types/unions.xml', 'xml/data/union_1.xml', ['-P -y Test::union_int32_default',    '-S -y Test::union_int16_default'],         [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'union_int32_default_int32_default'  : [ 'xml/types/unions.xml', 'xml/data/union_1.xml', ['-P -y Test::union_int32_default',    '-S -y Test::union_int32_default'],         [ReturnCode.OK, ReturnCode.OK] ],
    
    # EXT==FINAL: LABEL SETS MATCH EXACTLY [does this include 'default' label?]
    'union_final_5_vs_6'             : [ 'xml/types/unions.xml', 'xml/data/union_1.xml', ['-P -y Test::union_final_5',    '-S -y Test::union_final_6'],                         [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'union_final_6_vs_5'             : [ 'xml/types/unions.xml', 'xml/data/union_1.xml', ['-P -y Test::union_final_6',    '-S -y Test::union_final_5'],                         [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'union_final_one_default_1'      : [ 'xml/types/unions.xml', 'xml/data/union_1.xml', ['-P -y Test::union_final_5',    '-S -y Test::union_final_5_default'],                 [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'union_final_one_default_2'      : [ 'xml/types/unions.xml', 'xml/data/union_1.xml', ['-P -y Test::union_final_5_default',    '-S -y Test::union_final_5'],                 [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # EXT==!FINAL: LABEL SETS HAVE NON-EMPTY INTERSECTION [does this include 'default' label?] [these tests are ignore_member_names=TRUE]
    'union_appendable_one_common_1'  : [ 'xml/types/unions.xml', 'xml/data/union_a.xml', ['-P -y Test::union_appendable_a',    '-S -y Test::union_appendable_b'],               [ReturnCode.OK, ReturnCode.OK] ],
    'union_appendable_one_common_2'  : [ 'xml/types/unions.xml', 'xml/data/union_b.xml', ['-P -y Test::union_appendable_b',    '-S -y Test::union_appendable_a'],               [ReturnCode.OK, ReturnCode.OK] ],
    'union_appendable_no_common_1'   : [ 'xml/types/unions.xml', 'xml/data/union_a.xml', ['-P -y Test::union_appendable_a',    '-S -y Test::union_appendable_c'],               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'union_appendable_no_common_2'   : [ 'xml/types/unions.xml', 'xml/data/union_c.xml', ['-P -y Test::union_appendable_c',    '-S -y Test::union_appendable_a'],               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'union_appendable_no_common_w_default' : [ 'xml/types/unions.xml', 'xml/data/union_xd.xml', ['-P -y Test::union_appendable_a_default',    '-S -y Test::union_appendable_b_default'],  [ReturnCode.OK, ReturnCode.OK] ],

    'union_mutable_one_common'          : [ 'xml/types/unions.xml', 'xml/data/union_a.xml', ['-P -y Test::union_mutable_a',    '-S -y Test::union_mutable_b'],                  [ReturnCode.OK, ReturnCode.OK] ],
    'union_mutable_no_common'           : [ 'xml/types/unions.xml', 'xml/data/union_a.xml', ['-P -y Test::union_mutable_a',    '-S -y Test::union_mutable_c'],                  [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'union_mutable_no_common_w_default' : [ 'xml/types/unions.xml', 'xml/data/union_xd.xml', ['-P -y Test::union_mutable_a_default',    '-S -y Test::union_mutable_b_default'],  [ReturnCode.OK, ReturnCode.OK] ],
    
}
