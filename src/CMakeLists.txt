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

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# ###### Program ############################################################
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

TARGET_LINK_LIBRARIES(rserpooldemo Qt5::Widgets Qt5::Xml Qt5::Core Qt5::Network)
INSTALL(TARGETS             rserpooldemo
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION  ${CMAKE_INSTALL_BINDIR})
# INSTALL(FILES rserpooldemo.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
