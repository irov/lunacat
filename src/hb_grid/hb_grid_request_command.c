#include "hb_grid.h"
#include "hb_grid_process_api.h"

#include "hb_http/hb_http.h"
#include "hb_token/hb_token.h"
#include "hb_utils/hb_base64.h"
#include "hb_utils/hb_base16.h"

#include <string.h>

hb_http_code_t hb_grid_request_command( hb_grid_request_handle_t * _args )
{
    const char * arg_account_token;
    if( hb_grid_get_arg_string( _args, "account_token", &arg_account_token ) == HB_FAILURE )
    {
        return HTTP_BADREQUEST;
    }

    const char * arg_project_uid;
    if( hb_grid_get_arg_string( _args, "project_uid", &arg_project_uid ) == HB_FAILURE )
    {
        return HTTP_BADREQUEST;
    }

    const char * arg_method;
    if( hb_grid_get_arg_string( _args, "method", &arg_method ) == HB_FAILURE )
    {
        return HTTP_BADREQUEST;
    }

    hb_json_handle_t * json_method_args;
    if( hb_grid_get_arg_json( _args, "args", &json_method_args ) == HB_FAILURE )
    {
        return HTTP_BADREQUEST;
    }

    hb_account_token_t account_token;
    if( hb_grid_get_account_token( _args, arg_account_token, &account_token ) == HB_FAILURE )
    {
        return HTTP_BADREQUEST;
    }

    hb_uid_t project_uid;
    if( hb_grid_get_uid( arg_project_uid, &project_uid ) == HB_FAILURE )
    {
        return HTTP_BADREQUEST;
    }

    hb_grid_process_api_in_data_t in_data;
    in_data.project_uid = project_uid;
    in_data.user_uid = HB_UID_NONE;

    strncpy( in_data.api, "command", 32 );
    strncpy( in_data.method, arg_method, 32 );

    in_data.json_args = json_method_args;

    hb_grid_process_lock( _args->process, account_token.account_uid );

    hb_grid_process_api_out_data_t out_data;
    hb_result_t result = hb_grid_process_api( _args->process, &in_data, &out_data );

    hb_grid_process_unlock( _args->process, account_token.account_uid );

    if( result == HB_FAILURE )
    {
        return HTTP_BADREQUEST;
    }

    if( out_data.code != HB_ERROR_OK )
    {
        snprintf( _args->response, HB_GRID_RESPONSE_DATA_MAX_SIZE, "{\"code\":%u}"
            , out_data.code
        );

        return HTTP_OK;
    }

    snprintf( _args->response, HB_GRID_RESPONSE_DATA_MAX_SIZE, "{\"code\":0,\"data\":%.*s,\"stat\":{\"memory_used\":%zu,\"call_used\":%u}}"
        , (int32_t)out_data.response_size
        , out_data.response_data
        , out_data.memory_used
        , out_data.call_used
    );

    return HTTP_OK;
}