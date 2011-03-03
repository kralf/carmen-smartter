remake_find_library(
  carmen-writelog writelog.h
  PACKAGE CARMEN
)

remake_include(../../lib/writelog)
remake_add_executables(LINK smartter_writelog
  ${CARMEN_PARAM_INTERFACE_LIBRARY} ${CARMEN_WRITELOG_LIBRARY})
