#-------------------------------------------------------------------------------
# Common libraries
#-------------------------------------------------------------------------------

find_library( ROTOR_LIB rotor )
find_library( IPC_LIB ipc )
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

target_link_libraries(   
  alasca-interface
  ${Z_LIB} 
  ${IPC_LIB} 
  ${CARMEN_GLOBAL_LIB} 
)


sm_cpp_install( alasca-interface )


#-------------------------------------------------------------------------------
# smart-interface
#-------------------------------------------------------------------------------

file( GLOB SMART_INTERFACE_SRC smart-interface/*.c )

add_library( 
  smart-interface SHARED
  ${SMART_INTERFACE_SRC}
)

target_link_libraries(   
  smart-interface
  ${Z_LIB} 
  ${IPC_LIB} 
  ${CARMEN_GLOBAL_LIB} 
)

sm_cpp_install( smart-interface )

#-------------------------------------------------------------------------------
# gyro-interface
#-------------------------------------------------------------------------------

file( GLOB GYRO_INTERFACE_SRC gyro-interface/*.c )

add_library( 
  gyro-interface SHARED
  ${GYRO_INTERFACE_SRC}
)

target_link_libraries(   
  gyro-interface
  ${Z_LIB} 
  ${IPC_LIB} 
  ${CARMEN_GLOBAL_LIB} 
)

sm_cpp_install( gyro-interface )

#-------------------------------------------------------------------------------
# locfilter-interface
#-------------------------------------------------------------------------------

file( GLOB LOCFILTER_INTERFACE_SRC locfilter-interface/*.c )

add_library( 
  locfilter-interface SHARED
  ${LOCFILTER_INTERFACE_SRC}
)

target_link_libraries(   
  locfilter-interface
  ${Z_LIB} 
  ${IPC_LIB} 
  ${CARMEN_GLOBAL_LIB} 
)


sm_cpp_install( locfilter-interface )

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
  ${CARMEN_PARAM_LIB} 
)
