#include "hb_grid_process_newproject.h"

#include "hb_log/hb_log.h"
#include "hb_db/hb_db.h"
#include "hb_cache/hb_cache.h"
#include "hb_storage/hb_storage.h"
#include "hb_json/hb_json.h"
#include "hb_utils/hb_getopt.h"
#include "hb_utils/hb_httpopt.h"
#include "hb_utils/hb_rand.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

hb_result_t hb_grid_process_newproject( hb_grid_process_handle_t * _process, const hb_grid_process_newproject_in_data_t * _in, hb_grid_process_newproject_out_data_t * const _out )
{
    hb_db_collection_handle_t * db_collection_projects;
    if( hb_db_get_collection( _process->db_client, "hb", "projects", &db_collection_projects ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    hb_db_values_handle_t * project_values;
    if( hb_db_create_values( &project_values ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    hb_db_make_uid_value( project_values, "aoid", HB_UNKNOWN_STRING_SIZE, _in->account_uid );
    hb_db_make_string_value( project_values, "public_data", HB_UNKNOWN_STRING_SIZE, "{}", HB_UNKNOWN_STRING_SIZE );
    hb_db_make_int32_value( project_values, "public_data_revision", HB_UNKNOWN_STRING_SIZE, 0 );

    hb_uid_t puid;
    if( hb_db_new_document( db_collection_projects, project_values, &puid ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    hb_db_destroy_values( project_values );

    hb_db_destroy_collection( db_collection_projects );

    _out->code = HB_ERROR_OK;
    _out->project_uid = puid;

    return HB_SUCCESSFUL;
}