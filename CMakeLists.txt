# ##########################################################################
#
#              //===//   //=====   //===//   //       //   //===//
#             //    //  //        //    //  //       //   //    //
#            //===//   //=====   //===//   //       //   //===<<
#           //   \\         //  //        //       //   //    //
#          //     \\  =====//  //        //=====  //   //===//   Version III
#
#             ###################################################
#           ======  D E M O N S T R A T I O N   S Y S T E M  ======
#             ###################################################
#
# ############# An Efficient RSerPool Prototype Implementation #############
#
#   Copyright (C) 2002-2026 by Thomas Dreibholz
#
#   Authors: Thomas Dreibholz, thomas.dreibholz@gmail.com
#            Sebastian Rohde, rohde@iem.uni-due.de
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Contact: thomas.dreibholz@gmail.com

CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(rserpooldemo LANGUAGES CXX)

SET(BUILD_MAJOR "3")
SET(BUILD_MINOR "2")
SET(BUILD_PATCH "8")
SET(BUILD_VERSION ${BUILD_MAJOR}.${BUILD_MINOR}.${BUILD_PATCH})


#############################################################################
#### CMAKE INCLUDES                                                      ####
#############################################################################

INCLUDE(CheckCCompilerFlag)
INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckCSourceRuns)
INCLUDE(CheckIncludeFile)
INCLUDE(CheckStructHasMember)
INCLUDE(GNUInstallDirs)


#############################################################################
#### PACKAGING                                                           ####
#############################################################################

SET(CPACK_SOURCE_GENERATOR "TXZ")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${BUILD_MAJOR}.${BUILD_MINOR}.${BUILD_PATCH}")
SET(CPACK_SOURCE_IGNORE_FILES "\\\\.git;\\\\.swp$;~$;\\\\.\\\\#;/\\\\#")
LIST(APPEND CPACK_SOURCE_IGNORE_FILES "^${PROJECT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}[_-]")
LIST(APPEND CPACK_SOURCE_IGNORE_FILES "\\\\.make$|\\\\.log$")
LIST(APPEND CPACK_SOURCE_IGNORE_FILES "/CMakeCache\\\\.txt$")
LIST(APPEND CPACK_SOURCE_IGNORE_FILES "/(CMakeFiles|CMakeScripts|_CPack_Packages)/")
LIST(APPEND CPACK_SOURCE_IGNORE_FILES "/package-version\\\\.h$")
LIST(APPEND CPACK_SOURCE_IGNORE_FILES "/packaging\\\\.conf$")
LIST(APPEND CPACK_SOURCE_IGNORE_FILES "^${PROJECT_SOURCE_DIR}/(po.*/|src.*/|)Makefile$")
LIST(APPEND CPACK_SOURCE_IGNORE_FILES "^${PROJECT_SOURCE_DIR}/(po.*|src.*|[^/]*)\\\\.cmake$")
INCLUDE(CPack)

ADD_CUSTOM_TARGET(dist COMMAND ${CMAKE_MAKE_PROGRAM} clean package_source)


#############################################################################
#### UNINSTALL                                                           ####
#############################################################################

IF(NOT TARGET uninstall)
   CONFIGURE_FILE(
      "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
      "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
      IMMEDIATE @ONLY)

   ADD_CUSTOM_TARGET(uninstall
      COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
ENDIF()


#############################################################################
#### OS DEPENDENT                                                        ####
#############################################################################

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
   MESSAGE(STATUS "${CMAKE_SYSTEM_NAME} supported")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   MESSAGE(STATUS "${CMAKE_SYSTEM_NAME} supported")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "NetBSD")
   MESSAGE(STATUS "${CMAKE_SYSTEM_NAME} supported")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
   MESSAGE(STATUS "${CMAKE_SYSTEM_NAME} supported")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "SunOS")
   MESSAGE(STATUS "${CMAKE_SYSTEM_NAME} supported")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "GNU")
   MESSAGE(STATUS "${CMAKE_SYSTEM_NAME} supported")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
   MESSAGE(STATUS "${CMAKE_SYSTEM_NAME} supported")
ELSE()
   MESSAGE(FATAL_ERROR "${CMAKE_SYSTEM_NAME} not supported (yet?)")
ENDIF()


#############################################################################
#### REQUIREMENTS                                                        ####
#############################################################################

# ====== Qt =================================================================
FIND_PROGRAM(QMAKE NAMES qmake6 qmake-qt6 qmake5 qmake-qt5 qmake PATHS /usr/lib/qt/*/bin)
IF (QMAKE)
   EXECUTE_PROCESS(
      COMMAND "${QMAKE}" -query QT_INSTALL_LIBS
      OUTPUT_VARIABLE QT_INSTALL_LIBS
      OUTPUT_STRIP_TRAILING_WHITESPACE
   )
   LIST(APPEND CMAKE_PREFIX_PATH "${QT_INSTALL_LIBS}/cmake")
ENDIF()

SET(CMAKE_AUTOMOC ON)
FIND_PACKAGE(Qt6 COMPONENTS Core Network Widgets Xml PrintSupport)
IF (NOT Qt6_FOUND)
   # Try Qt5 instead:
   FIND_PACKAGE(Qt5 COMPONENTS Core Network Widgets Xml PrintSupport)
ENDIF()
IF ((NOT Qt6_FOUND) AND (NOT Qt5_FOUND))
   MESSAGE(FATAL_ERROR
           "Cannot find Qt! Try:\n"
           " * Ubuntu/Debian: sudo apt install -y qt6-base-dev\n"
           " * Fedora:        sudo dnf install -y qt6-qtbase-devel\n"
           " * OpenSUSE:      sudo zypper install -y qt6-base-devel\n"
           " * Alpine:        sudo apk add qt6-qtbase-dev\n"
           " * FreeBSD:       sudo pkg install -y qt6-base\n"
           " * NetBSD:        sudo pkgin -y install qt6-qtbase\n"
           " * OpenBSD:       sudo pkg_add qt6-qtbase\n"
           " * Solaris:       sudo pkg install library/qt6 print/cups\n"
           " * Apple:         brew install qt")
ELSEIF (Qt6_FOUND)
   SET(QT_FOUND TRUE)
   SET(QT_VERSION_MAJOR ${Qt6_VERSION_MAJOR})
ELSEIF (Qt5_FOUND)
   SET(QT_FOUND TRUE)
   SET(QT_VERSION_MAJOR ${Qt5_VERSION_MAJOR})
ENDIF()
MESSAGE("Using Qt version ${QT_VERSION_MAJOR}!")


#############################################################################
#### SOURCE DIRECTORIES                                                  ####
#############################################################################

ADD_SUBDIRECTORY(src)
