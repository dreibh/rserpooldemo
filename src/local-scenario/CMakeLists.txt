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

#############################################################################
# SCENARIO FILES
#############################################################################

LIST(APPEND scenario_files
   local-setup.xml
)

LIST(APPEND scenario_programs
   LocalScenario.config
   LocalScenario-StartAll
   LocalScenario-StopAll
   ComponentControl
   RunProgram
)

INSTALL(FILES    ${scenario_files}    DESTINATION ${CMAKE_INSTALL_DATADIR}/rserpooldemo/local-scenario)
INSTALL(PROGRAMS ${scenario_programs} DESTINATION ${CMAKE_INSTALL_DATADIR}/rserpooldemo/local-scenario)
