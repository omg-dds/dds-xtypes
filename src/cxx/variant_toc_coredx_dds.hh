
#include <dds/dds.hh>
#include <dds/dds_typesupport.hh>
#include <dds/xtypes.hh>

#define LISTENER_STATUS_MASK_ALL (ALL_STATUS)

void StringSeq_push(DDS::StringSeq  &string_seq, const char *elem)
{
  string_seq.push_back((char*)elem);
}

const char *get_qos_policy_name(DDS_QosPolicyId_t policy_id)
{
  return DDS_qos_policy_str(policy_id);
}

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
           const char          * data_uri )
{
  DDS::ReturnCode_t              retval = DDS::RETCODE_ERROR;
  if ( dd && data_uri )
    {
      retval = coredx::DynamicData_init_from_xmluri( dd, data_uri );
    }
  return retval;
}
   
void
PRINT_DATA( DDS::DynamicData  * dd )
{
  coredx::DynamicData_print_xml( stdout, dd, 0 );
}

