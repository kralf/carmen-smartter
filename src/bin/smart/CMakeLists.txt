remake_find_package(libsmartter CONFIG)

remake_add_executables(LINK smart_interface smart_core
  ${CARMEN_INTERFACE_LIBRARIES} ${LIBSMARTTER_LIBRARIES})
