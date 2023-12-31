#include "hb_script_handle.h"
#include "hb_script_json.h"

#include "hb_json/hb_json.h"

int hb_script_server_SetUserEntityPublicData( lua_State * L )
{
    hb_script_handle_t * script_handle = *(hb_script_handle_t **)lua_getextraspace( L );

    lua_Integer uid = lua_tointeger( L, 1 );

    hb_db_values_handle_t * values;

    if( hb_db_create_values( &values ) == HB_FAILURE )
    {
        HB_SCRIPT_ERROR( L, "internal error" );
    }

    hb_db_make_uid_value( values, "_id", HB_UNKNOWN_STRING_SIZE, (hb_uid_t)uid );
    hb_db_make_uid_value( values, "uoid", HB_UNKNOWN_STRING_SIZE, script_handle->user_uid );

    hb_bool_t exist;
    hb_uid_t eoid;
    if( hb_db_find_uid( script_handle->db_collection_user_entities, values, &eoid, &exist ) == HB_FAILURE )
    {
        HB_SCRIPT_ERROR( L, "internal error" );
    }

    hb_db_destroy_values( values );

    if( exist == HB_FALSE )
    {
        HB_SCRIPT_ERROR( L, "internal error" );
    }

    if( hb_script_json_set_public_data( L, 2, script_handle->db_collection_user_entities, eoid ) == HB_FAILURE )
    {
        HB_SCRIPT_ERROR( L, "internal error" );
    }

    return 0;
}