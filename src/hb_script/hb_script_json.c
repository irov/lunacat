#include "hb_script_json.h"
#include "hb_log/hb_log.h"

#include <stdio.h>
#include <memory.h>
#include <string.h>

#ifndef HB_SCRIPT_USER_PUBLIC_DATA_FIELD_MAX
#define HB_SCRIPT_USER_PUBLIC_DATA_FIELD_MAX 16
#endif

//////////////////////////////////////////////////////////////////////////
static hb_result_t __hb_script_json_value_dumps( lua_State * L, int _index, char * _buffer, hb_size_t * _offset, hb_size_t _capacity )
{
    HB_UNUSED( _capacity );

    int type = lua_type( L, _index );

    switch( type )
    {
    case LUA_TNIL:
        {
            *_offset += sprintf( _buffer + *_offset, "null" );
        }break;
    case LUA_TBOOLEAN:
        {
            int result = lua_toboolean( L, _index );

            *_offset += sprintf( _buffer + *_offset, result == 1 ? "true" : "false" );
        }break;
    case LUA_TNUMBER:
        {
            if( lua_isinteger( L, _index ) == 0 )
            {
                lua_Number result = lua_tonumber( L, _index );

                *_offset += sprintf( _buffer + *_offset, LUA_NUMBER_FMT, result );
            }
            else
            {
                lua_Integer result = lua_tointeger( L, _index );

                *_offset += sprintf( _buffer + *_offset, LUA_INTEGER_FMT, result );
            }
        }break;
    case LUA_TSTRING:
        {
            const char * result = lua_tostring( L, _index );

            *_offset += sprintf( _buffer + *_offset, "\"%s\"", result );
        }break;
    case LUA_TTABLE:
        {
            lua_Unsigned len = lua_rawlen( L, _index );

            if( len == 0 )
            {
                *_offset += sprintf( _buffer + *_offset, "{" );

                lua_pushvalue( L, _index );
                lua_pushnil( L );
                int it = lua_next( L, -2 );
                while( it != 0 )
                {
                    const char * key = lua_tostring( L, -2 );
                    *_offset += sprintf( _buffer + *_offset, "\"%s\": ", key );

                    if( __hb_script_json_value_dumps( L, -1, _buffer, _offset, _capacity ) == HB_FAILURE )
                    {
                        return HB_FAILURE;
                    }

                    lua_pop( L, 1 );

                    it = lua_next( L, -2 );

                    if( it == 0 )
                    {
                        break;
                    }

                    *_offset += sprintf( _buffer + *_offset, "," );
                }

                lua_pop( L, 1 );

                *_offset += sprintf( _buffer + *_offset, "}" );
            }
            else
            {
                *_offset += sprintf( _buffer + *_offset, "[" );

                lua_pushvalue( L, _index );
                lua_pushnil( L );
                int it = lua_next( L, -2 );
                while( it != 0 )
                {
                    if( __hb_script_json_value_dumps( L, -1, _buffer, _offset, _capacity ) == HB_FAILURE )
                    {
                        return HB_FAILURE;
                    }

                    lua_pop( L, 1 );

                    it = lua_next( L, -2 );

                    if( it == 0 )
                    {
                        break;
                    }

                    *_offset += sprintf( _buffer + *_offset, "," );
                }

                lua_pop( L, 1 );

                *_offset += sprintf( _buffer + *_offset, "]" );
            }
        }break;
    default:
        return HB_FAILURE;
    }

    return HB_SUCCESSFUL;
}
//////////////////////////////////////////////////////////////////////////
hb_result_t hb_script_json_dumps( lua_State * L, int32_t _index, char * _buffer, hb_size_t _capacity, hb_size_t * _size )
{
    hb_size_t offset = 0;

    if( __hb_script_json_value_dumps( L, _index, _buffer, &offset, _capacity ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    if( _size != HB_NULLPTR )
    {
        *_size = offset;
    }

    return HB_SUCCESSFUL;
}
//////////////////////////////////////////////////////////////////////////
static hb_result_t __hb_json_fields( hb_size_t _index, const hb_json_handle_t * _key, const hb_json_handle_t * _value, void * _ud );
//////////////////////////////////////////////////////////////////////////
static hb_result_t __hb_json_arrays( hb_size_t _index, const hb_json_handle_t * _value, void * _ud )
{
    lua_State * L = (lua_State *)_ud;

    hb_json_type_t json_value_type = hb_json_get_type( _value );

    switch( json_value_type )
    {
    case e_hb_json_object:
        {
            if( hb_json_is_object_empty( _value ) == HB_TRUE )
            {
                lua_createtable( L, 0, 0 );
            }
            else
            {
                uint32_t json_count = hb_json_get_fields_count( _value );

                lua_createtable( L, 0, json_count );

                if( hb_json_object_foreach( _value, &__hb_json_fields, (void *)L ) == HB_FAILURE )
                {
                    return HB_FAILURE;
                }
            }
        }break;
    case e_hb_json_array:
        {
            if( hb_json_is_array_empty( _value ) == HB_TRUE )
            {
                lua_createtable( L, 0, 0 );
            }
            else
            {
                uint32_t json_count = hb_json_array_count( _value );

                lua_createtable( L, json_count, 0 );

                if( hb_json_array_foreach( _value, &__hb_json_arrays, (void *)L ) == HB_FAILURE )
                {
                    return HB_FAILURE;
                }
            }
        }break;
    case e_hb_json_string:
        {
            hb_size_t length;
            const char * value;
            if( hb_json_to_string( _value, &value, &length ) == HB_FAILURE )
            {
                return HB_FAILURE;
            }

            lua_pushlstring( L, value, length );
        }break;
    case e_hb_json_integer:
        {
            int64_t value;
            if( hb_json_to_int64( _value, &value ) == HB_FAILURE )
            {
                return HB_FAILURE;
            }

            lua_pushinteger( L, (lua_Integer)value );
        }break;
    case e_hb_json_real:
        {
            double value;
            if( hb_json_to_double( _value, &value ) == HB_FAILURE )
            {
                return HB_FAILURE;
            }

            lua_pushnumber( L, (lua_Number)value );
        }break;
    case e_hb_json_true:
        {
            lua_pushboolean( L, 1 );
        }break;
    case e_hb_json_false:
        {
            lua_pushboolean( L, 0 );
        }break;
    case e_hb_json_null:
        {
            lua_pushnil( L );
        }break;
    default:
        return HB_FAILURE;
        break;
    }

    lua_rawseti( L, -2, _index + 1 );

    return HB_SUCCESSFUL;
}
//////////////////////////////////////////////////////////////////////////
static hb_result_t __hb_json_fields( hb_size_t _index, const hb_json_handle_t * _key, const hb_json_handle_t * _value, void * _ud )
{
    HB_UNUSED( _index );

    lua_State * L = (lua_State *)_ud;

    hb_json_type_t json_value_type = hb_json_get_type( _value );

    switch( json_value_type )
    {
    case e_hb_json_object:
        {
            if( hb_json_is_object_empty( _value ) == HB_TRUE )
            {
                lua_createtable( L, 0, 0 );
            }
            else
            {
                uint32_t json_count = hb_json_get_fields_count( _value );

                lua_createtable( L, 0, json_count );

                if( hb_json_object_foreach( _value, &__hb_json_fields, (void *)L ) == HB_FAILURE )
                {
                    return HB_FAILURE;
                }
            }
        }break;
    case e_hb_json_array:
        {
            if( hb_json_is_array_empty( _value ) == HB_TRUE )
            {
                lua_createtable( L, 0, 0 );
            }
            else
            {
                uint32_t json_count = hb_json_array_count( _value );

                lua_createtable( L, json_count, 0 );

                if( hb_json_array_foreach( _value, &__hb_json_arrays, (void *)L ) == HB_FAILURE )
                {
                    return HB_FAILURE;
                }
            }
        }break;
    case e_hb_json_string:
        {
            hb_size_t length;
            const char * value;
            if( hb_json_to_string( _value, &value, &length ) == HB_FAILURE )
            {
                return HB_FAILURE;
            }

            lua_pushlstring( L, value, length );
        }break;
    case e_hb_json_integer:
        {
            int64_t value;
            if( hb_json_to_int64( _value, &value ) == HB_FAILURE )
            {
                return HB_FAILURE;
            }

            lua_pushinteger( L, (lua_Integer)value );
        }break;
    case e_hb_json_real:
        {
            double value;
            if( hb_json_to_double( _value, &value ) == HB_FAILURE )
            {
                return HB_FAILURE;
            }

            lua_pushnumber( L, (lua_Number)value );
        }break;
    case e_hb_json_true:
        {
            lua_pushboolean( L, 1 );
        }break;
    case e_hb_json_false:
        {
            lua_pushboolean( L, 0 );
        }break;
    case e_hb_json_null:
        {
            lua_pushnil( L );
        }break;
    default:
        return HB_FAILURE;
        break;
    }

    const char * key_str;
    hb_size_t key_length;

    if( hb_json_to_string( _key, &key_str, &key_length ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    lua_setfield( L, -2, key_str );

    return HB_SUCCESSFUL;
}
//////////////////////////////////////////////////////////////////////////
hb_result_t hb_script_json_loads( lua_State * L, const hb_json_handle_t * _json )
{
    if( _json == HB_NULLPTR )
    {
        lua_createtable( L, 0, 0 );

        return HB_SUCCESSFUL;
    }

    if( hb_json_is_object_empty( _json ) == HB_TRUE )
    {
        lua_createtable( L, 0, 0 );

        return HB_SUCCESSFUL;
    }

    uint32_t json_count = hb_json_get_fields_count( _json );

    lua_createtable( L, 0, json_count );

    hb_result_t result = hb_json_object_foreach( _json, &__hb_json_fields, (void *)L );

    return result;
}
//////////////////////////////////////////////////////////////////////////
hb_result_t hb_script_json_loads_data( lua_State * L, const void * _data, hb_size_t _size )
{
    if( _size == 0 )
    {
        lua_createtable( L, 0, 0 );

        return HB_SUCCESSFUL;
    }
    else if( memcmp( _data, "{}", 2 ) == 0 )
    {
        lua_createtable( L, 0, 0 );

        return HB_SUCCESSFUL;
    }

    uint8_t pool[HB_DATA_MAX_SIZE];

    hb_json_handle_t * json_data;
    if( hb_json_create( _data, _size, pool, HB_DATA_MAX_SIZE, &json_data ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    hb_result_t result = hb_script_json_loads( L, json_data );

    return result;
}
//////////////////////////////////////////////////////////////////////////
hb_result_t hb_script_json_load_fields( lua_State * L, const hb_json_handle_t * _json, const char ** _fields, uint32_t _fieldcount )
{
    if( _fieldcount == 0 )
    {
        return HB_SUCCESSFUL;
    }

    for( uint32_t index = 0; index != _fieldcount; ++index )
    {
        const char * field = _fields[index];

        hb_json_handle_t * json_field;
        if( hb_json_get_field( _json, field, &json_field ) == HB_FAILURE )
        {
            return HB_FAILURE;
        }

        if( json_field == HB_NULLPTR )
        {
            lua_pushnil( L );

            continue;
        }

        hb_json_type_t json_field_type = hb_json_get_type( json_field );

        switch( json_field_type )
        {
        case e_hb_json_object:
            {
                if( hb_json_is_object_empty( json_field ) == HB_TRUE )
                {
                    lua_createtable( L, 0, 0 );
                }
                else
                {
                    uint32_t json_count = hb_json_get_fields_count( json_field );

                    lua_createtable( L, 0, json_count );

                    if( hb_json_object_foreach( json_field, &__hb_json_fields, (void *)L ) == HB_FAILURE )
                    {
                        return HB_FAILURE;
                    }
                }
            }break;
        case e_hb_json_array:
            {
                if( hb_json_is_array_empty( json_field ) == HB_TRUE )
                {
                    lua_createtable( L, 0, 0 );
                }
                else
                {
                    uint32_t json_count = hb_json_array_count( json_field );

                    lua_createtable( L, json_count, 0 );

                    if( hb_json_array_foreach( json_field, &__hb_json_arrays, (void *)L ) == HB_FAILURE )
                    {
                        return HB_FAILURE;
                    }
                }
            }break;
        case e_hb_json_string:
            {
                hb_size_t length;
                const char * value;
                if( hb_json_to_string( json_field, &value, &length ) == HB_FAILURE )
                {
                    return HB_FAILURE;
                }

                lua_pushlstring( L, value, length );
            }break;
        case e_hb_json_integer:
            {
                int64_t value;
                if( hb_json_to_int64( json_field, &value ) == HB_FAILURE )
                {
                    return HB_FAILURE;
                }

                lua_pushinteger( L, (lua_Integer)value );
            }break;
        case e_hb_json_real:
            {
                double value;
                if( hb_json_to_double( json_field, &value ) == HB_FAILURE )
                {
                    return HB_FAILURE;
                }

                lua_pushnumber( L, (lua_Number)value );
            }break;
        case e_hb_json_true:
            {
                lua_pushboolean( L, 1 );
            }break;
        case e_hb_json_false:
            {
                lua_pushboolean( L, 0 );
            }break;
        case e_hb_json_null:
            {
                lua_pushnil( L );
            }break;
        default:
            break;
        }
    }

    return HB_SUCCESSFUL;
}
//////////////////////////////////////////////////////////////////////////
hb_result_t hb_script_json_load_fields_data( lua_State * L, const void * _data, hb_size_t _size, const char ** _fields, uint32_t _fieldcount )
{
    if( _size == 0 )
    {
        lua_createtable( L, 0, 0 );

        return HB_SUCCESSFUL;
    }
    else if( memcmp( _data, "{}", 2 ) == 0 )
    {
        lua_createtable( L, 0, 0 );

        return HB_SUCCESSFUL;
    }

    uint8_t pool[HB_DATA_MAX_SIZE];

    hb_json_handle_t * json_data;
    if( hb_json_create( _data, _size, pool, HB_DATA_MAX_SIZE, &json_data ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    hb_result_t result = hb_script_json_load_fields( L, json_data, _fields, _fieldcount );

    return result;
}
//////////////////////////////////////////////////////////////////////////
hb_result_t hb_script_json_create( lua_State * L, int32_t _index, hb_json_handle_t ** _json )
{
    char json_data[HB_DATA_MAX_SIZE];
    hb_size_t json_size;
    if( hb_script_json_dumps( L, _index, json_data, HB_DATA_MAX_SIZE, &json_size ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    uint8_t pool[HB_DATA_MAX_SIZE];
    if( hb_json_create( json_data, json_size, pool, HB_DATA_MAX_SIZE, _json ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    return HB_SUCCESSFUL;
}
//////////////////////////////////////////////////////////////////////////
hb_result_t hb_script_json_get_public_data( lua_State * L, int32_t _index, const hb_db_collection_handle_t * _collection, hb_uid_t _uid, uint32_t * _count )
{
    const char * fields[HB_SCRIPT_USER_PUBLIC_DATA_FIELD_MAX];
    uint32_t fields_count = 0;

    if( hb_script_get_fields( L, _index, fields, &fields_count ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    const char * db_fields[] = {"public_data"};

    hb_db_values_handle_t * project_values;
    if( hb_db_get_values( _collection, _uid, db_fields, 1, &project_values, HB_NULLPTR ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    const char * public_data_symbol;
    hb_size_t public_data_symbol_length;

    if( hb_db_get_string_value( project_values, 0, &public_data_symbol, &public_data_symbol_length ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    if( hb_script_json_load_fields_data( L, public_data_symbol, public_data_symbol_length, fields, fields_count ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    hb_db_destroy_values( project_values );

    *_count = fields_count;

    return HB_SUCCESSFUL;
}
//////////////////////////////////////////////////////////////////////////
hb_result_t hb_script_json_set_public_data( lua_State * L, int32_t _index, const hb_db_collection_handle_t * _collection, hb_uid_t _uid )
{
    char json_data[HB_DATA_MAX_SIZE];
    hb_size_t json_data_size;
    if( hb_script_json_dumps( L, _index, json_data, HB_DATA_MAX_SIZE, &json_data_size ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    hb_db_values_handle_t * update_values;
    if( hb_db_create_values( &update_values ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    hb_db_make_string_value( update_values, "public_data", HB_UNKNOWN_STRING_SIZE, json_data, json_data_size );
    hb_db_inc_int32_value( update_values, "public_data_revision", HB_UNKNOWN_STRING_SIZE, 1 );

    if( hb_db_update_values( _collection, _uid, update_values ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    hb_db_destroy_values( update_values );

    return HB_SUCCESSFUL;
}
//////////////////////////////////////////////////////////////////////////
hb_result_t hb_script_json_update_public_data( lua_State * L, int32_t _index, const hb_db_collection_handle_t * _collection, hb_uid_t _uid )
{
    const char * db_fields[] = {"public_data"};

    hb_db_values_handle_t * user_values;
    if( hb_db_get_values( _collection, _uid, db_fields, 1, &user_values, HB_NULLPTR ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    const char * public_data_symbol;
    hb_size_t public_data_symbol_length;

    if( hb_db_get_string_value( user_values, 0, &public_data_symbol, &public_data_symbol_length ) == HB_FAILURE )
    {
        hb_db_destroy_values( user_values );

        return HB_FAILURE;
    }

    uint8_t pool[HB_DATA_MAX_SIZE];

    hb_json_handle_t * json_data;
    if( hb_json_create( public_data_symbol, public_data_symbol_length, pool, HB_DATA_MAX_SIZE, &json_data ) == HB_FAILURE )
    {
        hb_db_destroy_values( user_values );

        return HB_FAILURE;
    }

    hb_db_destroy_values( user_values );

    hb_json_handle_t * json_update;
    if( hb_script_json_create( L, _index, &json_update ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    hb_json_handle_t * json_result;

    uint8_t data[HB_DATA_MAX_SIZE];
    if( hb_json_update( json_data, json_update, data, HB_DATA_MAX_SIZE, &json_result ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    char json_result_data[HB_DATA_MAX_SIZE];
    hb_size_t json_result_data_size;
    if( hb_json_dumps( json_result, json_result_data, HB_DATA_MAX_SIZE, &json_result_data_size ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    hb_db_values_handle_t * update_values;

    if( hb_db_create_values( &update_values ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    hb_db_make_string_value( update_values, "public_data", HB_UNKNOWN_STRING_SIZE, json_result_data, json_result_data_size );
    hb_db_inc_int32_value( update_values, "public_data_revision", HB_UNKNOWN_STRING_SIZE, 1 );

    if( hb_db_update_values( _collection, _uid, update_values ) == HB_FAILURE )
    {
        return HB_FAILURE;
    }

    hb_db_destroy_values( update_values );

    return HB_SUCCESSFUL;
}
//////////////////////////////////////////////////////////////////////////
hb_result_t hb_script_get_fields( lua_State * L, int32_t _index, const char ** _fieds, uint32_t * _count )
{
    uint32_t field_iterator = 0;

    lua_pushvalue( L, _index );
    lua_pushnil( L );
    while( lua_next( L, -2 ) != 0 )
    {
        if( field_iterator == HB_SCRIPT_USER_PUBLIC_DATA_FIELD_MAX )
        {
            HB_LOG_MESSAGE_ERROR( "script", "invalid get fields overflow [%u]"
                , HB_SCRIPT_USER_PUBLIC_DATA_FIELD_MAX
            );

            return HB_FAILURE;
        }

        const char * value = lua_tostring( L, -1 );
        _fieds[field_iterator++] = value;

        lua_pop( L, 1 );
    }
    lua_pop( L, 1 );

    *_count = field_iterator;

    return HB_SUCCESSFUL;
}