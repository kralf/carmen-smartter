remake_find_library(
  carmen-joyctrl joyctrl.h
  PACKAGE CARMEN
)

remake_add_executables(LINK smart_interface ${CARMEN_PARAM_INTERFACE_LIBRARY}
  ${CARMEN_JOYCTRL_LIBRARY})
