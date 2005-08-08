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

#ifndef NETWORKLISTENER_H
#define NETWORKLISTENER_H

#include <byteswap.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>
#include <qptrlist.h>
#include <qsocketdevice.h>

#include "rsernode.h"


#define CSP_VERSION  0x0101
#define CSPHT_STATUS 0x0001

#define CID_GROUP(id)  ((id >> 56) & (0xffff))
#define CID_OBJECT(id) (id & 0xffffffffffffffLL)

#define CID_GROUP_NAMESERVER  0x0001
#define CID_GROUP_POOLELEMENT 0x0002
#define CID_GROUP_POOLUSER    0x0003

#define CID_COMPOUND(group, object)  (((uint64_t)group << 56) | object)


class CRSerPoolNode;

class CNetworkListener
{
   public:
   struct ComponentAssociationEntry
   {
      uint64_t ReceiverID;
      uint64_t Duration;
      uint16_t Flags;
      uint16_t ProtocolID;
      uint32_t PPID;
   };

   struct ComponentStatusProtocolHeader
   {
      uint16_t                         Type;
      uint16_t                         Version;
      uint32_t                         Length;

      uint64_t                         SenderID;
      uint64_t                         ReportInterval;
      uint64_t                         SenderTimeStamp;
      char                             StatusText[128];
      char                             ComponentAddress[128];

      uint32_t                         Associations;
      struct ComponentAssociationEntry AssociationArray[];
   };


   CNetworkListener(int                            _ListenPort,
                    QMap<QString, CRSerPoolNode*>& _RSerPoolNodesMap);
   ~CNetworkListener();


   static unsigned long long getMicroTime();
   void update();


   private:
   /**
   * Convert 64-bit value to network byte order.
   *
   * @param value Value in host byte order.
   * @return Value in network byte order.
   */
   static inline uint64_t hton64(const uint64_t value)
   {
      return(bswap_64(value));
   }


   /**
   * Convert 64-bit value to host byte order.
   *
   * @param value Value in network byte order.
   * @return Value in host byte order.
   */
   static inline uint64_t ntoh64(const uint64_t value)
   {
      return(bswap_64(value));
   }

   int                            m_ListenPort;
   QMap<QString, CRSerPoolNode*>& m_RSerPoolNodesMap;
   QSocketDevice*                 m_SocketDevice;
};

#endif
