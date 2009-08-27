#-------------------------------------------------------------------------------
# Common libraries
#-------------------------------------------------------------------------------

find_library( SMART_LIB smart )
find_library( DSP3000_LIB dsp3000 )

find_library( IPC_LIB ipc )

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
find_library( CARMEN_LOCFILTER_LIB loclib-interface )

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
  ${Z_LIB} 
  ${CARMEN_GLOBAL_LIB} 
  ${IPC_LIB} 
)

sm_cpp_install( axtCarmen )

#-------------------------------------------------------------------------------
# smartCarmen
#-------------------------------------------------------------------------------

file( GLOB SMART_CARMEN_SRC smart/*.c smart/*.cpp )

add_executable( smartCarmen ${SMART_CARMEN_SRC} )

target_link_libraries( 
  smartCarmen
  ${CARMEN_BASE_LIB}
  ${CARMEN_PARAM_LIB}
  ${SMART_LIB}
  ${CARMEN_SMART_LIB}
)

sm_cpp_install( smartCarmen )

#-------------------------------------------------------------------------------
# gyroCarmen
#-------------------------------------------------------------------------------

file( GLOB GYRO_CARMEN_SRC gyro/*.c gyro/*.cpp )

add_executable( gyroCarmen ${GYRO_CARMEN_SRC} )

target_link_libraries( 
  gyroCarmen
  ${CARMEN_PARAM_LIB}
  ${DSP3000_LIB}
  ${CARMEN_GYRO_LIB}
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
  ${CARMEN_LOCFILTER_LIB}
  ${CARMEN_LOCALIZE_LIB}
  ${CARMEN_PARAM_LIB}
)

sm_cpp_install( locfilterCarmen )

#-------------------------------------------------------------------------------
# elrobCarmen
#-------------------------------------------------------------------------------

file( GLOB ELROB_CARMEN_SRC elrob-carmen/*.cpp )

add_executable( elrobCarmen ${ELROB_CARMEN_SRC} )

target_link_libraries( 
  elrobCarmen
  ${ROTOR_LIB}
)

sm_cpp_install( elrobCarmen )

#-------------------------------------------------------------------------------
# elrobLocalize
#-------------------------------------------------------------------------------

file( GLOB ELROB_LOCALIZE_SRC elrob-localize/*.cpp )

add_executable( elrobLocalize ${ELROB_LOCALIZE_SRC} )

target_link_libraries( 
  elrobLocalize
  ${ROTOR_LIB}
)

sm_cpp_install( elrobLocalize )

#-------------------------------------------------------------------------------
# joysmartCarmen
#-------------------------------------------------------------------------------

file( GLOB JOYSMART_CARMEN_SRC joystick/*.c joystick/*.cpp )

add_executable( joysmartCarmen ${JOYSMART_CARMEN_SRC} )

target_link_libraries( 
  joysmartCarmen
  ${CARMEN_ROBOT_LIB}
  ${Z_LIB} 
  ${CARMEN_GLOBAL_LIB} 
  ${CARMEN_PARAM_LIB}
  ${IPC_LIB} 
  m
)

sm_cpp_install( joysmartCarmen )
