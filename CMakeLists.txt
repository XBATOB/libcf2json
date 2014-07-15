cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME libcf2json)
set(PROJECT_VERSION 1.0.0)

project(${PROJECT_NAME})

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

add_custom_target(FILES
    git ls-tree -r --name-only HEAD | grep -v '^obsolete' > FILES
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
set(PACKAGE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
add_custom_target(${PACKAGE_NAME}.tar.gz
    tar czf ${PACKAGE_NAME}.tar.gz "--transform=s:^:${PACKAGE_NAME}/:"
    --dereference --no-recursion --show-transformed -T FILES
    DEPENDS FILES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(arch DEPENDS ${PACKAGE_NAME}.tar.gz)
add_custom_target(srpm
    rpmbuild -ts ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_NAME}.tar.gz
    DEPENDS arch)
add_custom_target(rpm
    rpmbuild -ta ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_NAME}.tar.gz
    DEPENDS arch)
