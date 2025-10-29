
#include <dds/dds.hh>
#include <dds/dds_typesupport.hh>
#include <dds/xtypes.hh>

#define DDS_BOOLEAN_TRUE         (1)
#define DDS_BOOLEAN_FALSE        (0)
#define LISTENER_STATUS_MASK_ALL (ALL_STATUS)


void StringSeq_push(DDS::StringSeq  &string_seq, const char *elem)
{
  char * e = NULL;
  if ( elem )
    {
      e = new char[strlen(elem)+1];
      if ( e )
        {
          strcpy( e, elem );
          string_seq.push_back(e);
        }
    }
}

const char *get_qos_policy_name(DDS_QosPolicyId_t policy_id)
{
  return DDS_qos_policy_str(policy_id);
}

DDS::TypeConsistencyEnforcementQosPolicy
TypeConsistency_get_default(void) {
  DDS::TypeConsistencyEnforcementQosPolicy rval;
  DDS::DomainParticipantFactory *dpf =  DDS::DomainParticipantFactory::get_instance();
  if ( dpf )
    {
      DDS::DomainParticipantFactoryQos dpf_qos;
      dpf->get_qos( dpf_qos );
      dpf_qos.entity_factory.autoenable_created_entities = 0;
      dpf->set_qos( dpf_qos );
  
      DDS::DomainParticipant * dp = dpf->create_participant( 0, DDS::PARTICIPANT_QOS_DEFAULT, NULL, 0 );
      if ( dp )
        {
          DDS::Subscriber * sub = dp->create_subscriber( DDS::SUBSCRIBER_QOS_DEFAULT, NULL, 0 );
          DDS::DataReaderQos dr_qos;
          sub->get_default_datareader_qos( dr_qos );
          rval = dr_qos.type_consistency;  
          dp->delete_contained_entities( );
          dpf->delete_participant( dp );
        }
      dpf->get_qos( dpf_qos );
      dpf_qos.entity_factory.autoenable_created_entities = 1;
      dpf->set_qos( dpf_qos );
    }
  return rval;
}

void disable_type_information(DDS::DomainParticipantQos &dp_qos) {}

DDS::DynamicType  *
CREATE_TYPE( DDS::DomainParticipant * dp,
             const char * types_uri,
             const char * type_name )
{
  DDS::DynamicType               * dt     = NULL;
  if ( dp && types_uri && type_name )
    {
      DDS::DynamicTypeBuilderFactory * dtbf   =
        DDS::DynamicTypeBuilderFactoryXml::get_instance( );
      if ( dtbf )
        {
          DDS::DynamicTypeBuilder        * dtb  =
            dtbf->create_type_w_uri ( types_uri,
                                      type_name,
                                      NULL );
          if ( dtb )
            {
              dt = dtb->build( );
              dtbf->delete_type_builder( dtb );
            }
        }
    }
  return dt;
}

DDS::ReturnCode_t
REGISTER_TYPE( DDS::DomainParticipant * dp,
               DDS::DynamicType       * dt,
               const char             * type_name )
{
  DDS::ReturnCode_t              retval = DDS::RETCODE_ERROR;
  if ( dp && dt && type_name )
    {
      DDS::DynamicTypeSupport * dts =
        DDS::DynamicTypeSupport::create_type_support ( dt );
      if ( dts )
        {
          retval = dts->register_type( dp, type_name );
        }
    }
  return retval;
}

void
CLEANUP_TYPE( DDS::DomainParticipant * dp,
              DDS::DynamicType       * dt )
{
  if ( dp && dt )
    {
      DDS::DynamicTypeBuilderFactory * dtbf   =
        DDS::DynamicTypeBuilderFactoryXml::get_instance( );
      dtbf->delete_type( dt );
    }
}

DDS::DynamicData *
CREATE_DATA( DDS::DynamicType       * dt )
{
  DDS::DynamicData * retval = NULL;
  DDS::DynamicDataFactory * ddf = DDS::DynamicDataFactory::get_instance();
  if ( ddf )
    {
      retval = ddf->create_data( dt );
    }
  return retval;
}

DDS::ReturnCode_t
INIT_DATA( DDS::DynamicData    * dd,
           const char          * xml_data_uri,
           const char          * json_data_uri )
{
  DDS::ReturnCode_t              retval = DDS::RETCODE_ERROR;
  if ( dd )
    {
      fflush( stderr );
      if ( xml_data_uri )
        {
          retval = coredx::DynamicData_init_from_xmluri( dd, xml_data_uri );
        }
      else
        {
          /* no specific data, just init to 'defaults' */
          fprintf( stderr, "[ No data to load. Using empty sample... ]\n" );
          fflush( stderr );
          retval = DDS::RETCODE_OK;
        }
    }
  return retval;
}
   
void
PRINT_DATA( DDS::DynamicData  * dd )
{
  // coredx::DynamicData_print( stderr, dd, 0 );
  coredx::DynamicData_print_xml( stdout, dd, 0 );
}

void CLEANUP_DATA(DDS::DynamicData *dd)
{
  DDS::DynamicDataFactory * ddf = DDS::DynamicDataFactory::get_instance();
  ddf->delete_data( dd );
}


bool
CHECK_DATA(DDS::DynamicData *dd,
           const char *xml_data_uri,
           const char *json_data_uri)
{
  bool retval = false;

  if (dd == NULL && json_data_uri == NULL) {
    return retval;
  }

  DDS::DynamicData *data_check =
    CREATE_DATA( (DDS::DynamicType *) dd->get_type() );
  
  if (data_check == NULL) {
    retval = false;
    goto done;
  }
  if (INIT_DATA(data_check, xml_data_uri, json_data_uri) != DDS_RETCODE_OK) {
    retval = false;
    goto done;
  }

  retval = dd->equals(data_check);
  if ( !retval )
    {
      printf("Expected:\n");
      PRINT_DATA( data_check );
    }
  
 done:
  if (data_check != NULL) {
    CLEANUP_DATA(data_check);
  }
  return retval;
}
