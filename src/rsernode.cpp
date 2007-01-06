/* $Id$ */
/***************************************************************************
 *   The RSerPool Demo System                                              *
 *                                                                         *
 *   Authors: Thomas Dreibholz, dreibh@exp-math.uni-essen.de               *
 *            Sebastian Rohde, rohde@exp-math.uni-essen.de                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <qmessagebox.h>

#include "rsernode.h"
#include "networklistener.h"


CNode::CNode(QString uniqueID,
             QString displayName,
             QString imageActive,
             QString imageInactive,
             int     positionX,
             int     positionY,
             int     timeoutMultiplier)
   : m_UniqueID(uniqueID),
     m_DisplayName(displayName),
     m_ImageActive(imageActive),
     m_ImageInactive(imageInactive),
     m_PositionX(positionX),
     m_PositionY(positionY),
     m_TimeoutMultiplier(timeoutMultiplier),
     m_State(INACTIVE),
     m_ReportInterval(6000000),
     m_LastUpdated(0),
     m_Workload(-1.0)
{
   m_ContextMenuEntries.setAutoDelete(true);
}


void CNode::setUpdated()
{
   m_LastUpdated = CNetworkListener::getMicroTime();
}


const double CNode::getWorkload() const
{
   if((m_State == ACTIVE) &&
      (m_Workload >= 0.0)) {
      return(m_Workload);
   }
   return(-1.0);
}


const QString CNode::getWorkloadString() const
{
   if((m_State == ACTIVE) &&
      (m_Workload >= 0.0)) {
      char str[16];
      snprintf((char*)&str, sizeof(str), "%1.0f%%", 100.0 * m_Workload);
      return(QString(str));
   }
   return(QString(""));
}


void CNode::updateStatus()
{
   if((m_LastUpdated + (m_ReportInterval*m_TimeoutMultiplier)) < CNetworkListener::getMicroTime()) {
      m_State = INACTIVE;
      m_ConnectedUIDsMap.clear();
      m_StatusText = "";
      m_LocationText = "";
   }
   else {
      m_State = ACTIVE;
   }
}
