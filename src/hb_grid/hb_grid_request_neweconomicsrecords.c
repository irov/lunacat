#include "hb_grid.h"

#include "hb_grid_process_neweconomicsrecords.h"

#include "hb_token/hb_token.h"
#include "hb_http/hb_http.h"
#include "hb_json/hb_json.h"
#include "hb_utils/hb_base64.h"
#include "hb_utils/hb_base16.h"

#include <string.h>

hb_http_code_t hb_grid_request_neweconomicsrecords( hb_grid_request_handle_t * _args )
{
    HB_UNUSED( _args );

    //const char * arg_account_token = _args->arg1;
    //const char * arg_puid = _args->arg2;

    //hb_account_token_t account_token;
    //if( hb_cache_get_token( _process->cache, arg_account_token, 1800, &account_token, sizeof( account_token ), HB_NULLPTR ) == HB_FAILURE )
    //{
    //    return HTTP_BADREQUEST;
    //}

    //hb_grid_process_neweconomicsrecords_in_data_t in_data;

    //in_data.account_uid = account_token.account_uid;

    //if( hb_base16_decode( arg_puid, HB_UNKNOWN_STRING_SIZE, &in_data.project_uid, sizeof( in_data.project_uid ), HB_NULLPTR ) == HB_FAILURE )
    //{
    //    return HTTP_BADREQUEST;
    //}

    //uint32_t multipart_params_count;
    //multipart_params_handle_t multipart_params[8];
    //if( hb_http_get_request_params( _request, multipart_params, 8, &multipart_params_count ) == HB_FAILURE )
    //{
    //    return HTTP_BADREQUEST;
    //}

    //hb_grid_process_lock( _process, account_token.account_uid );

    //hb_size_t params_data_size;
    //const void * params_data;
    //hb_result_t result = hb_multipart_get_value( multipart_params, multipart_params_count, "data", &params_data, &params_data_size );

    //hb_grid_process_unlock( _process, account_token.account_uid );

    //if( result == HB_FAILURE )
    //{
    //    return HTTP_BADREQUEST;
    //}

    //if( params_data_size > HB_DATA_MAX_SIZE )
    //{
    //    return HTTP_BADREQUEST;
    //}

    //memcpy( in_data.records_source, params_data, params_data_size );
    //in_data.records_source_size = params_data_size;

    //hb_grid_process_neweconomicsrecords_out_data_t out_data;
    //if( hb_grid_process_neweconomicsrecords( _process, &in_data, &out_data ) == HB_FAILURE )
    //{
    //    return HTTP_BADREQUEST;
    //}

    //hb_size_t response_data_size = sprintf( _response, "{\"code\":0}" );

    //*_size = response_data_size;
    
    return HTTP_OK;
}