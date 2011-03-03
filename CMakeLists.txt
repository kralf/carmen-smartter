cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  "CARMEN SmartTer"
  VERSION 0.1
  RELEASE alpha
  SUMMARY "CARMEN modules for the SmartTer."
  AUTHOR "Ralf Kaestner"
  AUTHOR "Dizan Vasquez"
  CONTACT "ralf.kaestner@gmail.com"
  HOME "http://aslforge.ethz.ch/projects/carmen-smartter"
  LICENSE "GNU Lesser General Public License (LGPL)"
  INSTALL /usr
  PREFIX carmen-
)

remake_add_directories(
  data
  COMPONENT data
)

remake_pack_deb(
  carmen libaxt libdsp3000 libsmartter
)
remake_pack_deb(
  COMPONENT dev
  DESCRIPTION "development headers"
  carmen_smartter carmen-dev libaxt-dev libdsp3000-dev libsmartter-dev
)
remake_pack_deb(
  ARCH all
  COMPONENT data
  DESCRIPTION "map data"
)
