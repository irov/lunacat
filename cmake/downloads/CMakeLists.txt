CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

OPTION(LUNACAT_DOWNLOADS_SILENT "download silent" OFF)

PROJECT(downloads)

INCLUDE(../base_template.cmake)
INCLUDE(../lunacat_template.cmake)
include(../download_template.cmake)

LUNACAT_GIT_CLONE(zlib https://github.com/madler/zlib.git "v1.2.13")
LUNACAT_GIT_CLONE(mongo https://github.com/mongodb/mongo-c-driver.git "1.24.1")
LUNACAT_GIT_CLONE(lua https://github.com/lua/lua.git "v5.4.6")
LUNACAT_GIT_CLONE(lz4 https://github.com/lz4/lz4.git "v1.9.4")
LUNACAT_GIT_CLONE(libevent https://github.com/libevent/libevent.git "release-2.1.12-stable")
LUNACAT_GIT_CLONE(jansson https://github.com/akheron/jansson "v2.14")
LUNACAT_GIT_CLONE(hiredis https://github.com/redis/hiredis.git "v1.1.0")
LUNACAT_GIT_CLONE(openssl https://github.com/janbar/openssl-cmake.git "1.1.1t-20230216")
LUNACAT_GIT_CLONE(hummingbird https://github.com/irov/hummingbird)