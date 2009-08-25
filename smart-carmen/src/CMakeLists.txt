#-------------------------------------------------------------------------------
# Common libraries
#-------------------------------------------------------------------------------

find_library( ROTOR_LIB rotor )
find_library( IPC_LIB ipc )
find_library( SMART_LIB smart )
find_library( DSP3000_LIB dsp3000 )
find_library( CARMEN_GLOBAL_LIB global )
find_library( CARMEN_PARAM_LIB param_interface )
find_library( CARMEN_BASE_LIB base_interface )
find_library( CARMEN_ROBOT_LIB robot_interface )
find_library( CARMEN_LOCALIZE_LIB localize_interface )
find_library( CARMEN_SIMULATOR_LIB simulator_interface )
find_library( CARMEN_GPS_LIB gps_nmea_interface )
find_library( CARMEN_LASER_LIB laser_interface )
find_library( Z_LIB z )

#-------------------------------------------------------------------------------
# alasca-interface
#-------------------------------------------------------------------------------

file( GLOB ALASCA_INTERFACE_SRC alasca-interface/*.c )

add_library( 
  alasca-interface SHARED
  ${ALASCA_INTERFACE_SRC}
)

sm_cpp_install( alasca-interface )

#-------------------------------------------------------------------------------
# axtCarmen
#-------------------------------------------------------------------------------

file( GLOB AXT_CARMEN_SRC alasca/*.c alasca/*.cpp )

add_executable( axtCarmen ${AXT_CARMEN_SRC} )

target_link_libraries( 
  axtCarmen 
  m
  alasca-interface
  ${Z_LIB} 
  ${CARMEN_GLOBAL_LIB} 
  ${IPC_LIB} 
)

sm_cpp_install( axtCarmen )

#-------------------------------------------------------------------------------
# smart-interface
#-------------------------------------------------------------------------------

file( GLOB SMART_INTERFACE_SRC smart-interface/*.c )

add_library( 
  smart-interface SHARED
  ${SMART_INTERFACE_SRC}
)

sm_cpp_install( smart-interface )

#-------------------------------------------------------------------------------
# smartCarmen
#-------------------------------------------------------------------------------

file( GLOB SMART_CARMEN_SRC smart/*.c smart/*.cpp )

add_executable( smartCarmen ${SMART_CARMEN_SRC} )

target_link_libraries( 
  smartCarmen
  ${CARMEN_BASE_LIB}
  ${Z_LIB} 
  ${CARMEN_GLOBAL_LIB} 
  ${CARMEN_PARAM_LIB}
  ${IPC_LIB} 
  ${SMART_LIB}
)

sm_cpp_install( smartCarmen )

#-------------------------------------------------------------------------------
# gyro-interface
#-------------------------------------------------------------------------------

file( GLOB GYRO_INTERFACE_SRC gyro-interface/*.c )

add_library( 
  gyro-interface SHARED
  ${GYRO_INTERFACE_SRC}
)

sm_cpp_install( gyro-interface )

#-------------------------------------------------------------------------------
# gyroCarmen
#-------------------------------------------------------------------------------

file( GLOB GYRO_CARMEN_SRC gyro/*.c gyro/*.cpp )

add_executable( gyroCarmen ${GYRO_CARMEN_SRC} )

target_link_libraries( 
  gyroCarmen
  ${Z_LIB} 
  ${CARMEN_GLOBAL_LIB} 
  ${CARMEN_PARAM_LIB}
  ${IPC_LIB} 
  ${DSP3000_LIB}
)

sm_cpp_install( gyroCarmen )

#-------------------------------------------------------------------------------
# locfilter-interface
#-------------------------------------------------------------------------------

file( GLOB LOCFILTER_INTERFACE_SRC locfilter-interface/*.c )

add_library( 
  locfilter-interface SHARED
  ${LOCFILTER_INTERFACE_SRC}
)

sm_cpp_install( locfilter-interface )

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
  ${CARMEN_LOCALIZE_LIB}
  ${Z_LIB} 
  ${CARMEN_GLOBAL_LIB} 
  ${CARMEN_PARAM_LIB}
  ${IPC_LIB} 
)

sm_cpp_install( locfilterCarmen )

#-------------------------------------------------------------------------------
# logger-write
#-------------------------------------------------------------------------------

file( GLOB LOGGER_WRITE_SRC logger-write/*.c )

add_library( 
  logger-write SHARED
  ${LOGGER_WRITE_SRC}
)

target_link_libraries( 
  logger-write
  smart-interface
  gyro-interface
  ${CARMEN_BASE_LIB} 
  ${CARMEN_ROBOT_LIB} 
  ${CARMEN_LOCALIZE_LIB} 
  ${CARMEN_SIMULATOR_LIB} 
  ${CARMEN_GPS_LIB} 
  ${CARMEN_LASER_LIB} 
)

sm_cpp_install( logger-write )

#-------------------------------------------------------------------------------
# logger
#-------------------------------------------------------------------------------

file( GLOB LOGGER_SRC logger/*.c logger/*.cpp )

add_executable( elroblogger ${LOGGER_SRC} )

target_link_libraries( 
  elroblogger
  m
  logger-write
  ${Z_LIB} 
  ${CARMEN_GLOBAL_LIB} 
  ${CARMEN_PARAM_LIB} 
  ${IPC_LIB} 
)

sm_cpp_install( elroblogger )

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
