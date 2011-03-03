cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  "CARMEN ERA"
  VERSION 0.1
  RELEASE alpha
  SUMMARY "CARMEN modules for the Embedded Robot Arm (ERA)."
  AUTHOR "Ralf Kaestner"
  CONTACT "ralf.kaestner@gmail.com"
  HOME "http://aslforge.ethz.ch/projects/carmen-era"
  LICENSE "GNU Lesser General Public License (LGPL)"
  INSTALL /usr
  PREFIX carmen-
)

remake_pack_deb(
  carmen-interface era-libs tulibs
)
remake_pack_deb(
  COMPONENT dev
  DESCRIPTION "development headers"
  carmen_era carmen-interface-dev era-libs-dev
)
