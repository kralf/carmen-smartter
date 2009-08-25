project( elrob-carmen )

cmake_minimum_required( VERSION 2.6 )

set( PACKAGE_NAME ${PROJECT_NAME} )
set( PACKAGE_VERSION 0.0.1 )
set( PACKAGE_RELEASE 0 )
set( PACKAGE_MAINTAINER_NAME "Dizan Vasquez" )
set( PACKAGE_MAINTAINER_EMAIL "vasquez@mavt.ethz.ch" )
set( PACKAGE_DESCRIPTION_SUMMARY "Elrob to carmen tranlators." )
set( PACKAGE_DESCRIPTION "Elrob to carmen message translation for navigation and localization." )
set( PACKAGE_DEPENDS "rotor" )

include( SimpleCMake )
sm_configure_project()
