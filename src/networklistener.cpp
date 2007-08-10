/* $Id$
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
 *   Authors: Thomas Dreibholz, dreibh@exp-math.uni-essen.de
 *            Sebastian Rohde, rohde@exp-math.uni-essen.de
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

#include <iostream>
#include <qmessagebox.h>
#include <qstring.h>

#include "networklistener.h"


CNetworkListener::CNetworkListener(int                    listenPort,
                                   QMap<QString, CNode*>& nodesMap)
   : m_ListenPort(listenPort),
     m_NodesMap(nodesMap),
     m_SocketDevice(0)
{
   m_SocketDevice = new QSocketDevice(QSocketDevice::Datagram);
   m_SocketDevice->setAddressReusable(TRUE);
   if(m_SocketDevice->bind(QHostAddress(), m_ListenPort) == false) {
      QMessageBox::critical(0, "Error!", "Error binding socket!");
   }
   m_SocketDevice->setBlocking(false);
   m_SocketDevice->setReceiveBufferSize (49152);
}


CNetworkListener::~CNetworkListener()
{
   delete m_SocketDevice;
}


void CNetworkListener::update()
{
   char buffer[65536];
   while(m_SocketDevice->bytesAvailable()) {
      uint received = m_SocketDevice->readBlock(buffer, sizeof(buffer));
      if(received >= sizeof(ComponentStatusReport)) {
         ComponentStatusReport* cspReport = (ComponentStatusReport*)&buffer;
         cspReport->Header.Length          = ntohs(cspReport->Header.Length);
         cspReport->Header.Version         = ntohl(cspReport->Header.Version);
         cspReport->Header.SenderID        = ntoh64(cspReport->Header.SenderID);
         cspReport->Header.SenderTimeStamp = ntoh64(cspReport->Header.SenderTimeStamp);
         if(cspReport->Header.Version != CSP_VERSION) {
            std::cerr << "WARNING: Wrong message version received!" << std::endl;
            continue;
         }
         if(cspReport->Header.Type != CSPT_REPORT) {
            std::cerr << "WARNING: Wrong message type received!" << std::endl;
            continue;
         }
         cspReport->ReportInterval = ntohl(cspReport->ReportInterval);
         cspReport->Associations   = ntohs(cspReport->Associations);
         cspReport->Workload       = ntohs(cspReport->Workload);
         if(received < sizeof(ComponentStatusReport) + cspReport->Associations * sizeof(ComponentAssociation)) {
            std::cerr << "WARNING: Malformed report message received (too short)!" << std::endl;
            continue;
         }

         char nameBuffer[32];
         snprintf(nameBuffer, sizeof(nameBuffer), "%llx",
                  (unsigned long long)cspReport->Header.SenderID);
         QString name(nameBuffer);
         QMap<QString, CNode*>::iterator nodeIterator;
         if((nodeIterator = m_NodesMap.find(name)) != m_NodesMap.end()) {
            CNode* pNode = nodeIterator.data();
            pNode->setUpdated();
            pNode->setReportInterval(cspReport->ReportInterval);

            cspReport->Status[sizeof(cspReport->Status) - 1]     = 0x00;
            cspReport->Location[sizeof(cspReport->Location) - 1] = 0x00;
            pNode->setWorkload(CSR_GET_WORKLOAD(cspReport->Workload));
            pNode->setStatusText(cspReport->Status);
            pNode->setLocationText(cspReport->Location);

            pNode->getConnectedUIDsMap().clear();
            pNode->getConnectedUIDsDurationMap().clear();

            for(uint i = 0;i < cspReport->Associations;i++) {
               cspReport->AssociationArray[i].ReceiverID = ntoh64(cspReport->AssociationArray[i].ReceiverID);
               cspReport->AssociationArray[i].Duration   = ntoh64(cspReport->AssociationArray[i].Duration);
               cspReport->AssociationArray[i].Flags      = ntohs(cspReport->AssociationArray[i].Flags);
               cspReport->AssociationArray[i].ProtocolID = ntohs(cspReport->AssociationArray[i].ProtocolID);
               cspReport->AssociationArray[i].PPID       = ntohl(cspReport->AssociationArray[i].PPID);

               snprintf(nameBuffer, sizeof(nameBuffer), "%llx",
                        (unsigned long long)cspReport->AssociationArray[i].ReceiverID);
               QString peerName(nameBuffer);
               QMap<QString, CNode*>::iterator nodeIterator;
               if((nodeIterator = m_NodesMap.find(peerName)) != m_NodesMap.end()) {
                  pNode->getConnectedUIDsDurationMap()[peerName] = cspReport->AssociationArray[i].Duration;
                  pNode->getConnectedUIDsMap()[peerName]         = cspReport->AssociationArray[i].PPID;
               }
               else {
                  std::cerr << "WARNING: Received status for unknwon connection from " << name << " to " << peerName << std::endl;
               }
            }
         }
         else {
            std::cerr << "WARNING: Received status for unknwon ID " << name << std::endl;
         }
      }
   }
}


unsigned long long CNetworkListener::getMicroTime()
{
   struct timeval tv;
   gettimeofday(&tv,NULL);
   return(((unsigned long long)tv.tv_sec * (unsigned long long)1000000) +
             (unsigned long long)tv.tv_usec);
}
