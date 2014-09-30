cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  carmen-smartter
  VERSION 0.1
  RELEASE alpha
  SUMMARY "CARMEN modules for the SmartTer."
  AUTHOR "Ralf Kaestner"
  AUTHOR "Dizan Vasquez"
  CONTACT "ralf.kaestner@gmail.com"
  HOME "http://github.com/kralf/carmen-smartter"
  LICENSE "GNU Lesser General Public License (LGPL)"
  INSTALL /usr
  PREFIX carmen-
)

remake_add_directories(
  data
  COMPONENT data
)

remake_pack_deb(
  DEPENDS carmen libaxt libdsp3000 libsmartter
)
remake_pack_deb(
  COMPONENT dev
  DESCRIPTION "development headers"
  DEPENDS carmen-smartter carmen-core-dev libaxt-dev libdsp3000-dev
    libsmartter-dev
)
remake_pack_deb(
  ARCH all
  COMPONENT data
  DESCRIPTION "map data"
)

remake_distribute_deb(
  DISTRIBUTION lucid
  SECTION libs
  UPLOAD ppa:kralf/asl
  DEPENDS carmen-dev libaxt-dev libdsp3000-dev libsmartter-dev
    remake pkg-config doxygen
  PASS CMAKE_BUILD_TYPE CARMEN_SMARTTER_GIT_REVISION
)
remake_distribute_deb(
  DISTRIBUTION precise
  SECTION libs
  UPLOAD ppa:kralf/asl
  DEPENDS carmen-dev libaxt-dev libdsp3000-dev libsmartter-dev
    remake pkg-config doxygen
  PASS CMAKE_BUILD_TYPE CARMEN_SMARTTER_GIT_REVISION
)
remake_distribute_deb(
  DISTRIBUTION trusty
  SECTION libs
  UPLOAD ppa:kralf/asl
  DEPENDS carmen-dev libaxt-dev libdsp3000-dev libsmartter-dev
    remake pkg-config doxygen
  PASS CMAKE_BUILD_TYPE CARMEN_SMARTTER_GIT_REVISION
)
