/*
 * ##########################################################################
 *
 *              //===//   //=====   //===//   //       //   //===//
 *             //    //  //        //    //  //       //   //    //
 *            //===//   //=====   //===//   //       //   //===<<
 *           //   \\         //  //        //       //   //    //
 *          //     \\  =====//  //        //=====  //   //===//    Version II
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

#ifndef NETWORKLISTENER_H
#define NETWORKLISTENER_H

#include <QUdpSocket>

#include <byteswap.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>

#include "rdconfignode.h"
#include "componentstatuspackets.h"


class RDConfigNode;

class CNetworkListener
{
   public:
   CNetworkListener(int                    listenPort,
                    QMap<QString, RDConfigNode*>& nodesMap);
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

   int                    m_ListenPort;
   QMap<QString, RDConfigNode*>& m_NodesMap;
   QUdpSocket*            m_SocketDevice;
};

#endif
