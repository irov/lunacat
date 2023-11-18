CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

include_directories(${LUNACAT_DEPENDENCIES_SOURCE_DIRECTORY}/hummingbird/src)
include_directories(.)

ADD_SUBDIRECTORY(hb_economics)
ADD_SUBDIRECTORY(hb_events)
ADD_SUBDIRECTORY(hb_leaderboard)
ADD_SUBDIRECTORY(hb_matching)
ADD_SUBDIRECTORY(hb_messages)
ADD_SUBDIRECTORY(hb_script)

ADD_SUBDIRECTORY(hb_grid)