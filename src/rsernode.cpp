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

#include "rsernode.h"
#include <qmessagebox.h>
#include "networklistener.h"


CRSerPoolNode::CRSerPoolNode(QString _UniqueID,
                             QString _DisplayName,
                             QString _ImageActive,
                             QString _ImageInactive,
                             int _PositionX,
                             int _PositionY,
                             int _TimeoutMultiplier)
   : m_UniqueID(_UniqueID),
     m_DisplayName(_DisplayName),
     m_ImageActive(_ImageActive),
     m_ImageInactive(_ImageInactive),
     m_PositionX(_PositionX),
     m_PositionY(_PositionY),
     m_TimeoutMultiplier(_TimeoutMultiplier),
     m_State(INACTIVE),
     m_ReportInterval(6000000),
     m_LastUpdated(0),
     m_Workload(-1.0)
{
   m_ContextMenuEntries.setAutoDelete(true);
}


void CRSerPoolNode::setUpdated()
{
   m_LastUpdated = CNetworkListener::getMicroTime();
}


const QString CRSerPoolNode::getWorkload() const
{
   if((m_State == ACTIVE) &&
      (m_Workload >= 0.0)) {
      char str[16];
      snprintf((char*)&str, sizeof(str), "%1.0f%%", 100.0 * m_Workload);
      return(QString(str));
   }
   return(QString(""));
}


void CRSerPoolNode::updateStatus()
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
