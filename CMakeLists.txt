cmake_minimum_required(VERSION 2.8)
project(libcf2py)

include(FindPkgConfig)
pkg_check_modules(MODS REQUIRED
    libconfig++
)
include_directories(${MODS_INCLUDE_DIRS})

add_executable(libcf2py main.cpp)
target_link_libraries(libcf2py
    ${MODS_LDFLAGS}
)

install(TARGETS libcf2py RUNTIME DESTINATION bin)
