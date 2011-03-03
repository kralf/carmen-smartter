remake_find_library(
  carmen-global carmen.h
  PACKAGE CARMEN
)
remake_find_library(
  carmen-param_interface param_interface.h
  PACKAGE CARMEN
)
remake_find_library(
  tulibs-thread thread.h
  PACKAGE tulibs
)
remake_find_library(
  era era.h
  PACKAGE era-libs
)

remake_define(REDHAT_52 ON)
remake_define(REDHAT_6 ON)
remake_define(REDHAT_71 ON)

remake_include(${ERA_HEADERS})

remake_add_directories()
