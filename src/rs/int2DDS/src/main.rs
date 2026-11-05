#![allow(dead_code)]

use std::io::Write;
use std::process;
use std::sync::{Arc, OnceLock};
use std::time::Duration as StdDuration;

/// Print a line and flush stdout immediately.
macro_rules! println_flush {
    ($($arg:tt)*) => {{
        println!($($arg)*);
        let _ = std::io::stdout().flush();
    }};
}

use clap::{Parser, ValueEnum};

use int2dds::core::types::LENGTH_UNLIMITED;
use int2dds::dcps::infrastructure::qos_policy::OwnershipStrengthQosPolicy;
use int2dds::serialize::Float128;
use int2dds::{
    common::{env::set_xtypes_rti_key_compat, instance_handle::InstanceHandle},
    config::xml::XmlTypeRegistry,
    core::{error::DdsError, time::Duration},
    domain::{domain_participant_factory::DomainParticipantFactory, qos::DomainParticipantQos},
    infrastructure::{
        qos_policy::{
            DataRepresentationId, DataRepresentationQosPolicy, DeadlineQosPolicy,
            DurabilityQosPolicy, DurabilityQosPolicyKind, HistoryQosPolicy, HistoryQosPolicyKind,
            OwnershipQosPolicy, OwnershipQosPolicyKind, PartitionQosPolicy, ReliabilityQosPolicy,
            ReliabilityQosPolicyKind, TimeBasedFilterQosPolicy,
            TypeConsistencyEnforcementQosPolicy, TypeConsistencyKind,
        },
        status::StatusMask,
    },
    publication::{
        data_writer_listener::DataWriterListener,
        qos::{DataWriterQos, PublisherQos},
    },
    subscription::{
        data_reader_listener::DataReaderListener,
        qos::{DataReaderQos, SubscriberQos},
        sample_info::{InstanceStateKind, SampleStateKind, ViewStateKind},
    },
    topic::{qos::TopicQos, topic_listener::TopicListener},
    xtypes::{
        build_minimal_closure, DynamicData, DynamicType, DynamicTypeKind, DynamicTypeSupport,
        DynamicValue, EnumDescriptor, PrimitiveKind, TypeIdentifier, TypeObject,
        UNION_DISCRIMINATOR_FIELD,
    },
};

#[derive(Debug, Clone, Copy, PartialEq, ValueEnum)]
enum Verbosity {
    #[value(name = "e")]
    Error = 1,
    #[value(name = "d")]
    Debug = 2,
}

struct Logger {
    verbosity: Verbosity,
}

impl Logger {
    fn new(verbosity: Verbosity) -> Self {
        Self { verbosity }
    }

    fn log_message(&self, message: &str, level: Verbosity) {
        if level as i32 <= self.verbosity as i32 {
            println_flush!("{}", message);
        }
    }

    fn set_verbosity(&mut self, verbosity: Verbosity) {
        self.verbosity = verbosity;
    }
}

static LOGGER: OnceLock<Logger> = OnceLock::new();

fn init_logger(verbosity: Verbosity) {
    let _ = LOGGER.set(Logger::new(verbosity));
}

fn get_logger() -> &'static Logger {
    LOGGER.get().expect("Logger not initialized")
}

/// Value of a `[t|f|d]` type consistency option.
#[derive(Debug, Clone, Copy, PartialEq, ValueEnum)]
enum TriState {
    #[value(name = "t")]
    True,
    #[value(name = "f")]
    False,
    #[value(name = "d")]
    Default,
}

impl TriState {
    // An unset option keeps the test default, 'd' takes the QoS policy default.
    fn resolve(option: Option<TriState>, test_default: bool, qos_default: bool) -> bool {
        match option {
            None => test_default,
            Some(TriState::True) => true,
            Some(TriState::False) => false,
            Some(TriState::Default) => qos_default,
        }
    }
}

// QosUtils implementation
struct QosUtils;

impl QosUtils {
    fn get_reliability_policy(options: &TestOptions) -> ReliabilityQosPolicy {
        ReliabilityQosPolicy {
            kind: if options.best_effort {
                ReliabilityQosPolicyKind::BestEffort
            } else {
                ReliabilityQosPolicyKind::Reliable
            },
            max_blocking_time: Duration { sec: 0, nanosec: 100_000_000 },
        }
    }

    fn get_durability_policy(options: &TestOptions) -> DurabilityQosPolicy {
        DurabilityQosPolicy {
            kind: match options.durability.chars().next().unwrap_or('v') {
                'v' => DurabilityQosPolicyKind::Volatile,
                'l' => DurabilityQosPolicyKind::TransientLocal,
                't' => DurabilityQosPolicyKind::Transient,
                'p' => DurabilityQosPolicyKind::Persistent,
                _ => DurabilityQosPolicyKind::Volatile,
            },
        }
    }

    fn get_ownership_policy(options: &TestOptions) -> OwnershipQosPolicy {
        OwnershipQosPolicy {
            kind: if options.ownership_strength != -1 {
                OwnershipQosPolicyKind::Exclusive
            } else {
                OwnershipQosPolicyKind::Shared
            },
        }
    }

    fn get_ownership_strength_policy(options: &TestOptions) -> OwnershipStrengthQosPolicy {
        OwnershipStrengthQosPolicy {
            value: if options.ownership_strength != -1 {
                options.ownership_strength
            } else {
                0 // Default value for shared ownership
            },
        }
    }

    fn get_history_policy(options: &TestOptions) -> HistoryQosPolicy {
        HistoryQosPolicy {
            kind: match options.history_depth {
                depth if depth > 0 => HistoryQosPolicyKind::KeepLast(depth),
                0 => HistoryQosPolicyKind::KeepAll,
                _ => HistoryQosPolicyKind::default(),
            },
            ..Default::default()
        }
    }

    fn get_deadline_policy(options: &TestOptions) -> DeadlineQosPolicy {
        if options.deadline_interval == 0 {
            DeadlineQosPolicy::default()
        } else {
            DeadlineQosPolicy { period: Duration::from_seconds(options.deadline_interval) }
        }
    }

    fn get_time_based_filter_policy(options: &TestOptions) -> TimeBasedFilterQosPolicy {
        if options.timebasedfilter_interval == 0 {
            TimeBasedFilterQosPolicy::default() // no filter
        } else {
            TimeBasedFilterQosPolicy {
                minimum_separation: Duration::from_seconds(options.timebasedfilter_interval),
            }
        }
    }

    fn get_data_representation_policy(options: &TestOptions) -> DataRepresentationQosPolicy {
        match options.data_representation {
            1 => DataRepresentationQosPolicy {
                value: vec![DataRepresentationId::XcdrDataRepresentation],
            },
            2 => DataRepresentationQosPolicy {
                value: vec![DataRepresentationId::Xcdr2DataRepresentation],
            },
            _ => DataRepresentationQosPolicy {
                value: vec![DataRepresentationId::XcdrDataRepresentation],
            }, // default to XCDR
        }
    }

    fn get_type_consistency_policy(options: &TestOptions) -> TypeConsistencyEnforcementQosPolicy {
        let default = TypeConsistencyEnforcementQosPolicy::default();
        TypeConsistencyEnforcementQosPolicy {
            kind: match options.allow_type_coercion {
                None | Some(TriState::True) => TypeConsistencyKind::AllowTypeCoercion,
                Some(TriState::False) => TypeConsistencyKind::DisallowTypeCoercion,
                Some(TriState::Default) => default.kind,
            },
            ignore_sequence_bounds: TriState::resolve(
                options.ignore_seq_bounds,
                true,
                default.ignore_sequence_bounds,
            ),
            ignore_string_bounds: TriState::resolve(
                options.ignore_str_bounds,
                true,
                default.ignore_string_bounds,
            ),
            ignore_member_names: TriState::resolve(
                options.ignore_member_names,
                true,
                default.ignore_member_names,
            ),
            prevent_type_widening: TriState::resolve(
                options.prevent_type_widening,
                false,
                default.prevent_type_widening,
            ),
            force_type_validation: TriState::resolve(
                options.force_type_validation,
                false,
                default.force_type_validation,
            ),
        }
    }

    fn reliability_to_string(kind: ReliabilityQosPolicyKind) -> String {
        match kind {
            ReliabilityQosPolicyKind::BestEffort => "BEST_EFFORT".to_string(),
            ReliabilityQosPolicyKind::Reliable => "RELIABLE".to_string(),
        }
    }

    fn durability_to_string(kind: DurabilityQosPolicyKind) -> String {
        match kind {
            DurabilityQosPolicyKind::Volatile => "VOLATILE".to_string(),
            DurabilityQosPolicyKind::TransientLocal => "TRANSIENT_LOCAL".to_string(),
            DurabilityQosPolicyKind::Transient => "TRANSIENT".to_string(),
            DurabilityQosPolicyKind::Persistent => "PERSISTENT".to_string(),
        }
    }

    fn ownership_to_string(kind: OwnershipQosPolicyKind) -> String {
        match kind {
            OwnershipQosPolicyKind::Shared => "SHARED".to_string(),
            OwnershipQosPolicyKind::Exclusive => "EXCLUSIVE".to_string(),
        }
    }

    fn history_to_string(kind: HistoryQosPolicyKind) -> String {
        match kind {
            HistoryQosPolicyKind::KeepAll => "KEEP_ALL".to_string(),
            HistoryQosPolicyKind::KeepLast(_) => "KEEP_LAST".to_string(),
        }
    }

    fn verbosity_to_string(verbosity: Verbosity) -> String {
        match verbosity {
            Verbosity::Error => "ERROR".to_string(),
            Verbosity::Debug => "DEBUG".to_string(),
        }
    }

    fn data_representation_to_string(policy: &DataRepresentationQosPolicy) -> String {
        if policy.value.is_empty() {
            return "XCDR".to_string(); // default
        }

        let mut representations = Vec::new();
        for repr in &policy.value {
            match repr {
                DataRepresentationId::XcdrDataRepresentation => representations.push("XCDR"),
                DataRepresentationId::Xcdr2DataRepresentation => representations.push("XCDR2"),
                DataRepresentationId::XmlDataRepresentation => representations.push("XML"),
            }
        }
        representations.join(", ")
    }

    fn type_consistency_to_string(kind: TypeConsistencyKind) -> String {
        match kind {
            TypeConsistencyKind::DisallowTypeCoercion => "DISALLOW_TYPE_COERCION".to_string(),
            TypeConsistencyKind::AllowTypeCoercion => "ALLOW_TYPE_COERCION".to_string(),
        }
    }
}

/// DDS XTypes Interoperability Test Application
#[derive(Parser, Clone)]
#[command(author, version, about, long_about = None)]
#[command(allow_negative_numbers = true)]
struct TestOptions {
    /// Set log message verbosity
    #[arg(short = 'v', value_enum, default_value_t = Verbosity::Error)]
    verbosity: Verbosity,

    /// Publish samples
    #[arg(short = 'P', default_value_t = false)]
    publish: bool,

    /// Subscribe samples
    #[arg(short = 'S', default_value_t = false)]
    subscribe: bool,

    /// Domain id
    #[arg(short = 'd', default_value_t = 0)]
    domain_id: u32,

    /// BEST_EFFORT reliability
    #[arg(short = 'b', default_value_t = false)]
    best_effort: bool,

    /// RELIABLE reliability (default)
    #[arg(short = 'r', default_value_t = false)]
    reliable: bool,

    /// Keep history depth [0: KEEP_ALL]
    #[arg(short = 'k', default_value_t = -1)]
    history_depth: i32,

    /// Set a 'deadline' with interval (seconds) [0: OFF]
    #[arg(short = 'f', default_value_t = 0)]
    deadline_interval: i32,

    /// Apply 'time based filter' with interval (seconds) [0: OFF]
    #[arg(short = 'i', default_value_t = 0)]
    timebasedfilter_interval: i32,

    /// Set ownership strength [-1: SHARED]
    #[arg(short = 's', default_value_t = -1)]
    ownership_strength: i32,

    /// Set the topic name
    #[arg(short = 't')]
    topic_name: Option<String>,

    /// Set the type name
    #[arg(short = 'y')]
    type_name: Option<String>,

    /// Set a 'partition' string
    #[arg(short = 'p')]
    partition: Option<String>,

    /// Set durability (v=volatile, l=transient-local, t=transient, p=persistent)
    #[arg(short = 'D', default_value = "v")]
    durability: String,

    /// Set data representation [1: XCDR, 2: XCDR2]
    #[arg(short = 'x', default_value_t = 1)]
    data_representation: u8,

    /// Print Publisher's samples
    #[arg(short = 'w', default_value_t = false)]
    print_writer_samples: bool,

    /// Folder containing type definitions (eg: types)
    #[arg(long = "type-folder")]
    type_folder: Option<String>,

    /// Type definition file name without extension
    #[arg(long = "type-file")]
    type_file: Option<String>,

    /// Folder containing data samples (eg: data)
    #[arg(long = "data-folder")]
    data_folder: Option<String>,

    /// Data sample file name without extension
    #[arg(long = "data-file")]
    data_file: Option<String>,

    /// Enable, disable or default value for type_consistency.force_type_validation
    #[arg(long = "force-type-validation", value_enum)]
    force_type_validation: Option<TriState>,

    /// Enable, disable or default value for type_consistency.ignore_member_names
    #[arg(long = "ignore-member-names", value_enum)]
    ignore_member_names: Option<TriState>,

    /// Enable, disable or default value for type_consistency.ignore_sequence_bounds
    #[arg(long = "ignore-seq-bounds", value_enum)]
    ignore_seq_bounds: Option<TriState>,

    /// Enable, disable or default value for type_consistency.ignore_string_bounds
    #[arg(long = "ignore-str-bounds", value_enum)]
    ignore_str_bounds: Option<TriState>,

    /// Enable, disable or default value for type_consistency.prevent_type_widening
    #[arg(long = "prevent-type-widening", value_enum)]
    prevent_type_widening: Option<TriState>,

    /// Enable, disable type coercion or default value for type_consistency.kind
    #[arg(long = "allow-type-coercion", value_enum)]
    allow_type_coercion: Option<TriState>,

    /// Disable sending the type info for type assignability
    #[arg(long = "disable-type-info", default_value_t = false)]
    disable_type_info: bool,

    /// Set the Type Object version to use [1|2]
    #[arg(long = "type-object-version", default_value_t = 2)]
    type_object_version: u8,

    /// Print the equivalence hashes (TypeObjectV2) and exit
    #[arg(long = "print-typeid", default_value_t = false)]
    print_typeid: bool,

    /// Periodic participant announcement period in ms. Default 0 (off)
    #[arg(long = "periodic-announcement", default_value_t = 0)]
    periodic_announcement: u64,
}

impl TestOptions {
    fn validate_and_postprocess(&mut self) -> Result<(), i32> {
        let logger = get_logger();

        if self.topic_name.is_none() {
            self.topic_name = Some("test".to_string());
        }

        // Validation
        if self.type_name.is_none() {
            logger.log_message("please specify type name [-y]", Verbosity::Error);
            return Err(1);
        }
        if !self.publish && !self.subscribe {
            logger.log_message("please specify publish [-P] or subscribe [-S]", Verbosity::Error);
            return Err(1);
        }
        if self.publish && self.subscribe {
            logger.log_message(
                "please specify only one of: publish [-P] or subscribe [-S]",
                Verbosity::Error,
            );
            return Err(1);
        }
        if self.type_folder.is_none() || self.type_file.is_none() {
            logger.log_message(
                "please provide the types via --type-folder and --type-file",
                Verbosity::Error,
            );
            return Err(1);
        }

        // Validation for data representation
        match self.data_representation {
            1 | 2 => {} // Valid values: 1 = XCDR, 2 = XCDR2
            _ => {
                logger.log_message(
                    &format!(
                        "Invalid data representation: {}. Valid options are: 1 (XCDR), 2 (XCDR2)",
                        self.data_representation
                    ),
                    Verbosity::Error,
                );
                return Err(1);
            }
        }

        // Validation for durability: reject unrecognized values (empty keeps default)
        if let Some(c) = self.durability.chars().next() {
            if !matches!(c, 'v' | 'l' | 't' | 'p') {
                logger.log_message(
                    &format!("unrecognized value for durability {}", c),
                    Verbosity::Error,
                );
                return Err(1);
            }
        }

        // Validation for type object version
        match self.type_object_version {
            2 => {}
            1 => {
                logger.log_message("type object version 1 is not supported", Verbosity::Error);
                return Err(1);
            }
            _ => {
                logger.log_message(
                    &format!(
                        "unrecognized value for type object version {}",
                        self.type_object_version
                    ),
                    Verbosity::Error,
                );
                return Err(1);
            }
        }

        // Warnings
        self.print_warnings(logger);

        // Print configuration
        self.print_configuration(logger);

        Ok(())
    }

    fn print_warnings(&self, logger: &Logger) {
        if self.disable_type_info {
            logger.log_message(
                "warning: --disable-type-info is not supported, the type info is still sent",
                Verbosity::Error,
            );
        }
        if self.allow_type_coercion == Some(TriState::False)
            && (self.ignore_seq_bounds.is_some()
                || self.ignore_str_bounds.is_some()
                || self.ignore_member_names.is_some()
                || self.prevent_type_widening.is_some())
        {
            logger.log_message(
                "warning: prevent_type_widening, ignore_sequence_bounds, ignore_string_bounds, and ignore_member_names only apply when the type consistency kind is ALLOW_TYPE_COERCION",
                Verbosity::Error,
            );
        }
    }

    fn print_configuration(&self, logger: &Logger) {
        let app_kind = if self.publish { "publisher" } else { "subscriber" };
        let data_representation_policy = QosUtils::get_data_representation_policy(self);
        let type_consistency = QosUtils::get_type_consistency_policy(self);
        logger.log_message(&format!(
            "Test Options: \n    This application is a {}\n    DomainId = {}\n    ReliabilityKind = {}\n    DurabilityKind = {}\n    DataRepresentation = {}\n    HistoryDepth = {}\n    OwnershipStrength = {}\n    TimeBasedFilterInterval = {}\n    DeadlineInterval = {}\n    Type consistency kind = {}\n    Disable type info = {}\n    TypeObject version = {}\n    Print Type ID = {}\n    Periodic Announcement Period = {}ms\n    Verbosity = {}",
            app_kind,
            self.domain_id,
            QosUtils::reliability_to_string(QosUtils::get_reliability_policy(self).kind),
            QosUtils::durability_to_string(QosUtils::get_durability_policy(self).kind),
            QosUtils::data_representation_to_string(&data_representation_policy),
            self.history_depth,
            self.ownership_strength,
            self.timebasedfilter_interval,
            self.deadline_interval,
            QosUtils::type_consistency_to_string(type_consistency.kind),
            self.disable_type_info,
            self.type_object_version,
            self.print_typeid,
            self.periodic_announcement,
            QosUtils::verbosity_to_string(logger.verbosity)
        ), Verbosity::Debug);

        if !self.publish {
            logger.log_message(&format!(
                "    Force type validation = {}\n    Ignore member names = {}\n    Ignore sequence bounds = {}\n    Ignore string bounds = {}\n    Prevent type widening = {}",
                type_consistency.force_type_validation,
                type_consistency.ignore_member_names,
                type_consistency.ignore_sequence_bounds,
                type_consistency.ignore_string_bounds,
                type_consistency.prevent_type_widening
            ), Verbosity::Debug);
        }

        if let Some(ref topic_name) = self.topic_name {
            logger.log_message(&format!("    Topic = {}", topic_name), Verbosity::Debug);
        }
        if let Some(ref type_name) = self.type_name {
            logger.log_message(&format!("    Type  = {}", type_name), Verbosity::Debug);
        }
        if let Some(ref type_folder) = self.type_folder {
            logger.log_message(&format!("    Type Folder = {}", type_folder), Verbosity::Debug);
        }
        if let Some(ref type_file) = self.type_file {
            logger.log_message(&format!("    Type File = {}", type_file), Verbosity::Debug);
        }
        if let Some(ref data_folder) = self.data_folder {
            logger.log_message(&format!("    Data Folder = {}", data_folder), Verbosity::Debug);
        }
        if let Some(ref data_file) = self.data_file {
            logger.log_message(&format!("    Data File = {}", data_file), Verbosity::Debug);
        }
        if let Some(ref partition) = self.partition {
            logger.log_message(&format!("    Partition = {}", partition), Verbosity::Debug);
        }
    }

    fn get_type_file_path(&self) -> String {
        format!(
            "{}/xml/{}.xml",
            self.type_folder.as_deref().unwrap_or_default(),
            self.type_file.as_deref().unwrap_or_default()
        )
    }

    // A publisher without a data file writes an unpopulated sample.
    fn get_data_file_path(&self) -> Option<String> {
        match (&self.data_folder, &self.data_file) {
            (Some(folder), Some(file)) => Some(format!("{}/xml/{}.xml", folder, file)),
            _ => None,
        }
    }

    fn get_write_period_us(&self) -> u64 {
        1_000_000
    }

    fn get_read_period_us(&self) -> u64 {
        100_000
    }
}

// Publisher listener
struct PubListener;

impl DataWriterListener for PubListener {
    type Foo = DynamicData;

    fn on_offered_incompatible_qos(
        &self,
        writer: &int2dds::publication::data_writer::DataWriter<Self::Foo>,
        status: &int2dds::infrastructure::status::OfferedIncompatibleQosStatus,
    ) {
        let topic = writer.get_topic().unwrap();
        let topic_name = topic.get_name();
        let type_name = topic.get_type_name();
        println_flush!(
            "on_offered_incompatible_qos() topic: '{}'  type: '{}' : {:?} ({})",
            topic_name,
            type_name,
            status.last_policy_id(),
            status.last_policy_id().as_str()
        );
    }

    fn on_publication_matched(
        &self,
        writer: &int2dds::publication::data_writer::DataWriter<Self::Foo>,
        status: &int2dds::infrastructure::status::PublicationMatchedStatus,
    ) {
        let topic = writer.get_topic().unwrap();
        let topic_name = topic.get_name();
        let type_name = topic.get_type_name();
        READER_MATCHED.store(status.current_count() > 0, Ordering::SeqCst);
        println_flush!(
            "on_publication_matched() topic: '{}'  type: '{}' : matched readers {} (change = {})",
            topic_name,
            type_name,
            status.current_count(),
            status.current_count_change()
        );
    }

    fn on_offered_deadline_missed(
        &self,
        writer: &int2dds::publication::data_writer::DataWriter<Self::Foo>,
        status: &int2dds::infrastructure::status::OfferedDeadlineMissedStatus,
    ) {
        let topic = writer.get_topic().unwrap();
        let topic_name = topic.get_name();
        let type_name = topic.get_type_name();
        println_flush!(
            "on_offered_deadline_missed() topic: '{}'  type: '{}' : (total = {}, change = {})",
            topic_name,
            type_name,
            status.total_count(),
            status.total_count_change()
        );
    }

    fn on_liveliness_lost(
        &self,
        writer: &int2dds::publication::data_writer::DataWriter<Self::Foo>,
        status: &int2dds::infrastructure::status::LivelinessLostStatus,
    ) {
        let topic = writer.get_topic().unwrap();
        let topic_name = topic.get_name();
        let type_name = topic.get_type_name();
        println_flush!(
            "on_liveliness_lost() topic: '{}'  type: '{}' : (total = {}, change = {})",
            topic_name,
            type_name,
            status.total_count(),
            status.total_count_change()
        );
    }
}

// Subscriber listener
struct SubListener;

impl DataReaderListener for SubListener {
    type Foo = DynamicData;

    fn on_requested_incompatible_qos(
        &self,
        reader: &int2dds::subscription::data_reader::DataReader<Self::Foo>,
        status: &int2dds::infrastructure::status::RequestedIncompatibleQosStatus,
    ) {
        let topic_description = reader.get_topicdescription().unwrap();
        let topic_name = topic_description.get_name();
        let type_name = topic_description.get_type_name();
        println_flush!(
            "on_requested_incompatible_qos() topic: '{}'  type: '{}' : {:?} ({})",
            topic_name,
            type_name,
            status.last_policy_id(),
            status.last_policy_id().as_str()
        );
    }

    fn on_subscription_matched(
        &self,
        reader: &int2dds::subscription::data_reader::DataReader<Self::Foo>,
        status: &int2dds::infrastructure::status::SubscriptionMatchedStatus,
    ) {
        let topic_description = reader.get_topicdescription().unwrap();
        let topic_name = topic_description.get_name();
        let type_name = topic_description.get_type_name();
        println_flush!(
            "on_subscription_matched() topic: '{}'  type: '{}' : matched writers {} (change = {})",
            topic_name,
            type_name,
            status.current_count(),
            status.current_count_change()
        );
    }

    fn on_requested_deadline_missed(
        &self,
        reader: &int2dds::subscription::data_reader::DataReader<Self::Foo>,
        status: &int2dds::infrastructure::status::RequestedDeadlineMissedStatus,
    ) {
        let topic_description = reader.get_topicdescription().unwrap();
        let topic_name = topic_description.get_name();
        let type_name = topic_description.get_type_name();
        println_flush!(
            "on_requested_deadline_missed() topic: '{}'  type: '{}' : (total = {}, change = {})",
            topic_name,
            type_name,
            status.total_count(),
            status.total_count_change()
        );
    }

    fn on_liveliness_changed(
        &self,
        reader: &int2dds::subscription::data_reader::DataReader<Self::Foo>,
        status: &int2dds::infrastructure::status::LivelinessChangedStatus,
    ) {
        let topic_description = reader.get_topicdescription().unwrap();
        let topic_name = topic_description.get_name();
        let type_name = topic_description.get_type_name();
        println_flush!(
            "on_liveliness_changed() topic: '{}'  type: '{}' : (alive = {}, not_alive = {})",
            topic_name,
            type_name,
            status.alive_count(),
            status.not_alive_count()
        );
    }
}

// Topic listener
struct TestTopicListener;

impl TopicListener for TestTopicListener {
    fn on_inconsistent_topic(
        &self,
        topic: &int2dds::topic::topic::Topic,
        _status: &int2dds::infrastructure::status::InconsistentTopicStatus,
    ) {
        let topic_name = topic.get_name();
        let type_name = topic.get_type_name();
        println_flush!("on_inconsistent_topic() topic: '{}'  type: '{}'", topic_name, type_name);
    }
}

use std::sync::atomic::{AtomicBool, Ordering};

static SHOULD_EXIT: AtomicBool = AtomicBool::new(false);

// Set by PubListener while at least one DataReader is matched.
static READER_MATCHED: AtomicBool = AtomicBool::new(false);

fn install_signal_handlers() -> Result<(), Box<dyn std::error::Error>> {
    ctrlc::set_handler(move || {
        SHOULD_EXIT.store(true, Ordering::SeqCst);
    })?;
    Ok(())
}

// Sleeps in short steps so that SIGINT ends the wait early.
fn sleep_unless_exit(period_us: u64) {
    let mut remaining = period_us;
    while remaining > 0 && !SHOULD_EXIT.load(Ordering::SeqCst) {
        let step = remaining.min(100_000);
        std::thread::sleep(StdDuration::from_micros(step));
        remaining -= step;
    }
}

// TypeUtils implementation: XML type loading and TypeObject hashes
struct TypeUtils;

impl TypeUtils {
    fn load_registry(options: &TestOptions) -> Result<XmlTypeRegistry, DdsError> {
        XmlTypeRegistry::from_file(options.get_type_file_path())
    }

    fn load_type_support(options: &TestOptions) -> Result<DynamicTypeSupport, DdsError> {
        Self::load_registry(options)?.get(options.type_name.as_ref().unwrap())
    }

    fn print_typeid(options: &TestOptions) -> Result<(), DdsError> {
        let type_name = options.type_name.as_ref().unwrap();
        let registry = Self::load_registry(options)?;
        let (complete, dependencies) = registry
            .get_type_object_with_deps(type_name)
            .ok_or_else(|| DdsError::Error(format!("XML types: type '{}' not found", type_name)))?;

        let complete_hash = TypeObject::Complete(complete.clone()).compute_hash();
        let mut closure = vec![(TypeIdentifier::CompleteTypeId(complete_hash), complete)];
        for (type_identifier, type_object) in dependencies {
            if let TypeObject::Complete(dependency) = type_object {
                if type_identifier.equivalence_hash() != Some(&complete_hash) {
                    closure.push((type_identifier, dependency));
                }
            }
        }
        let minimal_hash = build_minimal_closure(&closure)
            .into_iter()
            .find(|(complete_key, _, _)| *complete_key == complete_hash)
            .map(|(_, minimal_hash, _)| minimal_hash)
            .ok_or_else(|| {
                DdsError::Error(format!("failed to derive the minimal type of '{}'", type_name))
            })?;

        println_flush!(
            "Complete Type Object V2 - Equivalence Hash: {}",
            Self::hash_to_string(complete_hash.as_bytes())
        );
        println_flush!(
            "Minimal Type Object V2 - Equivalence Hash: {}",
            Self::hash_to_string(minimal_hash.as_bytes())
        );
        Ok(())
    }

    fn hash_to_string(hash: &[u8]) -> String {
        hash.iter().map(|byte| format!("{:02x}", byte)).collect()
    }
}

type XmlNode<'a, 'input> = roxmltree::Node<'a, 'input>;

// DataUtils implementation: XML sample data to DynamicData, and comparison
struct DataUtils;

impl DataUtils {
    // The root element name is not significant; its children are the members.
    // Members missing in the XML take the default value of their type.
    fn load_sample(
        xml: Option<&str>,
        dynamic_type: &Arc<DynamicType>,
    ) -> Result<DynamicData, String> {
        match xml {
            Some(text) => {
                let document = roxmltree::Document::parse(text).map_err(|e| e.to_string())?;
                Self::struct_from_node(Some(document.root_element()), dynamic_type)
            }
            None => Self::struct_from_node(None, dynamic_type),
        }
    }

    fn struct_from_node(
        node: Option<XmlNode>,
        dynamic_type: &Arc<DynamicType>,
    ) -> Result<DynamicData, String> {
        let mut data = DynamicData::new(dynamic_type.clone());
        // A union-typed sample holds its discriminator and the selected branch by name.
        if dynamic_type.as_union().is_some() {
            let (discriminator, member_name, value) =
                Self::union_case_from_node(node, dynamic_type.kind())?;
            data.set_value(UNION_DISCRIMINATOR_FIELD, discriminator).map_err(|e| e.to_string())?;
            data.set_value(&member_name, value).map_err(|e| e.to_string())?;
            return Ok(data);
        }
        let members = dynamic_type.members().ok_or_else(|| {
            format!(
                "type '{}' is not a struct or union, DynamicData cannot hold it",
                dynamic_type.type_name()
            )
        })?;

        for member in members {
            let child = node.and_then(|n| Self::child(n, &member.name));
            if child.is_none() && member.is_optional {
                continue;
            }
            let value = Self::value_from_node(child, &member.member_type)
                .map_err(|e| format!("{}: {}", member.name, e))?;
            data.set_value(&member.name, value).map_err(|e| e.to_string())?;
        }
        Ok(data)
    }

    fn value_from_node(
        node: Option<XmlNode>,
        kind: &DynamicTypeKind,
    ) -> Result<DynamicValue, String> {
        match kind {
            DynamicTypeKind::Primitive(primitive) => {
                Self::parse_primitive(*primitive, node.map(Self::text).unwrap_or_default().trim())
            }
            DynamicTypeKind::String { .. } => {
                Ok(DynamicValue::String(node.map(Self::text).unwrap_or_default().to_string()))
            }
            DynamicTypeKind::WString { .. } => {
                Ok(DynamicValue::WString(node.map(Self::text).unwrap_or_default().to_string()))
            }
            DynamicTypeKind::Sequence { element_type, .. } => {
                let mut items = Vec::new();
                if let Some(n) = node {
                    for item in Self::items(n) {
                        items.push(Self::value_from_node(Some(item), element_type)?);
                    }
                }
                Ok(DynamicValue::Sequence(items))
            }
            DynamicTypeKind::Array { element_type, dimensions } => {
                let total: usize = dimensions.iter().map(|d| *d as usize).product();
                let mut nodes = Vec::new();
                if let Some(n) = node {
                    Self::collect_array_items(n, dimensions.len(), &mut nodes);
                }
                if nodes.len() > total {
                    return Err(format!("array of {} elements has {} items", total, nodes.len()));
                }
                let mut items = Vec::with_capacity(total);
                for item in nodes {
                    items.push(Self::value_from_node(Some(item), element_type)?);
                }
                while items.len() < total {
                    items.push(Self::value_from_node(None, element_type)?);
                }
                Ok(DynamicValue::Array(items))
            }
            DynamicTypeKind::Enum(desc) => Self::enum_from_node(node, desc),
            DynamicTypeKind::Union(_) => Self::union_from_node(node, kind),
            DynamicTypeKind::Bitmask(_) => Ok(DynamicValue::Bitmask(Self::bits_from_node(node)?)),
            DynamicTypeKind::Bitset(_) => Ok(DynamicValue::Bitset(Self::bits_from_node(node)?)),
            DynamicTypeKind::TypeRef(inner) => match inner.kind() {
                DynamicTypeKind::Struct(_) => {
                    Ok(DynamicValue::Struct(Box::new(Self::struct_from_node(node, inner)?)))
                }
                other => Self::value_from_node(node, other),
            },
            DynamicTypeKind::Struct(_)
            | DynamicTypeKind::Map { .. }
            | DynamicTypeKind::ExternalType { .. } => {
                Err("unsupported member type in XML sample data".to_string())
            }
        }
    }

    fn enum_from_node(
        node: Option<XmlNode>,
        desc: &EnumDescriptor,
    ) -> Result<DynamicValue, String> {
        let text = node.map(Self::text).unwrap_or_default().trim();
        if text.is_empty() {
            let literal = desc.default_literal().or_else(|| desc.literals().first());
            return Ok(DynamicValue::Enum {
                name: literal.map(|l| l.name.clone()).unwrap_or_default(),
                value: literal.map(|l| l.value).unwrap_or(0),
            });
        }
        if let Ok(number) = Self::parse_integer(text) {
            let value =
                i32::try_from(number).map_err(|_| format!("enum value {} out of range", number))?;
            let name = desc.get_literal_by_value(value).map(|l| l.name.clone()).unwrap_or_default();
            return Ok(DynamicValue::Enum { name, value });
        }
        let literal =
            desc.get_literal(text).ok_or_else(|| format!("unknown enum literal '{}'", text))?;
        Ok(DynamicValue::Enum { name: literal.name.clone(), value: literal.value })
    }

    fn union_from_node(
        node: Option<XmlNode>,
        kind: &DynamicTypeKind,
    ) -> Result<DynamicValue, String> {
        let (discriminator, _, value) = Self::union_case_from_node(node, kind)?;
        Ok(DynamicValue::Union { discriminator: Box::new(discriminator), value: Box::new(value) })
    }

    fn union_case_from_node(
        node: Option<XmlNode>,
        kind: &DynamicTypeKind,
    ) -> Result<(DynamicValue, Arc<str>, DynamicValue), String> {
        let DynamicTypeKind::Union(desc) = kind else {
            return Err("expected a union type".to_string());
        };

        let discriminator = match node.and_then(|n| Self::child(n, "discriminator")) {
            Some(d) => Self::parse_discriminator(Self::text(d).trim(), desc.discriminator_type())?,
            // Without a discriminator the case is the one whose member is present.
            None => node
                .and_then(|n| desc.members().iter().find(|m| Self::child(n, &m.name).is_some()))
                .or_else(|| desc.select_member(0))
                .or_else(|| desc.members().first())
                .and_then(|m| m.labels.first().copied())
                .map(i64::from)
                .unwrap_or(0),
        };
        let member = desc
            .select_member(discriminator)
            .ok_or_else(|| format!("no union member for discriminator {}", discriminator))?;
        let value = Self::value_from_node(
            node.and_then(|n| Self::child(n, &member.name)),
            &member.member_type,
        )
        .map_err(|e| format!("{}: {}", member.name, e))?;

        Ok((
            Self::discriminator_value(desc.discriminator_type(), discriminator)?,
            member.name.clone(),
            value,
        ))
    }

    fn parse_discriminator(text: &str, kind: &DynamicTypeKind) -> Result<i64, String> {
        if let Ok(number) = Self::parse_integer(text) {
            return i64::try_from(number)
                .map_err(|_| format!("union discriminator {} out of range", number));
        }
        match kind {
            DynamicTypeKind::TypeRef(inner) => Self::parse_discriminator(text, inner.kind()),
            DynamicTypeKind::Enum(desc) => desc
                .get_literal(text)
                .map(|l| i64::from(l.value))
                .ok_or_else(|| format!("unknown enum literal '{}'", text)),
            _ => Err(format!("invalid union discriminator '{}'", text)),
        }
    }

    fn discriminator_value(kind: &DynamicTypeKind, value: i64) -> Result<DynamicValue, String> {
        match kind {
            DynamicTypeKind::Primitive(primitive) => {
                Self::integer_value(*primitive, i128::from(value))
            }
            DynamicTypeKind::Enum(desc) => {
                let value = i32::try_from(value)
                    .map_err(|_| format!("enum value {} out of range", value))?;
                let name =
                    desc.get_literal_by_value(value).map(|l| l.name.clone()).unwrap_or_default();
                Ok(DynamicValue::Enum { name, value })
            }
            DynamicTypeKind::Bitmask(_) => Ok(DynamicValue::Bitmask(value as u64)),
            DynamicTypeKind::TypeRef(inner) => Self::discriminator_value(inner.kind(), value),
            _ => Err("unsupported union discriminator type".to_string()),
        }
    }

    fn bits_from_node(node: Option<XmlNode>) -> Result<u64, String> {
        let text = node.map(Self::text).unwrap_or_default().trim();
        if text.is_empty() {
            return Ok(0);
        }
        let number = Self::parse_integer(text)?;
        u64::try_from(number).map_err(|_| format!("value {} out of range", number))
    }

    fn parse_primitive(primitive: PrimitiveKind, text: &str) -> Result<DynamicValue, String> {
        match primitive {
            PrimitiveKind::Boolean => match text.to_ascii_lowercase().as_str() {
                "" | "false" | "0" => Ok(DynamicValue::Boolean(false)),
                "true" | "1" => Ok(DynamicValue::Boolean(true)),
                _ => Err(format!("invalid boolean '{}'", text)),
            },
            PrimitiveKind::Float32 if !text.is_empty() => text
                .parse()
                .map(DynamicValue::Float32)
                .map_err(|e| format!("invalid float32 '{}': {}", text, e)),
            PrimitiveKind::Float64 if !text.is_empty() => text
                .parse()
                .map(DynamicValue::Float64)
                .map_err(|e| format!("invalid float64 '{}': {}", text, e)),
            // Parsed straight to binary128: peers compare all 16 bytes against their own parse.
            PrimitiveKind::Float128 if !text.is_empty() => text
                .parse()
                .map(DynamicValue::Float128)
                .map_err(|e| format!("invalid float128 '{}': {}", text, e)),
            // A char is either a 0x-prefixed code or a plain character.
            PrimitiveKind::Char8 | PrimitiveKind::Char16 if !Self::is_hex(text) => {
                Ok(DynamicValue::Char8(text.chars().next().unwrap_or('\0')))
            }
            _ if text.is_empty() => Self::integer_value(primitive, 0),
            _ => Self::integer_value(primitive, Self::parse_integer(text)?),
        }
    }

    fn integer_value(primitive: PrimitiveKind, value: i128) -> Result<DynamicValue, String> {
        let out_of_range = |_| format!("value {} out of range for {:?}", value, primitive);
        Ok(match primitive {
            PrimitiveKind::Boolean => DynamicValue::Boolean(value != 0),
            PrimitiveKind::Int8 => DynamicValue::Int8(i8::try_from(value).map_err(out_of_range)?),
            PrimitiveKind::Int16 => {
                DynamicValue::Int16(i16::try_from(value).map_err(out_of_range)?)
            }
            PrimitiveKind::Int32 => {
                DynamicValue::Int32(i32::try_from(value).map_err(out_of_range)?)
            }
            PrimitiveKind::Int64 => {
                DynamicValue::Int64(i64::try_from(value).map_err(out_of_range)?)
            }
            PrimitiveKind::Uint8 => DynamicValue::Uint8(u8::try_from(value).map_err(out_of_range)?),
            PrimitiveKind::Uint16 => {
                DynamicValue::Uint16(u16::try_from(value).map_err(out_of_range)?)
            }
            PrimitiveKind::Uint32 => {
                DynamicValue::Uint32(u32::try_from(value).map_err(out_of_range)?)
            }
            PrimitiveKind::Uint64 => {
                DynamicValue::Uint64(u64::try_from(value).map_err(out_of_range)?)
            }
            PrimitiveKind::Byte => DynamicValue::Byte(u8::try_from(value).map_err(out_of_range)?),
            PrimitiveKind::Char8 => {
                DynamicValue::Char8(u8::try_from(value).map_err(out_of_range)? as char)
            }
            PrimitiveKind::Char16 => DynamicValue::Char8(
                u16::try_from(value)
                    .ok()
                    .and_then(|v| char::from_u32(u32::from(v)))
                    .ok_or_else(|| format!("value {} out of range for Char16", value))?,
            ),
            PrimitiveKind::Float32 => DynamicValue::Float32(value as f32),
            PrimitiveKind::Float64 => DynamicValue::Float64(value as f64),
            PrimitiveKind::Float128 => DynamicValue::Float128(
                value.to_string().parse::<Float128>().map_err(|e| e.to_string())?,
            ),
        })
    }

    // Decimal or 0x-prefixed hexadecimal, with an optional sign.
    fn parse_integer(text: &str) -> Result<i128, String> {
        let (negative, digits) = match text.strip_prefix('-') {
            Some(rest) => (true, rest),
            None => (false, text.strip_prefix('+').unwrap_or(text)),
        };
        let magnitude = match digits.strip_prefix("0x").or_else(|| digits.strip_prefix("0X")) {
            Some(hex) => i128::from_str_radix(hex, 16),
            None => digits.parse::<i128>(),
        }
        .map_err(|e| format!("invalid integer '{}': {}", text, e))?;
        Ok(if negative { -magnitude } else { magnitude })
    }

    fn is_hex(text: &str) -> bool {
        text.len() > 2 && (text.starts_with("0x") || text.starts_with("0X"))
    }

    fn child<'a, 'input>(node: XmlNode<'a, 'input>, name: &str) -> Option<XmlNode<'a, 'input>> {
        node.children().find(|c| c.is_element() && c.tag_name().name() == name)
    }

    fn items<'a, 'input>(node: XmlNode<'a, 'input>) -> impl Iterator<Item = XmlNode<'a, 'input>> {
        node.children().filter(|c| c.is_element() && c.tag_name().name() == "item")
    }

    // Multi-dimensional arrays are accepted either flat or as nested <item> levels.
    fn collect_array_items<'a, 'input>(
        node: XmlNode<'a, 'input>,
        depth: usize,
        out: &mut Vec<XmlNode<'a, 'input>>,
    ) {
        for item in Self::items(node) {
            if depth > 1 && Self::items(item).next().is_some() {
                Self::collect_array_items(item, depth - 1, out);
            } else {
                out.push(item);
            }
        }
    }

    fn text<'a>(node: XmlNode<'a, '_>) -> &'a str {
        node.text().unwrap_or_default()
    }

    // Both samples are of the local type; a member missing on either side
    // counts as the default value of its type.
    fn same_data(expected: &DynamicData, received: &DynamicData) -> bool {
        if expected.dynamic_type().as_union().is_some() {
            return expected
                .iter_fields()
                .all(|(name, e)| received.get_value(name).is_some_and(|r| Self::same_value(e, r)));
        }
        let Some(members) = expected.dynamic_type().members() else {
            return false;
        };
        members.iter().all(|member| {
            let default = || Self::value_from_node(None, &member.member_type).ok();
            let expected_value = expected.get_value(&member.name).cloned().or_else(default);
            let received_value = received.get_value(&member.name).cloned().or_else(default);
            match (expected_value, received_value) {
                (Some(e), Some(r)) => Self::same_value(&e, &r),
                _ => false,
            }
        })
    }

    fn same_value(expected: &DynamicValue, received: &DynamicValue) -> bool {
        match (expected, received) {
            (DynamicValue::Struct(e), DynamicValue::Struct(r)) => Self::same_data(e, r),
            (DynamicValue::Enum { value: e, .. }, DynamicValue::Enum { value: r, .. }) => e == r,
            (
                DynamicValue::Union { discriminator: ed, value: ev },
                DynamicValue::Union { discriminator: rd, value: rv },
            ) => Self::same_value(ed, rd) && Self::same_value(ev, rv),
            (DynamicValue::Sequence(e), DynamicValue::Sequence(r))
            | (DynamicValue::Array(e), DynamicValue::Array(r)) => {
                e.len() == r.len() && e.iter().zip(r).all(|(e, r)| Self::same_value(e, r))
            }
            (DynamicValue::Float32(e), DynamicValue::Float32(r)) => e.to_bits() == r.to_bits(),
            (DynamicValue::Float64(e), DynamicValue::Float64(r)) => e.to_bits() == r.to_bits(),
            // Float128 equality is bitwise, so this also checks the full binary128 value.
            (e, r) => e == r,
        }
    }

    fn data_to_string(data: &DynamicData) -> String {
        let fields: Vec<String> = match data.dynamic_type().members() {
            Some(members) => members
                .iter()
                .filter_map(|m| {
                    data.get_value(&m.name)
                        .map(|v| format!("{}: {}", m.name, Self::value_to_string(v)))
                })
                .collect(),
            None => data
                .iter_fields()
                .map(|(name, v)| format!("{}: {}", name, Self::value_to_string(v)))
                .collect(),
        };
        format!("{{ {} }}", fields.join(", "))
    }

    fn value_to_string(value: &DynamicValue) -> String {
        match value {
            DynamicValue::Struct(inner) => Self::data_to_string(inner),
            DynamicValue::Sequence(items) | DynamicValue::Array(items) => {
                let items: Vec<String> = items.iter().map(Self::value_to_string).collect();
                format!("[{}]", items.join(", "))
            }
            DynamicValue::Union { discriminator, value } => format!(
                "{{ discriminator: {}, value: {} }}",
                Self::value_to_string(discriminator),
                Self::value_to_string(value)
            ),
            DynamicValue::Char8(c) => format!("{:?}", c),
            DynamicValue::String(s) | DynamicValue::WString(s) => format!("{:?}", s),
            other => other.to_string(),
        }
    }
}

// Test application
struct TestApplication {
    participant: Option<Arc<int2dds::domain::domain_participant::DomainParticipant>>,
    topic: Option<Arc<int2dds::topic::topic::Topic>>,
    publisher: Option<Arc<int2dds::publication::publisher::Publisher>>,
    subscriber: Option<Arc<int2dds::subscription::subscriber::Subscriber>>,
    data_writer: Option<Arc<int2dds::publication::data_writer::DataWriter<DynamicData>>>,
    data_reader: Option<Arc<int2dds::subscription::data_reader::DataReader<DynamicData>>>,
    type_support: Option<Arc<DynamicTypeSupport>>,
    data_xml: Option<String>,
}

impl TestApplication {
    fn new() -> Self {
        Self {
            participant: None,
            topic: None,
            publisher: None,
            subscriber: None,
            data_writer: None,
            data_reader: None,
            type_support: None,
            data_xml: None,
        }
    }

    fn initialize(&mut self, options: &TestOptions) -> Result<(), DdsError> {
        let logger = get_logger();
        logger.log_message("Running initialize() function", Verbosity::Debug);

        let type_support = Arc::new(TypeUtils::load_type_support(options)?);
        self.type_support = Some(type_support.clone());

        // The data file is only read here; it is converted once it is needed.
        if let Some(path) = options.get_data_file_path() {
            let xml = std::fs::read_to_string(&path).map_err(|e| {
                DdsError::Error(format!("Failed to read data file {}: {}", path, e))
            })?;
            self.data_xml = Some(xml);
        }

        let factory = DomainParticipantFactory::get_instance();
        let participant_qos = DomainParticipantQos::default();
        let participant = factory.create_participant(
            options.domain_id.try_into().unwrap(),
            participant_qos,
            None,
            StatusMask::default(),
        )?;
        let participant_arc = Arc::new(participant);
        self.participant = Some(participant_arc.clone());

        // Create topic
        let topic_name = options.topic_name.as_ref().unwrap();
        println_flush!("Create topic: {}", topic_name);
        let topic_listener = TestTopicListener;
        let topic = participant_arc.create_topic_dynamic(
            topic_name,
            type_support,
            TopicQos::default(),
            Some(Arc::new(topic_listener)),
            StatusMask::default(),
        )?;
        self.topic = Some(Arc::new(topic));

        if options.publish {
            self.init_publisher(options)?;
        } else {
            self.init_subscriber(options)?;
        }

        Ok(())
    }

    fn run(&self, options: &TestOptions) -> Result<(), DdsError> {
        let logger = get_logger();
        logger.log_message("Running run() function", Verbosity::Debug);

        if options.publish {
            self.run_publisher(options)
        } else {
            self.run_subscriber(options)
        }
    }

    fn cleanup(&mut self) -> Result<(), DdsError> {
        if let Some(participant_arc) = self.participant.take() {
            participant_arc.delete_contained_entities()?;

            self.data_writer = None;
            self.data_reader = None;
            self.publisher = None;
            self.subscriber = None;
            self.topic = None;

            let participant = (*participant_arc).clone();
            drop(participant_arc);
            DomainParticipantFactory::get_instance().delete_participant(participant)?;
        }
        Ok(())
    }

    fn init_publisher(&mut self, options: &TestOptions) -> Result<(), DdsError> {
        let logger = get_logger();
        logger.log_message("Running init_publisher() function", Verbosity::Debug);

        let participant = self.participant.as_ref().unwrap();
        let topic = self.topic.as_ref().unwrap();
        let type_support = self.type_support.as_ref().unwrap();

        // Create publisher
        let mut publisher_qos = PublisherQos::default();
        if let Some(ref partition) = options.partition {
            publisher_qos.partition = PartitionQosPolicy { name: vec![partition.clone()] };
        }

        let publisher = participant.create_publisher(publisher_qos, None, StatusMask::default())?;
        let publisher_arc = Arc::new(publisher);
        self.publisher = Some(publisher_arc.clone());

        // Create data writer
        let writer_qos = DataWriterQos {
            reliability: QosUtils::get_reliability_policy(options),
            durability: QosUtils::get_durability_policy(options),
            ownership: QosUtils::get_ownership_policy(options),
            ownership_strength: QosUtils::get_ownership_strength_policy(options),
            history: QosUtils::get_history_policy(options),
            deadline: QosUtils::get_deadline_policy(options),
            data_representation: QosUtils::get_data_representation_policy(options),
            ..DataWriterQos::default()
        };

        logger.log_message("Data Writer QoS:", Verbosity::Debug);
        logger.log_message(
            &format!(
                "    Reliability = {}",
                QosUtils::reliability_to_string(writer_qos.reliability.kind)
            ),
            Verbosity::Debug,
        );
        logger.log_message(
            &format!(
                "    Durability = {}",
                QosUtils::durability_to_string(writer_qos.durability.kind)
            ),
            Verbosity::Debug,
        );
        logger.log_message(
            &format!(
                "    Data_Representation = {}",
                QosUtils::data_representation_to_string(&writer_qos.data_representation)
            ),
            Verbosity::Debug,
        );
        logger.log_message(
            &format!(
                "    Ownership = {}",
                QosUtils::ownership_to_string(writer_qos.ownership.kind)
            ),
            Verbosity::Debug,
        );
        logger.log_message(
            &format!("    History = {}", QosUtils::history_to_string(writer_qos.history.kind)),
            Verbosity::Debug,
        );

        println_flush!(
            "Create writer for topic: {} type: {}",
            topic.get_name(),
            options.type_name.as_ref().unwrap()
        );
        let listener = PubListener;
        let writer = publisher_arc.create_datawriter_dynamic(
            topic,
            type_support.clone(),
            writer_qos,
            Some(Arc::new(listener)),
            StatusMask::default(),
        )?;
        self.data_writer = Some(Arc::new(writer));

        logger.log_message("Data Writer created", Verbosity::Debug);

        Ok(())
    }

    fn init_subscriber(&mut self, options: &TestOptions) -> Result<(), DdsError> {
        let logger = get_logger();
        logger.log_message("Running init_subscriber() function", Verbosity::Debug);

        let participant = self.participant.as_ref().unwrap();
        let topic = self.topic.as_ref().unwrap();
        let type_support = self.type_support.as_ref().unwrap();

        // Create subscriber
        let mut subscriber_qos = SubscriberQos::default();
        if let Some(ref partition) = options.partition {
            subscriber_qos.partition = PartitionQosPolicy { name: vec![partition.to_owned()] };
        }

        let subscriber =
            participant.create_subscriber(subscriber_qos, None, StatusMask::default())?;
        let subscriber_arc = Arc::new(subscriber);
        self.subscriber = Some(subscriber_arc.clone());

        // Create data reader
        let data_reader_qos = DataReaderQos {
            reliability: QosUtils::get_reliability_policy(options),
            durability: QosUtils::get_durability_policy(options),
            ownership: QosUtils::get_ownership_policy(options),
            history: QosUtils::get_history_policy(options),
            deadline: QosUtils::get_deadline_policy(options),
            data_representation: QosUtils::get_data_representation_policy(options),
            time_based_filter: QosUtils::get_time_based_filter_policy(options),
            type_consistency_enforcement: QosUtils::get_type_consistency_policy(options),
            ..DataReaderQos::default()
        };

        logger.log_message("Data Reader QoS:", Verbosity::Debug);
        logger.log_message(
            &format!(
                "    Reliability = {}",
                QosUtils::reliability_to_string(data_reader_qos.reliability.kind)
            ),
            Verbosity::Debug,
        );
        logger.log_message(
            &format!(
                "    Durability = {}",
                QosUtils::durability_to_string(data_reader_qos.durability.kind)
            ),
            Verbosity::Debug,
        );
        logger.log_message(
            &format!(
                "    DataRepresentation = {}",
                QosUtils::data_representation_to_string(&data_reader_qos.data_representation)
            ),
            Verbosity::Debug,
        );
        logger.log_message(
            &format!(
                "    Ownership = {}",
                QosUtils::ownership_to_string(data_reader_qos.ownership.kind)
            ),
            Verbosity::Debug,
        );
        logger.log_message(
            &format!("    History = {}", QosUtils::history_to_string(data_reader_qos.history.kind)),
            Verbosity::Debug,
        );
        let type_consistency = &data_reader_qos.type_consistency_enforcement;
        logger.log_message(
            &format!(
                "    TypeConsistency * kind = {}\n                    * ignore_sequence_bounds = {}\n                    * ignore_string_bounds   = {}\n                    * ignore_member_names    = {}\n                    * prevent_type_widening  = {}\n                    * force_type_validation  = {}",
                QosUtils::type_consistency_to_string(type_consistency.kind),
                type_consistency.ignore_sequence_bounds,
                type_consistency.ignore_string_bounds,
                type_consistency.ignore_member_names,
                type_consistency.prevent_type_widening,
                type_consistency.force_type_validation
            ),
            Verbosity::Debug,
        );

        println_flush!("Create reader for topic: {}", topic.get_name());

        let listener = SubListener;
        let reader = subscriber_arc.create_datareader_dynamic(
            topic.as_ref(),
            type_support.clone(),
            data_reader_qos,
            Some(Arc::new(listener)),
            StatusMask::default(),
        )?;
        self.data_reader = Some(Arc::new(reader));

        logger.log_message("Data Reader created", Verbosity::Debug);

        Ok(())
    }

    fn run_publisher(&self, options: &TestOptions) -> Result<(), DdsError> {
        let logger = get_logger();
        logger.log_message("Running run_publisher() function", Verbosity::Debug);

        let writer = self.data_writer.as_ref().ok_or(DdsError::PreconditionNotMet)?;
        let dynamic_type = self.type_support.as_ref().unwrap().dynamic_type();

        // The sample is built on the first match, so a type whose data cannot be
        // represented still goes through discovery and type matching.
        let mut sample: Option<DynamicData> = None;
        let mut n = 0;

        loop {
            if sample.is_none() && READER_MATCHED.load(Ordering::SeqCst) {
                let data = DataUtils::load_sample(self.data_xml.as_deref(), dynamic_type)
                    .map_err(|e| DdsError::Error(format!("Error initializing data: {}", e)))?;
                sample = Some(data);
            }

            if let Some(ref data) = sample {
                writer.write(data, InstanceHandle::NIL)?;

                if options.print_writer_samples {
                    println_flush!(" Wrote:");
                    println_flush!("{}", DataUtils::data_to_string(data));
                }
            }

            sleep_unless_exit(options.get_write_period_us());

            n += 1;
            logger.log_message(&format!("Publisher iteration: <{}>", n), Verbosity::Debug);

            if SHOULD_EXIT.load(Ordering::SeqCst) {
                break;
            }
        }

        Ok(())
    }

    fn run_subscriber(&self, options: &TestOptions) -> Result<(), DdsError> {
        let logger = get_logger();
        logger.log_message("Running run_subscriber() function", Verbosity::Debug);

        let reader = self.data_reader.as_ref().ok_or(DdsError::PreconditionNotMet)?;
        let dynamic_type = self.type_support.as_ref().unwrap().dynamic_type();

        // The expected sample is built on the first received one, see run_publisher().
        let mut expected: Option<Result<DynamicData, String>> = None;
        let mut n = 0;

        loop {
            loop {
                if SHOULD_EXIT.load(Ordering::SeqCst) {
                    break;
                }

                logger.log_message("Calling take() function", Verbosity::Debug);

                let result = reader.take(
                    LENGTH_UNLIMITED,
                    &[SampleStateKind::ANY_SAMPLE_STATE],
                    &[ViewStateKind::ANY_VIEW_STATE],
                    &[InstanceStateKind::ANY_INSTANCE_STATE],
                );

                match result {
                    Ok(samples) => {
                        if samples.is_empty() {
                            break;
                        }

                        logger.log_message(
                            &format!("Read {} sample(s), printing them...", samples.len()),
                            Verbosity::Debug,
                        );

                        for sample in samples.iter() {
                            if !sample.sample_info().valid_data {
                                continue;
                            }

                            // A sample that cannot be constructed is dropped without
                            // reporting it as received. The harness reads any "[N]" on
                            // stdout as a received sample, so the error is debug only.
                            let data = match sample.data() {
                                Ok(d) => d,
                                Err(e) => {
                                    logger.log_message(
                                        &format!("warning: deserialize failed: {}", e),
                                        Verbosity::Debug,
                                    );
                                    continue;
                                }
                            };

                            println_flush!("sample_received()");
                            println_flush!("{}", DataUtils::data_to_string(&data));

                            let loaded = expected.get_or_insert_with(|| {
                                DataUtils::load_sample(self.data_xml.as_deref(), dynamic_type)
                            });
                            match loaded {
                                Ok(loaded) if DataUtils::same_data(loaded, &data) => {
                                    println_flush!("Received sample is the same as loaded");
                                }
                                Ok(_) => {
                                    println_flush!("Received sample is not the same as loaded");
                                }
                                Err(e) => {
                                    logger.log_message(
                                        &format!("Error initializing data: {}", e),
                                        Verbosity::Error,
                                    );
                                    println_flush!("Received sample is not the same as loaded");
                                }
                            }
                        }
                    }
                    Err(_) => break,
                }
            }

            n += 1;
            logger.log_message(&format!("Subscriber iteration: <{}>", n), Verbosity::Debug);

            if SHOULD_EXIT.load(Ordering::SeqCst) {
                break;
            }

            std::thread::sleep(StdDuration::from_micros(options.get_read_period_us()));
        }

        Ok(())
    }
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let _ = install_signal_handlers();

    // The suite expects Connext key matching, which skips the XTypes Table 19 key rules.
    set_xtypes_rti_key_compat(true);

    let mut options = match TestOptions::try_parse() {
        Ok(options) => options,
        Err(e) => {
            let _ = e.print();
            process::exit(if e.use_stderr() { 1 } else { 0 });
        }
    };

    init_logger(options.verbosity);

    // Post-process options to match original behavior
    if let Err(code) = options.validate_and_postprocess() {
        process::exit(code);
    }

    let logger = get_logger();

    if options.print_typeid {
        if let Err(e) = TypeUtils::print_typeid(&options) {
            eprintln!("Error printing type id: {}", e);
            process::exit(2);
        }
        return Ok(());
    }

    logger.log_message("Initializing TestApp...", Verbosity::Debug);

    let mut test_app = TestApplication::new();

    if let Err(e) = test_app.initialize(&options) {
        eprintln!("Error initializing application: {}", e);
        process::exit(2);
    }

    logger.log_message("Running TestApp...", Verbosity::Debug);

    test_app.run(&options).unwrap_or_else(|e| {
        eprintln!("Error running application: {}", e);
        process::exit(3);
    });

    if let Err(e) = test_app.cleanup() {
        eprintln!("Error during cleanup: {}", e);
    }

    println_flush!("Done.");
    Ok(())
}
