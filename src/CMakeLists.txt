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
#   Copyright (C) 2002-2025 by Thomas Dreibholz
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

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

#############################################################################
#### PROGRAMS                                                            ####
#############################################################################

ADD_EXECUTABLE(rserpooldemo
   csplistener.cc
   rdconfignode.cc
   rdconfiguration.cc
   rdcontextmenuconfig.cc
   rdgraphicsnode.cc
   rdgraphicsscene.cc
   rdgraphicsview.cc
   rdmainwindow.cc
   rserpooldemo.cc
)

TARGET_LINK_LIBRARIES(rserpooldemo Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Xml)
INSTALL(TARGETS             rserpooldemo
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION  ${CMAKE_INSTALL_BINDIR})
INSTALL(FILES rserpooldemo.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

INSTALL(FILES rserpooldemo.png     DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/512x512/apps)
INSTALL(FILES rserpooldemo.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
INSTALL(FILES rserpooldemo.xml     DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/mime/packages)


#############################################################################
#### CONFIGURATION FILES                                                 ####
#############################################################################

INSTALL(PROGRAMS system-info.d/30-rsplib DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR}/system-info.d)

INSTALL(PROGRAMS system-maintenance.d/30-rsplib DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR}/system-maintenance.d)



#############################################################################
#### SOURCE DIRECTORIES                                                  ####
#############################################################################

ADD_SUBDIRECTORY(background)
ADD_SUBDIRECTORY(graphics)
ADD_SUBDIRECTORY(grub)

ADD_SUBDIRECTORY(local-scenario)
ADD_SUBDIRECTORY(nornet-mec-scenario)
