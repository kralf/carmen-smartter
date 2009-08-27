project( smart-carmen )

cmake_minimum_required( VERSION 2.6 )

set( PACKAGE_NAME ${PROJECT_NAME} )
set( PACKAGE_VERSION 0.0.1 )
set( PACKAGE_RELEASE 0 )
set( PACKAGE_MAINTAINER_NAME "Dizan Vasquez" )
set( PACKAGE_MAINTAINER_EMAIL "vasquez@mavt.ethz.ch" )
set( PACKAGE_DESCRIPTION_SUMMARY "Carmen drivers for the smartter platform." )
set( PACKAGE_DESCRIPTION "Carmen drivers fo the Smartter hardware." )
set( PACKAGE_DEPENDS "rotor smart-hardware smart-interfaces" )

include( SimpleCMake )
sm_configure_project()
