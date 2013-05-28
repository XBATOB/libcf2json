cmake_minimum_required(VERSION 2.8)
project(libcf2json)

include(FindPkgConfig)
pkg_check_modules(MODS REQUIRED
    libconfig++
    jsoncpp
)
include_directories(${MODS_INCLUDE_DIRS})

add_executable(libcf2json main.cpp)
target_link_libraries(libcf2json
    ${MODS_LDFLAGS}
)

install(TARGETS libcf2json RUNTIME DESTINATION bin)
