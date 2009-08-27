project( smart-interfaces )

cmake_minimum_required( VERSION 2.6 )

set( PACKAGE_NAME ${PROJECT_NAME} )
set( PACKAGE_VERSION 0.0.1 )
set( PACKAGE_RELEASE 0 )
set( PACKAGE_MAINTAINER_NAME "Dizan Vasquez" )
set( PACKAGE_MAINTAINER_EMAIL "vasquez@mavt.ethz.ch" )
set( PACKAGE_DESCRIPTION_SUMMARY "Carmen interfaces for the Smart platform" )
set( PACKAGE_DESCRIPTION "Contains all special interfaces and logging facilities for the SmartTer platform" )
set( PACKAGE_DEPENDS )

include( SimpleCMake )
sm_configure_project()
