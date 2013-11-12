cmake_minimum_required(VERSION 2.8)
project(libcf2json)

include(FindPkgConfig)
pkg_check_modules(MODS REQUIRED
    libconfig++
    jsoncpp
)
add_definitions(-DENABLE_NLS)
include_directories(${MODS_INCLUDE_DIRS})

add_executable(libcf2json main.cpp)
target_link_libraries(libcf2json
    ${MODS_LDFLAGS}
)

install(TARGETS libcf2json RUNTIME DESTINATION bin)

set(CPACK_GENERATOR RPM)
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_PACKAGE_NAME "libcf2json")
set(CPACK_PACKAGE_VENDOR "RuToll.RU")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "convert configuration from format libconfig++ to json")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

include(CPack)
