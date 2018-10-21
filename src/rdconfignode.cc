/*
 * ##########################################################################
 *
 *              //===//   //=====   //===//   //       //   //===//
 *             //    //  //        //    //  //       //   //    //
 *            //===//   //=====   //===//   //       //   //===<<
 *           //   \\         //  //        //       //   //    //
 *          //     \\  =====//  //        //=====  //   //===//   Version III
 *
 *             ###################################################
 *           ======  D E M O N S T R A T I O N   S Y S T E M  ======
 *             ###################################################
 *
 * ############# An Efficient RSerPool Prototype Implementation #############
 *
 *   Copyright (C) 2002-2019 by Thomas Dreibholz
 *
 *   Authors: Thomas Dreibholz, dreibh@iem.uni-due.de
 *            Sebastian Rohde, rohde@iem.uni-due.de
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact: dreibh@iem.uni-due.de
 */

#include "rdconfignode.h"
#include "csplistener.h"


// ###### Constructor #######################################################
RDConfigNode::RDConfigNode(const QString& uniqueID,
                           const QString& displayName,
                           const QString& imageActive,
                           const QString& imageInactive,
                           int            positionX,
                           int            positionY,
                           int            timeoutMultiplier)
   : m_UniqueID(uniqueID),
     m_DisplayName(displayName),
     m_ImageActive(imageActive),
     m_ImageInactive(imageInactive),
     m_PositionX(positionX),
     m_PositionY(positionY),
     m_TimeoutMultiplier(timeoutMultiplier),
     m_Status(INACTIVE),
     m_ReportInterval(6000000),
     m_LastUpdated(0),
     m_Workload(-1.0)
{
}


// ###### Destructor ########################################################
RDConfigNode::~RDConfigNode()
{
   while (!m_ContextMenuEntries.isEmpty()) {
      delete m_ContextMenuEntries.takeFirst();
   }
}


void RDConfigNode::setUpdated()
{
   m_LastUpdated = CSPListener::getMicroTime();
}


// ###### Get workload value ################################################
double RDConfigNode::getWorkload() const
{
   if((m_Status == ACTIVE) && (m_Workload >= 0.0)) {
      return(m_Workload);
   }
   return(-1.0);
}


// ###### Get workload string ###############################################
const QString RDConfigNode::getWorkloadString() const
{
   if((m_Status == ACTIVE) && (m_Workload >= 0.0)) {
      char str[16];
      snprintf((char*)&str, sizeof(str), "%1.0f%%", 100.0 * m_Workload);
      return(QString(str));
   }
   return(QString(""));
}


// ###### Update status #####################################################
void RDConfigNode::updateStatus()
{
   if((m_LastUpdated + (m_ReportInterval*m_TimeoutMultiplier)) < CSPListener::getMicroTime()) {
      m_Status = INACTIVE;
      m_ConnectedUIDsMap.clear();
      m_StatusText   = "";
      m_LocationText = "";
   }
   else {
      m_Status = ACTIVE;
   }
}
