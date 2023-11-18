PROJECT(hb_script LANGUAGES C)

LUNACAT_ADD_FILTER(
src
    hb_script.h
    hb_script.c
    hb_script_compiler.h
    hb_script_compiler.c
    hb_script_json.h
    hb_script_json.c
    
    hb_script_handle.h
    hb_script_handle.c
)

LUNACAT_ADD_FILTER(
src_project
    hb_script_server_SetProjectPublicData.c
    hb_script_server_GetProjectPublicData.c
    hb_script_server_UpdateProjectPublicData.c
)

LUNACAT_ADD_FILTER(
src_user
    hb_script_server_GetCurrentUserPublicData.c
    hb_script_server_SetCurrentUserPublicData.c
    hb_script_server_UpdateCurrentUserPublicData.c
)

LUNACAT_ADD_FILTER(
src_user_entity
    hb_script_server_CreateUserEntity.c
    hb_script_server_SelectUserEntity.c
    hb_script_server_GetUserEntityPublicData.c
    hb_script_server_SetUserEntityPublicData.c
    hb_script_server_UpdateUserEntityPublicData.c
)

LUNACAT_ADD_FILTER(
src_project_entity
    hb_script_server_CreateProjectEntity.c
    hb_script_server_GetProjectEntity.c
    hb_script_server_SelectProjectEntity.c
    hb_script_server_GetProjectEntityPublicData.c
    hb_script_server_SetProjectEntityPublicData.c
    hb_script_server_UpdateProjectEntityPublicData.c
)

LUNACAT_ADD_FILTER(
src_world
    hb_script_server_GetWorldPublicData.c
    hb_script_server_SetWorldPublicData.c
    hb_script_server_UpdateWorldPublicData.c
    hb_script_server_SetAllowWorldTurn.c
)

LUNACAT_ADD_FILTER(
src_matching
    hb_script_server_CreateMatching.c
    hb_script_server_JoinMatching.c
    hb_script_server_GetStatusMatching.c
)

INCLUDE_DIRECTORIES(${LUNACAT_DEPENDENCIES_SOURCE_DIRECTORY}/lua)

LUNACAT_ADD_LIBRARY()

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LUNACAT_DEPENDENCIES_LIBRARY_DIRECTORY}/${CMAKE_STATIC_LIBRARY_PREFIX}lua${CMAKE_STATIC_LIBRARY_SUFFIX})

IF(LUNACAT_TARGET_UNIX)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} m)
endif()