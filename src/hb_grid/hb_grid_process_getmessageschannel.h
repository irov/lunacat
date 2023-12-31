#ifndef HB_GRID_PROCESS_NEWMESSAGESCHANNEL_H_
#define HB_GRID_PROCESS_NEWMESSAGESCHANNEL_H_

#include "hb_config/hb_config.h"

#include "hb_grid.h"
//////////////////////////////////////////////////////////////////////////
typedef struct hb_grid_process_getmessageschannel_in_data_t
{
    hb_uid_t user_uid;
    hb_uid_t project_uid;

    hb_uid_t messageschannel_uid;
    uint32_t messageschannel_postid;
} hb_grid_process_getmessageschannel_in_data_t;
//////////////////////////////////////////////////////////////////////////
typedef struct hb_grid_process_getmessageschannel_out_data_t
{
    hb_messages_get_t posts[256];
    uint32_t posts_count;
    hb_error_code_t code;
} hb_grid_process_getmessageschannel_out_data_t;
//////////////////////////////////////////////////////////////////////////
hb_result_t hb_grid_process_getmessageschannel( hb_grid_process_handle_t * _process, const hb_grid_process_getmessageschannel_in_data_t * _in, hb_grid_process_getmessageschannel_out_data_t * const _out );
//////////////////////////////////////////////////////////////////////////
#endif
