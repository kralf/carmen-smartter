remake_find_library(dsp3000 libdsp3000/dsp3000.h PACKAGE libdsp3000)

remake_add_executables(LINK gyro_interface gyro_core
  ${CARMEN_PARAM_INTERFACE_LIBRARY} ${DSP3000_LIBRARY})
