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
#   Copyright (C) 2002-2019 by Thomas Dreibholz
#
#   Authors: Thomas Dreibholz, dreibh@iem.uni-due.de
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
# Contact: dreibh@iem.uni-due.de

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(rserpooldemo)

SET(BUILD_MAJOR "3")
SET(BUILD_MINOR "0")
SET(BUILD_PATCH "0~rc1.19")
SET(BUILD_VERSION ${BUILD_MAJOR}.${BUILD_MINOR}.${BUILD_PATCH})


#############################################################################
# INSTALLATION_DIRECTORIES
#############################################################################

SET(CMAKE_INSTALL_BINDIR        "bin"       CACHE PATH "user executables [PREFIX/bin]")
SET(CMAKE_INSTALL_SBINDIR       "sbin"      CACHE PATH "system admin executables [EPREFIX/sbin]")
SET(CMAKE_INSTALL_DATADIR       "share"     CACHE PATH "read-only architecture-independent data [PREFIX/share]")
SET(CMAKE_INSTALL_SYSCONFDIR    "etc"       CACHE PATH "read-only single-machine data [PREFIX/etc]")
SET(CMAKE_INSTALL_LOCALSTATEDIR "var"       CACHE PATH "modifiable single-machine data [PREFIX/var]")
SET(CMAKE_INSTALL_LIBDIR        "lib"       CACHE PATH "object code libraries [PREFIX/lib]")
SET(CMAKE_INSTALL_INCLUDEDIR    "include"   CACHE PATH "read-only includes [PREFIX/include]")
SET(CMAKE_INSTALL_MANDIR        "share/man" CACHE PATH "man documentation [PREFIX/share/man]")


#############################################################################
# PACKAGING
#############################################################################

SET(CPACK_SOURCE_GENERATOR "TGZ")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME
   "${CMAKE_PROJECT_NAME}-${BUILD_MAJOR}.${BUILD_MINOR}.${BUILD_PATCH}")
SET(CPACK_SOURCE_IGNORE_FILES
   "${PROJECT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}-*;${PROJECT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}_*;/build/;/.git/;;/.gimp/;/config.log;/CMakeFiles/;CMakeCache.txt$;.cmake$;.tar.gz$;/_CPack_Packages;/Makefile$;~$;/build-deb;/clean-deb;/filter-empty-entries;/make-symbols;/make-ppa;/make-deb;/debian.conf;/make-rpm;/rpm.conf;${CPACK_SOURCE_IGNORE_FILES};/background/Output/")
INCLUDE(CPack)

ADD_CUSTOM_TARGET(dist COMMAND ${CMAKE_MAKE_PROGRAM} clean package_source)


INCLUDE(CheckIncludeFile)
INCLUDE(CheckStructHasMember)


#############################################################################
# OS-DEPENDENT
#############################################################################

IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    MESSAGE(STATUS ${CMAKE_SYSTEM_NAME} " supported")
    ADD_DEFINITIONS("-D_DEFAULT_SOURCE -DLINUX")

ELSEIF (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    MESSAGE(STATUS ${CMAKE_SYSTEM_NAME} " supported")
    SET(CMAKE_REQUIRED_INCLUDES "/usr/local/include" "/usr/include")
    SET(CMAKE_LIBRARY_PATH "/usr/local/lib")
    INCLUDE_DIRECTORIES("/usr/local/include")
    ADD_DEFINITIONS("-DFreeBSD")

ELSEIF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    MESSAGE(STATUS ${CMAKE_SYSTEM_NAME} " supported")
    SET(CMAKE_REQUIRED_INCLUDES "/usr/local/include" "/usr/include" "/usr/local/opt/openssl/include")
    SET(CMAKE_LIBRARY_PATH "/usr/local/lib")
    INCLUDE_DIRECTORIES("/usr/local/include" "/usr/local/opt/openssl/include")
    ADD_DEFINITIONS("-DDARWIN")

ELSEIF (${CMAKE_SYSTEM_NAME} MATCHES "NetBSD")
    MESSAGE(STATUS ${CMAKE_SYSTEM_NAME} " supported")
    LIST(APPEND neat_headers neat_bsd.h)
    LIST(APPEND neat_sources neat_bsd.c)
    SET(CMAKE_REQUIRED_INCLUDES "/usr/pkg/include" "/usr/include" "/usr/local/include")
    SET(CMAKE_LIBRARY_PATH "/usr/local/lib")
    INCLUDE_DIRECTORIES("/usr/pkg/include" "/usr/local/include")
    ADD_DEFINITIONS("-DNetBSD")

ELSEIF (${CMAKE_SYSTEM_NAME} MATCHES "NetBSD")
    MESSAGE(STATUS ${CMAKE_SYSTEM_NAME} " supported")
    ADD_DEFINITIONS("-DSOLARIS")

ELSE()
    MESSAGE(FATAL_ERROR ${CMAKE_SYSTEM_NAME} " not supported (yet?)")

ENDIF()


#############################################################################
# REQUIREMENTS
#############################################################################

# ###### Qt #################################################################
OPTION(ENABLE_QT "Include Qt programs" 1)
IF (ENABLE_QT)
   SET(CMAKE_AUTOMOC ON)
   FIND_PACKAGE(Qt5 COMPONENTS Core Xml Widgets Network REQUIRED)
ENDIF()


#############################################################################
# COMPILER FLAGS
#############################################################################

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_STANDARD_REQUIRED ON)
SET(CMAKE_C_EXTENSIONS OFF)

SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -Werror")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

IF (CMAKE_BUILD_TYPE MATCHES DEBUG)
    MESSAGE(STATUS "Building in DEBUG mode")
    # _FORTIFY_SOURCE requires compiling with optimization
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O")
ENDIF()


#############################################################################
# SOURCE DIRECTORIES
#############################################################################

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(background)
ADD_SUBDIRECTORY(grub)
