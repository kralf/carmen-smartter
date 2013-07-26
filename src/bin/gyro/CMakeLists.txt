remake_find_package(libdsp3000 CONFIG)

remake_add_executable(carmen-dsp3000 dsp3000.c PREFIX OFF
  LINK gyro_interface gyro_core ${CARMEN_INTERFACE_LIBRARIES}
    ${LIBDSP3000_LIBRARIES}
)
