#include "Utils.h"
#include <iostream>

#if DATA_REPRESENTATION_QOS_SUPPORTED == 1
DDSDataReader *create_reader_with_data_representation(
        DDSSubscriber *subscriber,
        DDSTopic *topic,
        const DDS_DataRepresentationIdSeq &representation_id_seq)
{
    DDS_DataReaderQos reader_qos = get_test_datareader_default_qos();
    reader_qos.representation.value.copy(representation_id_seq);
    DDSDataReader *reader = subscriber->create_datareader(
          topic,
          reader_qos,
          NULL,
          DDS_STATUS_MASK_NONE);
    return reader;
}

DDSDataWriter *create_writer_with_data_representation(
        DDSPublisher *publisher,
        DDSTopic *topic,
        const DDS_DataRepresentationIdSeq &representation_id_seq)
{
    DDS_DataWriterQos writer_qos = get_test_datawriter_default_qos();
    writer_qos.representation.value.copy(representation_id_seq);
    DDSDataWriter *writer = publisher->create_datawriter(topic, writer_qos, NULL, DDS_STATUS_MASK_NONE);
    return writer;
}

DDS_DataRepresentationIdSeq deserialize_cdr_list(CDRPolicy cdr_list)
{
    DDS_DataRepresentationIdSeq seq;
    switch (cdr_list) {
        case CDR_POLICY_XCDR:
            seq.maximum(1);
            seq.length(1);
            seq[0] = DDS_XCDR_DATA_REPRESENTATION;
            break;
        case CDR_POLICY_XCDR2:
            seq.maximum(1);
            seq.length(1);
            seq[0] = DDS_XCDR2_DATA_REPRESENTATION;
            break;
        case CDR_POLICY_XCDR_XCDR2:
            seq.maximum(2);
            seq.length(2);
            seq[0] = DDS_XCDR_DATA_REPRESENTATION;
            seq[1] = DDS_XCDR2_DATA_REPRESENTATION;
            break;
        default:
            break;
    }
    return seq;
}

CDRPolicy serialize_cdr_list(std::vector<DDS_DataRepresentationId_t> list)
{
    if (list.size() <= 0 || list.size() > 1)
        throw new std::runtime_error("cdr list wrong length: " + std::to_string(list.size()));
    if (list.size() == 2) {
        if(list[0] == DDS_XCDR_DATA_REPRESENTATION && list[1] == DDS_XCDR2_DATA_REPRESENTATION)
            return CDR_POLICY_XCDR_XCDR2;
        else
            throw new std::runtime_error("cdr list wrong length: " + std::to_string(list.size()));
    }
    if (list[0] == DDS_XCDR_DATA_REPRESENTATION)
        return CDR_POLICY_XCDR;
    if (list[0] == DDS_XCDR2_DATA_REPRESENTATION)
        return CDR_POLICY_XCDR2;
    throw new std::runtime_error("cdr list wrong length: " + std::to_string(list.size()));
}
#endif

void launch_helper(std::string name)
{
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#include <Windows.h>
    STARTUPINFO info = {sizeof(info)};
    PROCESS_INFORMATION processInfo;
    name += ".exe";
    if(CreateProcess(name.c_str()+2, NULL, NULL, NULL, true, 0, NULL, NULL, &info, &processInfo) == 0)
    {
        std::cout << "CreateProcess() failed: " << GetLastError() << std::endl;
    }
#else
    //fork a child process.
    if (fork() == 0) {
        char *argv[2] = {DDS_String_dup(name.data()), NULL};
        execv(name.data(), argv);
        if (errno != ENOENT)
            perror("execve() failed ");
        char cwd[256];
        getcwd(cwd, sizeof(cwd));
        ::printf("%s\n", cwd);
        throw std::runtime_error("failed to launch helper " + name);
    }
#endif
}

void init_entities(
        DDSDomainParticipant *&participant,
        DDSPublisher *&publisher,
        DDSSubscriber *&subscriber,
        TestController *&controller,
        bool isMaster)
{
#if PARTICIPANT_REGISTRY_SUPPORTED == 1
    DDS_DomainParticipantFactoryQos dpf_qos;
    TheParticipantFactory->get_qos(dpf_qos);
    dpf_qos.resource_limits.max_participants = 32;
    TheParticipantFactory->set_qos(dpf_qos);
    RT::Registry *registry = TheParticipantFactory->get_registry();
    DPDE::DiscoveryPluginProperty dpde_properties;

    dpde_properties.participant_liveliness_assert_period.sec = LIVELINESS_TEST_ASSERT_PERIOD;
    dpde_properties.participant_liveliness_lease_duration.sec = LIVELINESS_TEST_LEASE_DURATION;
    
    if (!registry->register_component(
                                      "wh",
                                      WHSM::HistoryFactory::get_interface(),
                                      NULL,
                                      NULL)) {
        throw std::runtime_error("failed to register wh\n");
    }
    
    if (!registry->register_component("rh",
                                      RHSM::HistoryFactory::get_interface(),
                                      NULL,
                                      NULL)) {
        throw std::runtime_error("failed to register rh\n");
    }
    
    if (!registry->register_component(
                                      "dpde",
                                      DPDE::DiscoveryFactory::get_interface(),
                                      &dpde_properties._parent,
                                      NULL)) {
        throw std::runtime_error("failed to register dpde\n");
    }
#endif
    DDS_DomainParticipantQos dp_qos = get_test_participant_default_qos();

    participant = DDSTheParticipantFactory->create_participant(
            TEST_DOMAIN_ID,
            dp_qos,
            NULL,
            DDS_STATUS_MASK_NONE);
    if (participant == NULL) {
        throw std::runtime_error("init_entity() fails to create participant");
    }
    publisher = create_publisher(participant);
    if (publisher == NULL) {
        throw std::runtime_error("init_entity() fails to create publisher");
    }
    subscriber = create_subscriber(participant);
    if (subscriber == NULL) {
        throw std::runtime_error("init_entity() fails to create subscriber");
    }
    
    controller = new TestController(participant, publisher, subscriber, isMaster);
    if (controller == NULL) {
        throw std::runtime_error("init_entity() fails to create controller");
    }
}

void destroy_entities(
        DDSDomainParticipant *&participant,
        DDSPublisher *&publisher,
        DDSSubscriber *&subscriber,
        TestController *&controller)
{
    if (controller != NULL) {
        ControlMessage msg;
        msg.action = TEST_ACTION_EXIT;
        msg.param1._d  = TEST_ACTION_EXIT;
        controller->send_msg(msg);
        delete controller;
        controller = NULL;
    }
    
    if (participant != NULL) {
        participant->delete_contained_entities();
        DDSTheParticipantFactory->delete_participant(participant);
        participant = NULL;
        publisher = NULL;
        subscriber = NULL;
    }

#if PARTICIPANT_REGISTRY_SUPPORTED == 1
    RT::Registry *registry = TheParticipantFactory->get_registry();
    DPDE::DiscoveryPluginProperty dpde_properties;
    
    if (!registry->unregister(
        "wh",
        NULL,
        NULL)) {
        throw std::runtime_error("failed to unregister wh\n");
    }
    
    if (!registry->unregister("rh",
            NULL,
            NULL)) {
        throw std::runtime_error("failed to unregister rh\n");
    }
    if (!registry->unregister(
            "dpde",
            NULL,
            NULL)) {
        throw std::runtime_error("failed to unregister dpde\n");
    }
#endif
}

DDSSubscriber *create_subscriber(DDSDomainParticipant *participant, const DDS_SubscriberQos &qos)
{
    return participant->create_subscriber(qos, NULL, DDS_STATUS_MASK_NONE);
}

DDSPublisher *create_publisher(DDSDomainParticipant *participant, const DDS_PublisherQos &qos)
{
    return participant->create_publisher(qos, NULL, DDS_STATUS_MASK_NONE);
}

DDS_DataReaderQos get_test_datareader_default_qos()
{
    DDS_DataReaderQos qos = DDS_DATAREADER_QOS_DEFAULT;
    qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
    qos.durability.kind = DDS_TRANSIENT_LOCAL_DURABILITY_QOS;
    qos.history.kind = DDS_KEEP_LAST_HISTORY_QOS;
    qos.history.depth = 1;
#if DATA_REPRESENTATION_QOS_SUPPORTED == 1
    qos.representation.value.length(2);
    qos.representation.value[0] = DDS_XCDR_DATA_REPRESENTATION;
    qos.representation.value[0] = DDS_XCDR2_DATA_REPRESENTATION;
#endif
    return qos;
}

DDS_DataWriterQos get_test_datawriter_default_qos()
{
    DDS_DataWriterQos qos = DDS_DATAWRITER_QOS_DEFAULT;
    qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
    qos.durability.kind = DDS_TRANSIENT_LOCAL_DURABILITY_QOS;
    qos.history.kind = DDS_KEEP_LAST_HISTORY_QOS;
    qos.history.depth = 1;
    return qos;
}

DDS_DomainParticipantQos get_test_participant_default_qos()
{
    DDS_DomainParticipantQos dp_qos;
    DDSTheParticipantFactory->get_default_participant_qos(dp_qos);
    dp_qos.discovery.initial_peers.maximum(1);
    dp_qos.discovery.initial_peers.length(1);
    dp_qos.discovery.initial_peers[0] = DDS_String_dup("127.0.0.1");
#if DISCOVERY_SETNAME_SUPPORTED == 1
    dp_qos.discovery.discovery.name.set_name("dpde");
#endif
    
#if MAX_PORT_QOS_SUPPORTED == 1
    dp_qos.resource_limits.max_destination_ports = 32;
    dp_qos.resource_limits.max_receive_ports = 32;
    dp_qos.resource_limits.max_destination_ports = 32;
    dp_qos.resource_limits.max_receive_ports = 32;
#endif
    
#if MAX_LOCAL_TYPE_QOS_SUPPORTED == 1
    dp_qos.resource_limits.local_type_allocation = 8;
#endif
#if RESOURCE_LIMIT_QOS_IS_FLAT == 1
    dp_qos.resource_limits.local_topic_allocation = 8;
    dp_qos.resource_limits.matching_reader_writer_pair_allocation = 4;
    dp_qos.resource_limits.matching_writer_reader_pair_allocation = 4;
    dp_qos.resource_limits.local_subscriber_allocation = 8;
    dp_qos.resource_limits.local_publisher_allocation = 8;
    dp_qos.resource_limits.local_reader_allocation = 8;
    dp_qos.resource_limits.local_writer_allocation = 8;
    dp_qos.resource_limits.remote_participant_allocation = 8;
    dp_qos.resource_limits.remote_reader_allocation = 8;
    dp_qos.resource_limits.remote_writer_allocation = 8;
#endif
    return dp_qos;
}

DDS_ReturnCode_t wait_on_status(
        DDSDataReader *reader,
        const DDS_StatusMask &mask,
        DDSConditionSeq &active_condition,
        const DDS_Duration_t &timeout)
{
    DDS_ReturnCode_t ret;
    DDSStatusCondition *status_condition = reader->get_statuscondition();
    if (status_condition == NULL) {
        throw std::runtime_error("failed to get status condition!");
    }
    status_condition->set_enabled_statuses(mask);
    DDSWaitSet waitset;
    ret = waitset.attach_condition(status_condition);
    if (ret != DDS_RETCODE_OK) {
        throw std::runtime_error("failed to attach condition! " + std::to_string(ret));
    }
    return waitset.wait(active_condition, timeout);
}

template <typename T>
void set_qos_properties(T qos,
                   const std::vector<QosProperty> &set_to_one,
                   const std::vector<QosProperty> &set_to_zero)
{
#if TYPE_CONSISTENCY_QOS_SUPPORTED == 1
    for(QosProperty prop : set_to_one)
    {
        switch (prop) {
            case PREVENT_TYPE_WIDENING:
                qos.type_consistency.prevent_type_widening = DDS_BOOLEAN_TRUE;
                break;
            case IGNORE_STRING_BOUNDS:
                qos.type_consistency.ignore_string_bounds = DDS_BOOLEAN_TRUE;
                break;
            case IGNORE_SEQUENCE_BOUNDS:
                qos.type_consistency.ignore_sequence_bounds = DDS_BOOLEAN_TRUE;
                break;
            case IGNORE_MEMBER_NAMES:
                qos.type_consistency.ignore_member_names = DDS_BOOLEAN_TRUE;
                break;
        }
    }
    for(QosProperty prop : set_to_zero)
    {
        switch (prop) {
            case PREVENT_TYPE_WIDENING:
                qos.type_consistency.prevent_type_widening = DDS_BOOLEAN_FALSE;
                break;
            case IGNORE_STRING_BOUNDS:
                qos.type_consistency.ignore_string_bounds = DDS_BOOLEAN_FALSE;
                break;
            case IGNORE_SEQUENCE_BOUNDS:
                qos.type_consistency.ignore_sequence_bounds = DDS_BOOLEAN_FALSE;
                break;
            case IGNORE_MEMBER_NAMES:
                qos.type_consistency.ignore_member_names = DDS_BOOLEAN_FALSE;
                break;
        }
    }
#endif
}

void apply_qos_properties(DDS_DataReaderQos qos,
                          const std::vector<QosProperty> &set_to_one,
                          const std::vector<QosProperty> &set_to_zero)
{
    set_qos_properties(qos, set_to_one, set_to_zero);
}

void apply_qos_properties(DDS_DataWriterQos qos,
                          const std::vector<QosProperty> &set_to_one,
                          const std::vector<QosProperty> &set_to_zero)
{
    set_qos_properties(qos, set_to_one, set_to_zero);
}
