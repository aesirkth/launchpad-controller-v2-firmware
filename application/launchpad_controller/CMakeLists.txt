cmake_minimum_required(VERSION 3.20.0)

set(BOARD_ROOT ${CMAKE_CURRENT_LIST_DIR})


# if(TEST_BUILD)
#     set(EXTRA_CONF_FILE ${CMAKE_CURRENT_LIST_DIR}/testing/preferences.conf)
# else()
#     set(EXTRA_CONF_FILE ${CMAKE_CURRENT_LIST_DIR}/preferences.conf)
# endif()

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(launchpad_controller)

target_sources(app PRIVATE
    src/main.c
)

# # main
# target_sources(app PRIVATE
#     src/flight_state.c
#     src/sensors.c
#     src/communication.c
#     src/commands.c
#     src/actuation.c
#     src/filter.c
# )

# if(TEST_BUILD)
#     message(STATUS "Building test")
#     target_sources(app PRIVATE
#         testing/test_main.c
#     )
# else()
#     message(STATUS "Building app")
#     target_sources(app PRIVATE
#         src/main.c
#     )
# endif()