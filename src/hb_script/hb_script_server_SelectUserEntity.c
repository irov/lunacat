#include "hb_script_handle.h"
#include "hb_script_json.h"

#include "hb_json/hb_json.h"
#include "hb_utils/hb_rand.h"
#include "hb_utils/hb_base16.h"

#include <string.h>

#ifndef HB_SCRIPT_SELECT_USER_ENTITY_MAX
#define HB_SCRIPT_SELECT_USER_ENTITY_MAX 512
#endif

int hb_script_server_SelectUserEntity( lua_State * L )
{
    hb_script_handle_t * script_handle = *(hb_script_handle_t **)lua_getextraspace( L );

    hb_size_t parent_len;
    const char * parent = lua_tolstring( L, 1, &parent_len );

    if( parent_len == 0 )
    {
        HB_SCRIPT_ERROR( L, "internal error" );
    }

    hb_db_values_handle_t * query;

    if( hb_db_create_values( &query ) == HB_FAILURE )
    {
        HB_SCRIPT_ERROR( L, "internal error" );
    }

    hb_db_make_string_value( query, "name", HB_UNKNOWN_STRING_SIZE, parent, parent_len );
    hb_db_make_uid_value( query, "uoid", HB_UNKNOWN_STRING_SIZE, script_handle->user_uid );

    const char * db_fields[1] = {"_id"};

    uint32_t exists = 0;
    hb_db_values_handle_t * values[HB_SCRIPT_SELECT_USER_ENTITY_MAX];
    if( hb_db_select_values( script_handle->db_collection_user_entities, query, db_fields, 1, values, HB_SCRIPT_SELECT_USER_ENTITY_MAX, &exists ) == HB_FAILURE )
    {
        HB_SCRIPT_ERROR( L, "internal error" );
    }

    hb_db_destroy_values( query );

    lua_createtable( L, exists, 0 );
    for( uint32_t index = 0; index != exists; ++index )
    {
        const hb_db_values_handle_t * value = values[index];

        int32_t eid;
        hb_db_get_int32_value( value, 0, &eid );

        lua_pushinteger( L, eid );
        lua_rawseti( L, -2, (int32_t)index + 1 );
    }

    for( uint32_t index = 0; index != exists; ++index )
    {
        hb_db_values_handle_t * value = values[index];

        hb_db_destroy_values( value );
    }

    return 1;
}