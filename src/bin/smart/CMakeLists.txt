remake_find_library(
  smartter libsmartter/smart.h
  PACKAGE libsmartter
)

remake_add_executables(LINK smart_interface smart_core
  ${CARMEN_PARAM_INTERFACE_LIBRARY} ${CARMEN_BASE_INTERFACE_LIBRARY}
  ${SMARTTER_LIBRARY})
