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

#ifndef NETWORKLISTENER_H
#define NETWORKLISTENER_H

#include <byteswap.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>
#include <qptrlist.h>
#include <qsocketdevice.h>

#include "rsernode.h"
#include "componentstatuspackets.h"


class CRSerPoolNode;

class CNetworkListener
{
   public:
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
#if BYTE_ORDER == LITTLE_ENDIAN
      return(bswap_64(value));
#elif BYTE_ORDER == BIG_ENDIAN
      return(value);
#else
#error Byte order is not defined!
#endif
   }

   /**
   * Convert 64-bit value to host byte order.
   *
   * @param value Value in network byte order.
   * @return Value in host byte order.
   */
   static inline uint64_t ntoh64(const uint64_t value)
   {
#if BYTE_ORDER == LITTLE_ENDIAN
      return(bswap_64(value));
#elif BYTE_ORDER == BIG_ENDIAN
      return(value);
#else
#error Byte order is not defined!
#endif
   }

   int                            m_ListenPort;
   QMap<QString, CRSerPoolNode*>& m_RSerPoolNodesMap;
   QSocketDevice*                 m_SocketDevice;
};

#endif
