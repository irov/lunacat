#ifndef HB_GRID_PROCESS_NEWUSER_H_
#define HB_GRID_PROCESS_NEWUSER_H_

#include "hb_config/hb_config.h"

#include "hb_grid.h"

typedef struct hb_grid_process_newuser_in_data_t
{
    hb_uid_t project_uid;

    char user_login[128];
    char user_password[128];

} hb_grid_process_newuser_in_data_t;

typedef struct hb_grid_process_newuser_out_data_t
{
    hb_uid_t user_uid;
    hb_error_code_t code;
} hb_grid_process_newuser_out_data_t;

hb_result_t hb_grid_process_newuser( hb_grid_process_handle_t * _process, const hb_grid_process_newuser_in_data_t * _in, hb_grid_process_newuser_out_data_t * const _out );

#endif
