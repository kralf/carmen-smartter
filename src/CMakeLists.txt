remake_find_library(
  carmen-global carmen.h
  PACKAGE CARMEN
)
remake_find_library(
  carmen-param_interface param_interface.h
  PACKAGE CARMEN
)
remake_find_library(
  carmen-base_interface base_interface.h
  PACKAGE CARMEN
)
remake_find_library(
  smartter libsmartter/smart.h
  PACKAGE libsmartter
)

remake_define(REDHAT_52 ON)
remake_define(REDHAT_6 ON)
remake_define(REDHAT_71 ON)

remake_add_directories()
