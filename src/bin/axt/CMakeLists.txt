remake_find_package(libaxt CONFIG)

remake_add_executable(carmen-axt axt.c PREFIX OFF
  LINK axt_interface axt_core ${CARMEN_INTERFACE_LIBRARIES}
    ${LIBAXT_LIBRARIES}
)
