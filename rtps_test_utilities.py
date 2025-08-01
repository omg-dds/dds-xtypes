#################################################################
# Use and redistribution is source and binary forms is permitted
# subject to the OMG-DDS INTEROPERABILITY TESTING LICENSE found
# at the following URL:
#
# https://github.com/omg-dds/dds-rtps/blob/master/LICENSE.md
#
#################################################################
import re

from enum import Enum
class ReturnCode(Enum):
    """"
    Codes to give information about Shape Applications' behavior.

    OK                   : Publisher/Subscriber sent/received data correctly
    TOPIC_NOT_CREATED    : Publisher/Subscriber does not create the topic
    READER_NOT_CREATED   : Subscriber does not create the Data Reader
    WRITER_NOT_CREATED   : Publisher does not create the Data Writer
    FILTER_NOT_CREATED   : Subscriber does not create the content filter
    INCOMPATIBLE_QOS     : Publisher/Subscriber with incompatible QoS.
    INCONSISTENT_TOPIC   : Inconsistent topic (eg, types not assignable)
    READER_NOT_MATCHED   : Publisher does not find any compatible Data Reader
    DATA_NOT_RECEIVED    : Subscriber does not receive the data
    DATA_NOT_SENT        : Publisher does not send the data
    DATA_NOT_CORRECT     : Subscriber does not find the data expected
    RECEIVING_FROM_ONE   : Subscriber receives from one Publisher
    RECEIVING_FROM_BOTH  : Subscriber receives from two Publishers
    DEADLINE_MISSED      : Publisher/Subscriber missed the deadline period
    ORDERED_ACCESS_INSTANCE : Subscriber reading with ordered access and access scope INSTANCE
    ORDERED_ACCESS_TOPIC : Subscriber reading with ordered access and access scope TOPIC
    """
    OK = 0
    TOPIC_NOT_CREATED = 1
    READER_NOT_CREATED = 2
    WRITER_NOT_CREATED = 3
    FILTER_NOT_CREATED = 4
    INCOMPATIBLE_QOS = 5
    INCONSISTENT_TOPIC = 6
    READER_NOT_MATCHED = 7
    DATA_NOT_RECEIVED = 9
    DATA_NOT_SENT = 10
    DATA_NOT_CORRECT = 11
    RECEIVING_FROM_ONE = 12
    RECEIVING_FROM_BOTH = 13
    DEADLINE_MISSED = 14
    ORDERED_ACCESS_INSTANCE = 15
    ORDERED_ACCESS_TOPIC = 16
    WRITER_UNEXPECTED_ERROR = 17
    READER_UNEXPECTED_ERROR = 18



def log_message(message, verbosity):
    if verbosity:
        print(message)

def remove_ansi_colors(text):
    ansi_escape = re.compile(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])')
    cleaned_str = ansi_escape.sub('', text)
    return cleaned_str

def no_check(child_sub, samples_sent, last_sample_saved, timeout):
    return ReturnCode.OK

def check_pub_sub_app_params (parameters: list[str]):
    """
    Check whether the app parameters start with pub-exe or sub-exe and that
    they are creating a publisher/subscriber accordingly.
    This also removes the pub-exe/sub-exe from the parameters list because they
    are not part of the executable parameters. This is just syntactic sugar
    to make sure the parameters are provided properly.
    """
    modified_parameters = []

    for param_str in parameters:
        parts = param_str.split()

        if 'pub-exe' == parts[0]:
            if '-P' not in parts:
                raise Exception(f'Publisher app does not have -P: {param_str}')
        elif 'sub-exe' == parts[0]:
            if '-S' not in parts:
                raise Exception(f'Subscriber app does not have -S: {param_str}')
        else:
            raise Exception(f'Apps parameters should start with "pub-exe" or '
                            '"sub-exe": {param_str}')
        parts = parts[1:]
         # Join back into a string and collect
        modified_parameters.append(" ".join(parts))

    return modified_parameters
