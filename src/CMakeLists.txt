remake_find_package(carmen-core CONFIG)
remake_find_package(carmen-interface CONFIG)
remake_find_package(carmen CONFIG)
remake_find_library(m math.h PACKAGE libm)

remake_define(REDHAT_52 ON)
remake_define(REDHAT_6 ON)
remake_define(REDHAT_71 ON)

remake_add_directories()
remake_pkg_config_generate(REQUIRES carmen-core)
