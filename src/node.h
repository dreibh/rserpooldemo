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

#ifndef NODE_H
#define NODE_H

#include <netinet/in.h>
#include <qstring.h>
#include <qptrlist.h>

#include "contextmenuconfig.h"


class CNode
{
   public:
   CNode(QString uniqueID,
         QString displayName,
         QString imageActive,
         QString imageInactive,
         int     positionX,
         int     positionY,
         int     timeoutMultiplier);

   enum NodeStatus
   {
      INACTIVE = 0,
      ACTIVE   = 1
   };

   inline const QString& getImageInactive() const {
      return m_ImageInactive;
   }
   inline const QString& getImageActive() const {
      return m_ImageActive;
   }
   inline const QString& getUniqueID() const {
      return m_UniqueID;
   }
   inline int getPositionX(int _TotalSizeX) const {
      return (m_PositionX * _TotalSizeX / 100);
   }
   inline int getPositionY(int _TotalSizeY) const {
      return (m_PositionY * _TotalSizeY / 100);
   }
   inline const QString& getDisplayName() const {
      return m_DisplayName;
   }
   const double getWorkload() const;
   const QString getWorkloadString() const;
   inline const QString& getStatusText() const {
      return m_StatusText;
   }
   inline const QString& getLocationText() const {
      return m_LocationText;
   }

   inline QPtrList<CContextMenuConfig>& getContextMenuConfig() {
      return m_ContextMenuEntries;
   }
   inline QMap<QString, int>& getConnectedUIDsMap() {
      return m_ConnectedUIDsMap;
   }
   inline QMap<QString, uint64_t>& getConnectedUIDsDurationMap() {
      return m_ConnectionDurationMap;
   }

   inline NodeStatus getStatus() const {
      return m_State;
   }


   inline void setWorkload(const double _rWorkload) {
      m_Workload = _rWorkload;
   }
   inline void setStatusText(const QString &_rStatusText) {
      m_StatusText = _rStatusText;
   }
   inline void setLocationText(const QString &_rLocationText) {
      m_LocationText = _rLocationText;
   }
   inline void setReportInterval(uint64_t _ReportInterval) {
      m_ReportInterval = _ReportInterval;
   }

   void setUpdated();
   void updateStatus();


   private:
   // Values loaded from config files
   QString m_UniqueID;
   QString m_DisplayName;
   QString m_ImageActive;
   QString m_ImageInactive;
   int     m_PositionX;
   int     m_PositionY;
   int     m_TimeoutMultiplier;

   // Values need in RunTime
   QPtrList<CContextMenuConfig> m_ContextMenuEntries;
   QMap<QString, int>           m_ConnectedUIDsMap;
   QMap<QString, uint64_t>      m_ConnectionDurationMap;
   QString                      m_StatusText;
   QString                      m_LocationText;
   NodeStatus                   m_State;
   uint64_t                     m_ReportInterval;
   uint64_t                     m_LastUpdated;
   double                       m_Workload;
};


#endif