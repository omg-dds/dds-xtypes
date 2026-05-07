#include "dds/dds.h"
#include "dds/../../share/CycloneDDS/examples/dynsub/dyntypelib.h"
#include "dds/../../share/CycloneDDS/examples/dynsub/domtree.h"
#include "dds/../../share/CycloneDDS/examples/dynsub/dynsub.h"
#include "dds/../../share/CycloneDDS/examples/dynsub/compare_samples.h"
#include "dds/../../share/CycloneDDS/examples/dynsub/print_type.h"
#include "dds/../../share/CycloneDDS/examples/dynsub/type_cache.h"
#include "dds/../../share/CycloneDDS/examples/dynsub/size_and_align.h"

#define DDS_BOOLEAN_TRUE         (1)
#define DDS_BOOLEAN_FALSE        (0)
#define LISTENER_STATUS_MASK_ALL (ALL_STATUS)

const char* get_qos_policy_name(uint32_t last_policy_id) {

    switch (last_policy_id) {
        case DDS_INVALID_QOS_POLICY_ID: return "INVALID"; 
        case DDS_USERDATA_QOS_POLICY_ID: return "USERDATA"; 
        case DDS_DURABILITY_QOS_POLICY_ID: return "DURABILITY"; 
        case DDS_PRESENTATION_QOS_POLICY_ID: return "PRESENTATION"; 
        case DDS_DEADLINE_QOS_POLICY_ID: return "DEADLINE"; 
        case DDS_LATENCYBUDGET_QOS_POLICY_ID: return "LATENCYBUDGET"; 
        case DDS_OWNERSHIP_QOS_POLICY_ID: return "OWNERSHIP"; 
        case DDS_OWNERSHIPSTRENGTH_QOS_POLICY_ID: return "OWNERSHIPSTRENGTH"; 
        case DDS_LIVELINESS_QOS_POLICY_ID: return "LIVELINESS"; 
        case DDS_TIMEBASEDFILTER_QOS_POLICY_ID: return "TIMEBASEDFILTER"; 
        case DDS_PARTITION_QOS_POLICY_ID: return "PARTITION"; 
        case DDS_RELIABILITY_QOS_POLICY_ID: return "RELIABILITY"; 
        case DDS_DESTINATIONORDER_QOS_POLICY_ID: return "DESTINATIONORDER"; 
        case DDS_HISTORY_QOS_POLICY_ID: return "HISTORY"; 
        case DDS_RESOURCELIMITS_QOS_POLICY_ID: return "RESOURCELIMITS"; 
        case DDS_ENTITYFACTORY_QOS_POLICY_ID: return "ENTITYFACTORY"; 
        case DDS_WRITERDATALIFECYCLE_QOS_POLICY_ID: return "WRITERDATALIFECYCLE"; 
        case DDS_READERDATALIFECYCLE_QOS_POLICY_ID: return "READERDATALIFECYCLE"; 
        case DDS_TOPICDATA_QOS_POLICY_ID: return "TOPICDATA"; 
        case DDS_GROUPDATA_QOS_POLICY_ID: return "GROUPDATA"; 
        case DDS_TRANSPORTPRIORITY_QOS_POLICY_ID: return "TRANSPORTPRIORITY"; 
        case DDS_LIFESPAN_QOS_POLICY_ID: return "LIFESPAN"; 
        case DDS_DURABILITYSERVICE_QOS_POLICY_ID: return "DURABILITYSERVICE"; 
        case DDS_PROPERTY_QOS_POLICY_ID: return "PROPERTY"; 
        case DDS_TYPE_CONSISTENCY_ENFORCEMENT_QOS_POLICY_ID: return "TYPE_CONSISTENCY_ENFORCEMENT"; 
        case DDS_DATA_REPRESENTATION_QOS_POLICY_ID: return "DATAREPRESENTATION";
        default:
            return 0; 
    }
}


dds_return_t
init_data( void               ** dd,
           struct dyntypelib   * dtl,
           struct dyntype      * dt,
           const char          * data_folder,
           const char          * data_file )
{
  dds_return_t retval = DDS_RETCODE_ERROR;
  if (data_folder == NULL || data_file == NULL)
  {
    return retval;
  }
  char* xml_data_uri = malloc(strlen(data_folder) + strlen("/xml/") + strlen(data_file) + strlen(".xml") + 1);
  sprintf(xml_data_uri, "%s%s%s%s", data_folder, "/xml/", data_file, ".xml");
  if ( dd )
    {
      fflush( stderr );
      if ( xml_data_uri )
        {
          struct elem* input = domtree_from_file( xml_data_uri );
          struct dyntypelib_error err;
          *dd = dtl_scan_sample(dtl, input, &dt->typeobj->_u.complete, true, &err);
          if (*dd){
            retval = DDS_RETCODE_OK;
          }
        }
      else
        {
          /* no specific data, just init to 'defaults' */
          fprintf( stderr, "[ No data to load. Using empty sample... ]\n" );
          fflush( stderr );
          retval = DDS_RETCODE_ERROR;
        }
    }
  return retval;
}

bool check_data(void        *dynamic_sample,
                struct dyntypelib *dtl,
                struct dyntype    *dt,
                const char  *data_folder,
                const char  *data_file)
{
  bool retval = false;

  if (dynamic_sample == NULL && data_file == NULL) {
    return retval;
  }

  void* data_check = NULL;

  if (init_data(&data_check, dtl, dt, data_folder, data_file) != DDS_RETCODE_OK) {
    retval = false;
    goto done;
  }

  retval = compare_samples(dtl->typecache, true, dynamic_sample, data_check, &dt->typeobj->_u.complete);
  if ( retval <= 0 )
    {
      printf("%d: ", retval);
      printf("Expected:\n");
      dtl_print_sample(dtl, true, data_check, &dt->typeobj->_u.complete );
    }
  
 done:
  if (data_check != NULL) {
    ddsrt_free(data_check);
  }
  return retval;
}

void print_typeid(struct dyntype *dt, int version) {
    const DDS_XTypes_EquivalenceHash *id = &((DDS_XTypes_TypeInformation *)dt->typeinfo)->minimal.typeid_with_size.type_id._u.equivalence_hash;
    printf("Complete Type Object V%d - Type ID: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", version,
             (unsigned) (*id)[0], (unsigned) (*id)[1], (unsigned) (*id)[2], (unsigned) (*id)[3],
             (unsigned) (*id)[4], (unsigned) (*id)[5], (unsigned) (*id)[6], (unsigned) (*id)[7],
             (unsigned) (*id)[8], (unsigned) (*id)[9], (unsigned) (*id)[10], (unsigned) (*id)[11],
             (unsigned) (*id)[12], (unsigned) (*id)[13]);
    id = &((DDS_XTypes_TypeInformation *)dt->typeinfo)->minimal.typeid_with_size.type_id._u.equivalence_hash;
    printf("Minimal Type Object V%d - Type ID: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", version,
             (unsigned) (*id)[0], (unsigned) (*id)[1], (unsigned) (*id)[2], (unsigned) (*id)[3],
             (unsigned) (*id)[4], (unsigned) (*id)[5], (unsigned) (*id)[6], (unsigned) (*id)[7],
             (unsigned) (*id)[8], (unsigned) (*id)[9], (unsigned) (*id)[10], (unsigned) (*id)[11],
             (unsigned) (*id)[12], (unsigned) (*id)[13]);
}
