from test_util import ReturnCode, log_message

# FIRST: check a bunch of 'OK' combinations.
#        these don't really 'prove' anything
#        (they could pass without even comparing types...)
#        but, we still need to make sure these pass...

# THEN: a bunch of examples to test the various ways
#       that a structure shouldn't be assignable.

    # TEST NAME (unique) : [ xml_filename, [PARAMS_1,  PARAMS_2, ... ],    [ EXPECTED OUTPUT_1, EXPECTED_OUTPUT_2, ... ] ]
    

xtypes_v2_struct_test_suite = {

    # PRIMITIVES - struct primitive members assignable 
    'primitives_struct_final'      : [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitives_final',      '-S -y Test::struct_primitives_final'     ], [ReturnCode.OK, ReturnCode.OK] ],
    'primitives_struct_appendable' : [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitives_appendable', '-S -y Test::struct_primitives_appendable'], [ReturnCode.OK, ReturnCode.OK] ],
    'primitives_struct_mutable'    : [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitives_mutable',    '-S -y Test::struct_primitives_mutable'   ], [ReturnCode.OK, ReturnCode.OK] ],

    # EXTENSIBILITY MUST MATCH:
    'struct_final_appendable'      : [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitives_final',      '-S -y Test::struct_primitives_appendable'], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'struct_final_mutable'         : [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitives_final',      '-S -y Test::struct_primitives_mutable'],    [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'struct_appendable_final'      : [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitives_appendable', '-S -y Test::struct_primitives_final'],      [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'struct_appendable_mutable'    : [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitives_appendable', '-S -y Test::struct_primitives_mutable'],    [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'struct_mutable_final'         : [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitives_mutable',    '-S -y Test::struct_primitives_final'],      [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'struct_mutable_appendable'    : [ 'xml/types/primitives.xml', 'xml/data/struct_primitives.xml', ['-P -y Test::struct_primitives_mutable',    '-S -y Test::struct_primitives_appendable'], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # IF MEMBER NAME MATCHES, MEMBER ID MUST MATCH [can be bypassed with TypeConsistency.ignore_member_names] 
    'struct_different_ids_ok'      : [ 'xml/types/struct_names.xml', 'xml/data/struct_1.xml', ['-P -y Test::struct_1',      '-S -y Test::struct_2'],                                  [ReturnCode.OK, ReturnCode.OK] ],
    'struct_different_ids'         : [ 'xml/types/struct_names.xml', 'xml/data/struct_1.xml', ['-P -y Test::struct_1',      '-S -y Test::struct_2 --check-member-names'],             [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # IF MEMBER ID MATCHES, MEMBER NAME MUST MATCH [can be bypassed with TypeConsistency.ignore_member_names]
    'struct_different_names_ok'    : [ 'xml/types/struct_names.xml', 'xml/data/struct_1.xml', ['-P -y Test::struct_3',      '-S -y Test::struct_4'],                                  [ReturnCode.OK, ReturnCode.OK] ],
    'struct_different_names'       : [ 'xml/types/struct_names.xml', 'xml/data/struct_1.xml', ['-P -y Test::struct_3',      '-S -y Test::struct_4 --check-member-names'],             [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # AT LEAST ONE MEMBER IN COMMON [ same id ]
    'struct_no_common_ids'         : [ 'xml/types/struct_names.xml', 'xml/data/struct_1.xml', ['-P -y Test::struct_5',      '-S -y Test::struct_6'],                                  [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # MEMBERS WITH MATCHING ID ARE ASSIGNABLE [KeyErased]
    'struct_members_assignable_1'  : [ 'xml/types/struct_names.xml', 'xml/data/struct_1.xml', ['-P -y Test::struct_primitive_uint8',      '-S -y Test::struct_primitive_uint16'],     [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # !OPTIONAL + MUST_UNDERSTAND MEMBER PRESENT IN ONE TYPE MUST APPEAR IN OTHER TYPE
    'struct_grok_1'                : [ 'xml/types/struct_names.xml', 'xml/data/struct_1.xml', ['-P -y Test::struct_grok_1', '-S -y Test::struct_grok_2'],                             [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'struct_grok_2'                : [ 'xml/types/struct_names.xml', 'xml/data/struct_1.xml', ['-P -y Test::struct_grok_2', '-S -y Test::struct_grok_1'],                             [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # KEY MEMBERS PRESENT IN ONE TYPE APPEAR IN THE OTHER
    'struct_key_1'                 : [ 'xml/types/struct_names.xml', 'xml/data/struct_1.xml', ['-P -y Test::struct_key_1', '-S -y Test::struct_key_2'],                               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    'struct_key_2'                 : [ 'xml/types/struct_names.xml', 'xml/data/struct_1.xml', ['-P -y Test::struct_key_2', '-S -y Test::struct_key_1'],                               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # STRING KEY MEMBER IN T2 BOUND CHECK [ not bypassed with TypeConsistency.ignore_string_bounds ]
    'struct_key_string_1'          : [ 'xml/types/struct_names.xml', 'xml/data/struct_str.xml', ['-P -y Test::struct_key_string_1', '-S -y Test::struct_key_string_2'],                 [ReturnCode.OK, ReturnCode.OK] ],
    'struct_key_string_2'          : [ 'xml/types/struct_names.xml', 'xml/data/struct_str.xml', ['-P -y Test::struct_key_string_1', '-S -y Test::struct_key_string_1'],                 [ReturnCode.OK, ReturnCode.OK] ],
    'struct_key_string_3'          : [ 'xml/types/struct_names.xml', 'xml/data/struct_str.xml', ['-P -y Test::struct_key_string_2', '-S -y Test::struct_key_string_1'],                 [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # ENUM KEY MEMBER IN T2 HAVE SAME CONSTANTS
    'struct_key_enum_1'            : [ 'xml/types/struct_names.xml', 'xml/data/struct_enum.xml', ['-P -y Test::struct_key_enum_1', '-S -y Test::struct_key_enum_2'],                     [ReturnCode.OK, ReturnCode.OK] ],
    'struct_key_enum_2'            : [ 'xml/types/struct_names.xml', 'xml/data/struct_enum.xml', ['-P -y Test::struct_key_enum_2', '-S -y Test::struct_key_enum_1'],                     [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # SEQ KEY MEMBER IN T2 BOUND CHECK [ not bypassed with TypeConsistency.ignore_sequence_bounds ]
    'struct_key_seq_1'             : [ 'xml/types/struct_names.xml', 'xml/data/struct_seq.xml', ['-P -y Test::struct_key_seq_1', '-S -y Test::struct_key_seq_2'],                       [ReturnCode.OK, ReturnCode.OK] ],
    'struct_key_seq_2'             : [ 'xml/types/struct_names.xml', 'xml/data/struct_seq.xml', ['-P -y Test::struct_key_seq_2', '-S -y Test::struct_key_seq_1'],                       [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
     
    # STRUCT KEY MEMBER IN T2 CHECK KeyHolder(is-assignable-from)
    'struct_key_struct_1'          : [ 'xml/types/struct_names.xml', 'xml/data/struct_key.xml', ['-P -y Test::struct_key_struct_1', '-S -y Test::struct_key_struct_2'],                 [ReturnCode.OK, ReturnCode.OK] ],
    'struct_key_struct_2'          : [ 'xml/types/struct_names.xml', 'xml/data/struct_key.xml', ['-P -y Test::struct_key_struct_2', '-S -y Test::struct_key_struct_1'],                 [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    
    # UNION KEY MEMBER IN T2 CHECK KeyHolder(is-assignable-from)
    'struct_key_union_1'           : [ 'xml/types/struct_names.xml', 'xml/data/struct_key_union.xml', ['-P -y Test::struct_key_union_1', '-S -y Test::struct_key_union_2'],                   [ReturnCode.OK, ReturnCode.OK] ],
    'struct_key_union_2'           : [ 'xml/types/struct_names.xml', 'xml/data/struct_key_union.xml', ['-P -y Test::struct_key_union_2', '-S -y Test::struct_key_union_1'],                   [ReturnCode.OK, ReturnCode.OK] ],

    # ad nauseam...
}

