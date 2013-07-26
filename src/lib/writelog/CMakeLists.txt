remake_include(../interface/*)

remake_add_library(
  smartter_writelog
  LINK axt_interface gyro_interface smart_interface
    ${CARMEN_INTERFACE_LIBRARIES}
)
remake_add_headers()
