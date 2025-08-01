from rtps_test_utilities import ReturnCode
import test_suite_functions as tsf

# In the notes, T1 refers to reader's type and T2 refers to writer's type

# TEST NAME (unique) : [ xml_filename, [PARAMS_1,  PARAMS_2, ... ],    [ EXPECTED OUTPUT_1, EXPECTED_OUTPUT_2, ... ] ]


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
    'union_primitives_mutable': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_primitives_mutable -V xml/data/union_primitive.xml -J json/data/union_primitive.json',
                 'sub-exe -S -t test -y Test::union_primitives_mutable -V xml/data/union_primitive.xml -J json/data/union_primitive.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_final_appendable': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_primitives_final -V xml/data/union_primitive.xml -J json/data/union_primitive.json',
                 'sub-exe -S -t test -y Test::union_primitives_appendable -V xml/data/union_primitive.xml -J json/data/union_primitive.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_final_mutable': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_primitives_final -V xml/data/union_primitive.xml -J json/data/union_primitive.json',
                 'sub-exe -S -t test -y Test::union_primitives_mutable -V xml/data/union_primitive.xml -J json/data/union_primitive.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_appendable_mutable': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_primitives_appendable -V xml/data/union_primitive.xml -J json/data/union_primitive.json',
                 'sub-exe -S -t test -y Test::union_primitives_mutable -V xml/data/union_primitive.xml -J json/data/union_primitive.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_uint32_bitmask32': {
        'common_args': ['-X xml/types/unions_bitmask.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_uint32 -V xml/data/union_uint32.xml -J json/data/union_uint32.json',
                 'sub-exe -S -t test -y Test::union_bitmask32 -V xml/data/union_bitmask.xml -J json/data/union_bitmask.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_uint32_bitmask16': {
        'common_args': ['-X xml/types/unions_bitmask.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_uint32 -V xml/data/union_uint32.xml -J json/data/union_uint32.json',
                 'sub-exe -S -t test -y Test::union_bitmask16 -V xml/data/union_bitmask.xml -J json/data/union_bitmask.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_uint32_one_key': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_uint32 -V xml/data/union_uint32.xml -J json/data/union_uint32.json',
                 'sub-exe -S -t test -y Test::union_uint32_key -V xml/data/union_uint32.xml -J json/data/union_uint32.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_different_ids_ok': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_1 -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_2 -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_different_ids': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_1 -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_2 --ignore-member-names f -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_different_names_ok': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_3 -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_4 -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_different_names': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_3 -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_4 --ignore-member-names f -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_int16_int32': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_int16 -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_int32 -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_int16_int32_default': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_int16 -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_int32_default -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_int32_int32_default': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_int32 -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_int32_default -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_int32_default_int16': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_int32_default -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_int16 -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_int32_default_int32': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_int32_default -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_int32 -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_int32_default_int16_default': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_int32_default -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_int16_default -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_int32_default_int32_default': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_int32_default -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_int32_default -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_final_5_vs_6': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_final_5 -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_final_6 -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_final_6_vs_5': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_final_6 -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_final_5 -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_final_one_default_1': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_final_5 -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_final_5_default -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_final_one_default_2': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_final_5_default -V xml/data/union_1.xml -J json/data/union_1.json',
                 'sub-exe -S -t test -y Test::union_final_5 -V xml/data/union_1.xml -J json/data/union_1.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_appendable_one_common_1': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_appendable_a -V xml/data/union_a.xml -J json/data/union_a.json',
                 'sub-exe -S -t test -y Test::union_appendable_b -V xml/data/union_a.xml -J json/data/union_a.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_appendable_one_common_2': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_appendable_b -V xml/data/union_b.xml -J json/data/union_b.json',
                 'sub-exe -S -t test -y Test::union_appendable_a -V xml/data/union_b.xml -J json/data/union_b.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_appendable_no_common_1': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_appendable_a -V xml/data/union_a.xml -J json/data/union_a.json',
                 'sub-exe -S -t test -y Test::union_appendable_c -V xml/data/union_a.xml -J json/data/union_a.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_appendable_no_common_2': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_appendable_c -V xml/data/union_c.xml -J json/data/union_c.json',
                 'sub-exe -S -t test -y Test::union_appendable_a -V xml/data/union_c.xml -J json/data/union_c.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_appendable_no_common_w_default': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_appendable_a_default -V xml/data/union_xd.xml -J json/data/union_xd.json',
                 'sub-exe -S -t test -y Test::union_appendable_b_default -V xml/data/union_xd.xml -J json/data/union_xd.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_mutable_one_common': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_mutable_a -V xml/data/union_a.xml -J json/data/union_a.json',
                 'sub-exe -S -t test -y Test::union_mutable_b -V xml/data/union_a.xml -J json/data/union_a.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_mutable_no_common': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_mutable_a -V xml/data/union_a.xml -J json/data/union_a.json',
                 'sub-exe -S -t test -y Test::union_mutable_c -V xml/data/union_a.xml -J json/data/union_a.json'],
        'expected_codes': [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },
    'union_mutable_no_common_w_default': {
        'common_args': ['-X xml/types/unions.xml'],
        'apps': ['pub-exe -P -t test -y Test::union_mutable_a_default -V xml/data/union_xd.xml -J json/data/union_xd.json',
                 'sub-exe -S -t test -y Test::union_mutable_b_default -V xml/data/union_xd.xml -J json/data/union_xd.json'],
        'expected_codes': [ReturnCode.OK, ReturnCode.OK],
        'check_function': tsf.data_is_correct,
        'title': '',
        'description': ''
    },


    # # PRIMITIVES - struct primitive members assignable
    # 'union_primitives_final'      : [ 'xml/types/unions.xml', 'xml/data/union_primitive.xml',   ['-P -y Test::union_primitives_final',      '-S -y Test::union_primitives_final'     ], [ReturnCode.OK, ReturnCode.OK] ],
    # 'union_primitives_appendable' : [ 'xml/types/unions.xml', 'xml/data/union_primitive.xml',   ['-P -y Test::union_primitives_appendable', '-S -y Test::union_primitives_appendable'], [ReturnCode.OK, ReturnCode.OK] ],
    # 'union_primitives_mutable'    : [ 'xml/types/unions.xml', 'xml/data/union_primitive.xml',   ['-P -y Test::union_primitives_mutable',    '-S -y Test::union_primitives_mutable'   ], [ReturnCode.OK, ReturnCode.OK] ],

    # # EXTENSIBILITY MISMATCH
    # 'union_final_appendable'      : [ 'xml/types/unions.xml', 'xml/data/union_primitive.xml',   ['-P -y Test::union_primitives_final',      '-S -y Test::union_primitives_appendable'], [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'union_final_mutable'         : [ 'xml/types/unions.xml', 'xml/data/union_primitive.xml',   ['-P -y Test::union_primitives_final',      '-S -y Test::union_primitives_mutable'],    [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'union_appendable_mutable'    : [ 'xml/types/unions.xml', 'xml/data/union_primitive.xml',   ['-P -y Test::union_primitives_appendable', '-S -y Test::union_primitives_mutable'],    [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # # DISCRIMINATORs STRONGLY ASSIGNABLE
    # 'union_uint32_bitmask32'      : [ 'xml/types/unions.xml', 'xml/data/union_uint32.xml',   ['-P -y Test::union_uint32',                   '-S -y Test::union_bitmask32'],          [ReturnCode.OK, ReturnCode.OK] ],
    # 'union_uint32_bitmask16'      : [ 'xml/types/unions.xml', 'xml/data/union_uint32.xml',   ['-P -y Test::union_uint32',                   '-S -y Test::union_bitmask16'],          [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # # DISCRIMINATOR KEY IN ONE BUT NOT OTHER
    # 'union_uint32_one_key'        : [ 'xml/types/unions.xml', 'xml/data/union_uint32.xml',   ['-P -y Test::union_uint32',                    '-S -y Test::union_uint32_key'],        [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # # IF MEMBER NAME MATCHES, MEMBER ID MUST MATCH [can be bypassed with TypeConsistency.ignore_member_names]
    # 'union_different_ids_ok'      : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_1',      '-S -y Test::union_2'],                                    [ReturnCode.OK, ReturnCode.OK] ],
    # 'union_different_ids'         : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_1',      '-S -y Test::union_2 --check-member-names'],               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # # IF MEMBER ID MATCHES, MEMBER NAME MUST MATCH [can be bypassed with TypeConsistency.ignore_member_names]
    # 'union_different_names_ok'    : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_3',      '-S -y Test::union_4'],                                    [ReturnCode.OK, ReturnCode.OK] ],
    # 'union_different_names'       : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_3',      '-S -y Test::union_4 --check-member-names'],               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # # LABELS IN T2 SELECT A T1 MEMBER, THEN T1.m1 IS ASSIGNABLE FROM T2's MEMBER
    # 'union_int16_int32'           : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_int16',      '-S -y Test::union_int32'],                            [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'union_int16_int32_default'   : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_int16',      '-S -y Test::union_int32_default'],                    [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'union_int32_int32_default'   : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_int32',      '-S -y Test::union_int32_default'],                    [ReturnCode.OK, ReturnCode.OK] ],

    # # LABELS IN T1 SELECT 'default' in T2 THEN T1.m1 IS ASSIGNABLE FROM T2.default
    # 'union_int32_default_int16'   : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_int32_default',    '-S -y Test::union_int16'],                      [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'union_int32_default_int32'   : [ 'xml/types/unions.xml', 'xml/data/union_1.xml',   ['-P -y Test::union_int32_default',    '-S -y Test::union_int32'],                      [ReturnCode.OK, ReturnCode.OK] ],

    # # BOTH HAVE 'default' THEN T1.default IS ASSIGNABLE FROM T2.default
    # 'union_int32_default_int16_default'  : [ 'xml/types/unions.xml', 'xml/data/union_1.xml', ['-P -y Test::union_int32_default',    '-S -y Test::union_int16_default'],         [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'union_int32_default_int32_default'  : [ 'xml/types/unions.xml', 'xml/data/union_1.xml', ['-P -y Test::union_int32_default',    '-S -y Test::union_int32_default'],         [ReturnCode.OK, ReturnCode.OK] ],

    # # EXT==FINAL: LABEL SETS MATCH EXACTLY [does this include 'default' label?]
    # 'union_final_5_vs_6'             : [ 'xml/types/unions.xml', 'xml/data/union_1.xml', ['-P -y Test::union_final_5',    '-S -y Test::union_final_6'],                         [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'union_final_6_vs_5'             : [ 'xml/types/unions.xml', 'xml/data/union_1.xml', ['-P -y Test::union_final_6',    '-S -y Test::union_final_5'],                         [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'union_final_one_default_1'      : [ 'xml/types/unions.xml', 'xml/data/union_1.xml', ['-P -y Test::union_final_5',    '-S -y Test::union_final_5_default'],                 [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'union_final_one_default_2'      : [ 'xml/types/unions.xml', 'xml/data/union_1.xml', ['-P -y Test::union_final_5_default',    '-S -y Test::union_final_5'],                 [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],

    # # EXT==!FINAL: LABEL SETS HAVE NON-EMPTY INTERSECTION [does this include 'default' label?] [these tests are ignore_member_names=TRUE]
    # 'union_appendable_one_common_1'  : [ 'xml/types/unions.xml', 'xml/data/union_a.xml', ['-P -y Test::union_appendable_a',    '-S -y Test::union_appendable_b'],               [ReturnCode.OK, ReturnCode.OK] ],
    # 'union_appendable_one_common_2'  : [ 'xml/types/unions.xml', 'xml/data/union_b.xml', ['-P -y Test::union_appendable_b',    '-S -y Test::union_appendable_a'],               [ReturnCode.OK, ReturnCode.OK] ],
    # 'union_appendable_no_common_1'   : [ 'xml/types/unions.xml', 'xml/data/union_a.xml', ['-P -y Test::union_appendable_a',    '-S -y Test::union_appendable_c'],               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'union_appendable_no_common_2'   : [ 'xml/types/unions.xml', 'xml/data/union_c.xml', ['-P -y Test::union_appendable_c',    '-S -y Test::union_appendable_a'],               [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'union_appendable_no_common_w_default' : [ 'xml/types/unions.xml', 'xml/data/union_xd.xml', ['-P -y Test::union_appendable_a_default',    '-S -y Test::union_appendable_b_default'],  [ReturnCode.OK, ReturnCode.OK] ],

    # 'union_mutable_one_common'          : [ 'xml/types/unions.xml', 'xml/data/union_a.xml', ['-P -y Test::union_mutable_a',    '-S -y Test::union_mutable_b'],                  [ReturnCode.OK, ReturnCode.OK] ],
    # 'union_mutable_no_common'           : [ 'xml/types/unions.xml', 'xml/data/union_a.xml', ['-P -y Test::union_mutable_a',    '-S -y Test::union_mutable_c'],                  [ReturnCode.INCONSISTENT_TOPIC, ReturnCode.INCONSISTENT_TOPIC] ],
    # 'union_mutable_no_common_w_default' : [ 'xml/types/unions.xml', 'xml/data/union_xd.xml', ['-P -y Test::union_mutable_a_default',    '-S -y Test::union_mutable_b_default'],  [ReturnCode.OK, ReturnCode.OK] ],

}
