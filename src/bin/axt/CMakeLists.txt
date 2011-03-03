remake_find_library(axt libaxt/axt.h PACKAGE libaxt)

remake_add_executables(LINK axt_interface axt_core
  ${CARMEN_PARAM_INTERFACE_LIBRARY} ${AXT_LIBRARY})
