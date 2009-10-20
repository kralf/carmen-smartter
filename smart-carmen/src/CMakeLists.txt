#-------------------------------------------------------------------------------
# Common libraries
#-------------------------------------------------------------------------------

find_library( SMART_LIB smart )
find_library( DSP3000_LIB dsp3000 )

find_library( IPC_LIB ipc )
find_library( Z_LIB z )

find_library( CARMEN_GLOBAL_LIB global )
find_library( CARMEN_PARAM_LIB param_interface )
find_library( CARMEN_BASE_LIB base_interface )
find_library( CARMEN_ROBOT_LIB robot_interface )
find_library( CARMEN_LOCALIZE_LIB localize_interface )
find_library( CARMEN_SIMULATOR_LIB simulator_interface )
find_library( CARMEN_GPS_LIB gps_nmea_interface )
find_library( CARMEN_LASER_LIB laser_interface )
find_library( CARMEN_ALASCA_LIB alasca-interface )
find_library( CARMEN_SMART_LIB smart-interface )
find_library( CARMEN_GYRO_LIB gyro-interface )
find_library( CARMEN_LOCFILTER_LIB locfilter-interface )

find_library( ROTOR_LIB rotor )


#-------------------------------------------------------------------------------
# axtCarmen
#-------------------------------------------------------------------------------

file( GLOB AXT_CARMEN_SRC alasca/*.c alasca/*.cpp )

add_executable( axtCarmen ${AXT_CARMEN_SRC} )

target_link_libraries(
  axtCarmen
  m
  ${CARMEN_ALASCA_LIB}
)

sm_cpp_install( axtCarmen )

#-------------------------------------------------------------------------------
# smartCarmen
#-------------------------------------------------------------------------------

file( GLOB SMART_CARMEN_SRC smart/*.c smart/*.cpp )

add_executable( smartCarmen ${SMART_CARMEN_SRC} )

target_link_libraries(
  smartCarmen
  ${SMART_LIB}
  ${CARMEN_SMART_LIB}
  ${CARMEN_BASE_LIB}
  ${Z_LIB}
  ${IPC_LIB}
  ${CARMEN_GLOBAL_LIB}
  ${CARMEN_PARAM_LIB}
)

sm_cpp_install( smartCarmen )

#-------------------------------------------------------------------------------
# gyroCarmen
#-------------------------------------------------------------------------------

file( GLOB GYRO_CARMEN_SRC gyro/*.c gyro/*.cpp )

add_executable( gyroCarmen ${GYRO_CARMEN_SRC} )

target_link_libraries(
  gyroCarmen
  ${DSP3000_LIB}
  ${CARMEN_GYRO_LIB}
  ${Z_LIB}
  ${IPC_LIB}
  ${CARMEN_GLOBAL_LIB}
  ${CARMEN_PARAM_LIB}
)

sm_cpp_install( gyroCarmen )


#-------------------------------------------------------------------------------
# locfilterCarmen
#-------------------------------------------------------------------------------

file( GLOB LOCFILTER_CARMEN_SRC locfilter/*.c locfilter/*.cpp )

add_executable( locfilterCarmen ${LOCFILTER_CARMEN_SRC} )

target_link_libraries(
  locfilterCarmen
  gyro-interface
  m
  ${CARMEN_BASE_LIB}
  ${CARMEN_LOCFILTER_LIB}
  ${CARMEN_LOCALIZE_LIB}
  ${Z_LIB}
  ${IPC_LIB}
  ${CARMEN_GLOBAL_LIB}
  ${CARMEN_PARAM_LIB}
)

sm_cpp_install( locfilterCarmen )

#-------------------------------------------------------------------------------
# smartFrontLaser
#-------------------------------------------------------------------------------

file( GLOB SMART_FRONT_LASER_SRC smartFrontLaser/*.cpp )

add_executable( smartFrontLaser ${SMART_FRONT_LASER_SRC} )

target_link_libraries(
  smartFrontLaser
  ${ROTOR_LIB}
)

sm_cpp_install( smartFrontLaser )

#-------------------------------------------------------------------------------
# elrobLocalize
#-------------------------------------------------------------------------------

file( GLOB SMART_LOCALIZE_SRC smartLocalize/*.cpp )

add_executable( smartLocalize ${SMART_LOCALIZE_SRC} )

target_link_libraries(
  smartLocalize
  ${ROTOR_LIB}
)

sm_cpp_install( smartLocalize )

#-------------------------------------------------------------------------------
# joysmartCarmen
#-------------------------------------------------------------------------------

file( GLOB JOYSMART_CARMEN_SRC joystick/*.c joystick/*.cpp )

add_executable( joysmartCarmen ${JOYSMART_CARMEN_SRC} )

target_link_libraries(
  joysmartCarmen
  ${CARMEN_SMART_LIB}
  ${CARMEN_ROBOT_LIB}
  ${Z_LIB}
  ${IPC_LIB}
  ${CARMEN_GLOBAL_LIB}
  ${CARMEN_PARAM_LIB}
  m
)

sm_cpp_install( joysmartCarmen )
