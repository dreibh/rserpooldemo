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
 *   Copyright (C) 2002-2020 by Thomas Dreibholz
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

#ifndef NODE_H
#define NODE_H

#include <list>

#include <QString>
#include <QMap>

#include <netinet/in.h>

#include "rdcontextmenuconfig.h"


class RDConfigNode
{
   public:
   RDConfigNode(const QString& uniqueID,
                const QString& displayName,
                const QString& imageActive,
                const QString& imageInactive,
                int            positionX,
                int            positionY,
                int            timeoutMultiplier);
   ~RDConfigNode();

   enum NodeStatus
   {
      INACTIVE = 0,
      ACTIVE   = 1
   };

   inline const QString& getUniqueID() const {
      return m_UniqueID;
   }
   inline const QString& getDisplayName() const {
      return m_DisplayName;
   }
   inline const QString& getImageInactive() const {
      return m_ImageInactive;
   }
   inline const QString& getImageActive() const {
      return m_ImageActive;
   }

   inline int getPositionX(const int totalSizeX) const {
      return (m_PositionX * totalSizeX / 100);
   }
   inline int getPositionY(const int totalSizeY) const {
      return (m_PositionY * totalSizeY / 100);
   }

   double getWorkload() const;
   const QString getWorkloadString() const;
   inline void setWorkload(const double workload) {
      m_Workload = workload;
   }

   inline const QString& getStatusText() const {
      return m_StatusText;
   }
   inline void setStatusText(const QString& statusText) {
      m_StatusText = statusText;
   }

   inline const QString& getLocationText() const {
      return m_LocationText;
   }
   inline void setLocationText(const QString& locationText) {
      m_LocationText = locationText;
   }

   inline std::list<RDContextMenuConfig*>& getContextMenuConfig() {
      return m_ContextMenuEntries;
   }
   inline QMap<QString, int>& getConnectedUIDsMap() {
      return m_ConnectedUIDsMap;
   }
   inline QMap<QString, uint64_t>& getConnectedUIDsDurationMap() {
      return m_ConnectionDurationMap;
   }

   inline void setReportInterval(const uint64_t reportInterval) {
      m_ReportInterval = reportInterval;
   }

   inline NodeStatus getStatus() const {
      return m_Status;
   }
   void updateStatus();
   void setUpdated();


   private:
   // Values loaded from config files
   const QString                     m_UniqueID;
   const QString                     m_DisplayName;
   const QString                     m_ImageActive;
   const QString                     m_ImageInactive;
   int                               m_PositionX;
   int                               m_PositionY;
   int                               m_TimeoutMultiplier;

   // Values need during runtime
   std::list<RDContextMenuConfig*>   m_ContextMenuEntries;
   QMap<QString, int>                m_ConnectedUIDsMap;
   QMap<QString, uint64_t>           m_ConnectionDurationMap;
   QString                           m_StatusText;
   QString                           m_LocationText;
   NodeStatus                        m_Status;
   uint64_t                          m_ReportInterval;
   uint64_t                          m_LastUpdated;
   double                            m_Workload;
};


#endif
