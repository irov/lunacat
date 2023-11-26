PROJECT(hb_grid LANGUAGES C)

LUNACAT_ADD_FILTER(
src
    main.c

    hb_grid.h
    hb_grid.c
)

LUNACAT_ADD_FILTER(
request
    hb_grid_request_api.c
    hb_grid_request_avatar.c
    hb_grid_request_command.c
    hb_grid_request_upload.c
    hb_grid_request_setusernickname.c
    hb_grid_request_setleaderscore.c
    hb_grid_request_getleaderrank.c
    hb_grid_request_getleaderboard.c
    hb_grid_request_newmessageschannel.c
    hb_grid_request_postmessageschannel.c
    hb_grid_request_getmessageschannel.c
    hb_grid_request_neweconomicsrecords.c
    hb_grid_request_neweventstopic.c
    hb_grid_request_geteventstopic.c
    hb_grid_request_loginuser.c
    hb_grid_request_newuser.c
    hb_grid_request_newproject.c
    hb_grid_request_newaccount.c
    hb_grid_request_loginaccount.c
)

LUNACAT_ADD_FILTER(
process
    hb_grid_process_newaccount.h
    hb_grid_process_newaccount.c
    hb_grid_process_loginaccount.h
    hb_grid_process_loginaccount.c
    hb_grid_process_newuser.h
    hb_grid_process_newuser.c
    hb_grid_process_setusernickname.h
    hb_grid_process_setusernickname.c
    hb_grid_process_setleaderscore.h
    hb_grid_process_setleaderscore.c
    hb_grid_process_getleaderrank.h
    hb_grid_process_getleaderrank.c
    hb_grid_process_getleaderboard.h
    hb_grid_process_getleaderboard.c
    hb_grid_process_loginuser.h
    hb_grid_process_loginuser.c
    hb_grid_process_newproject.h
    hb_grid_process_newproject.c
    hb_grid_process_upload.h
    hb_grid_process_upload.c
    hb_grid_process_api.h
    hb_grid_process_api.c
    hb_grid_process_newmessageschannel.h
    hb_grid_process_newmessageschannel.c
    hb_grid_process_getmessageschannel.h
    hb_grid_process_getmessageschannel.c
    hb_grid_process_postmessageschannel.h
    hb_grid_process_postmessageschannel.c
    hb_grid_process_neweconomicsrecords.h
    hb_grid_process_neweconomicsrecords.c
)

INCLUDE_DIRECTORIES(${LUNACAT_DEPENDENCIES_CONFIG_OUTPUT_DIRECTORY}/libevent/include)
INCLUDE_DIRECTORIES(${LUNACAT_DEPENDENCIES_SOURCE_DIRECTORY}/libevent/include)

LUNACAT_ADD_EXECUTABLE()

LUNACAT_LINK_DEPENDENCIES_LIBRARY(event)
LUNACAT_LINK_DEPENDENCIES_LIBRARY(lz4)
LUNACAT_LINK_DEPENDENCIES_LIBRARY(yyjson)

if(MSVC)
    LUNACAT_LINK_DEPENDENCIES_LIBRARY(hiredis_static)
endif()

if(UNIX)
    LUNACAT_LINK_DEPENDENCIES_LIBRARY(hiredis)
endif()

LUNACAT_LINK_DEPENDENCIES_LIBRARY(mongoc-static-1.0)
LUNACAT_LINK_DEPENDENCIES_LIBRARY(bson-static-1.0)

if(LUNACAT_TARGET_WIN32)
    LUNACAT_LINK_DEPENDENCIES_LIBRARY(zlib)
elseif(LUNACAT_TARGET_UNIX)
    LUNACAT_LINK_DEPENDENCIES_LIBRARY(z)
    LUNACAT_LINK_DEPENDENCIES_LIBRARY(ssl)
    LUNACAT_LINK_DEPENDENCIES_LIBRARY(crypto)
endif()

if(LUNACAT_TARGET_WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} Ws2_32.lib)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} Crypt32.lib)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} Bcrypt.lib)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} Secur32.lib)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} Dnsapi.lib)
elseif(LUNACAT_TARGET_UNIX)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} resolv)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} rt)
    
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} Threads::Threads)
ELSEIF(LUNACAT_TARGET_OSX)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} resolv)
endif()

TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_json)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_memory)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_log)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_log_tcp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_log_file)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_log_console)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_http)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_db)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_cache)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_token)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_storage)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_archive)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_pool)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_utils)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_thread)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_mutex)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_script)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_leaderboard)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_messages)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_economics)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_matching)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hb_events)

IF(MSVC)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} Iphlpapi.lib)
ENDIF()