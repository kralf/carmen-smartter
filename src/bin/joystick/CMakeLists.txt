remake_find_library(
  carmen-joyctrl joyctrl.h
  PACKAGE CARMEN
)

remake_add_executables(LINK smart_interface ${CARMEN_INTERFACE_LIBRARIES}
  ${CARMEN_LIBRARIES})
