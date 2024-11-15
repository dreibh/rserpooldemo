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
 *   Copyright (C) 2002-2025 by Thomas Dreibholz
 *
 *   Authors: Thomas Dreibholz, thomas.dreibholz@gmail.com
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
 * Contact: thomas.dreibholz@gmail.com
 */

#include <QMessageBox>
#include <QString>
#include <iostream>

#include "csplistener.h"


// ###### Constructor #######################################################
CSPListener::CSPListener(int                    listenPort,
                                   QMap<QString, RDConfigNode*>& nodesMap)
   : m_ListenPort(listenPort),
     m_NodesMap(nodesMap),
     m_SocketDevice(0)
{
   m_SocketDevice = new QUdpSocket;
   Q_CHECK_PTR(m_SocketDevice);
   if(m_SocketDevice->bind(QHostAddress(), m_ListenPort, QUdpSocket::ReuseAddressHint) == false) {
      QMessageBox::critical(0, "Error!", "Error binding CSP socket!");
   }
}


// ###### Destructor ########################################################
CSPListener::~CSPListener()
{
   delete m_SocketDevice;
   m_SocketDevice = NULL;
}


// ###### Update configuration from CSP messages ############################
void CSPListener::update()
{
   char buffer[65536];
   while(m_SocketDevice->bytesAvailable()) {
      uint received = m_SocketDevice->readDatagram(buffer, sizeof(buffer));
      if(received >= sizeof(ComponentStatusReport)) {
         ComponentStatusReport* cspReport = (ComponentStatusReport*)&buffer;
         cspReport->Header.Length          = be16toh(cspReport->Header.Length);
         cspReport->Header.Version         = be32toh(cspReport->Header.Version);
         cspReport->Header.SenderID        = be64toh(cspReport->Header.SenderID);
         cspReport->Header.SenderTimeStamp = be64toh(cspReport->Header.SenderTimeStamp);
         if(cspReport->Header.Version != CSP_VERSION) {
            std::cerr << "WARNING: Wrong message version received!" << std::endl;
            continue;
         }
         if(cspReport->Header.Type != CSPT_REPORT) {
            std::cerr << "WARNING: Wrong message type received!" << std::endl;
            continue;
         }
         cspReport->ReportInterval = be32toh(cspReport->ReportInterval);
         cspReport->Associations   = be16toh(cspReport->Associations);
         cspReport->Workload       = be16toh(cspReport->Workload);
         if(received < sizeof(ComponentStatusReport) + cspReport->Associations * sizeof(ComponentAssociation)) {
            std::cerr << "WARNING: Malformed report message received (too short)!" << std::endl;
            continue;
         }

         char nameBuffer[32];
         snprintf(nameBuffer, sizeof(nameBuffer), "%llx",
                  (unsigned long long)cspReport->Header.SenderID);
         const QString name(nameBuffer);
         QMap<QString, RDConfigNode*>::iterator nodeIterator = m_NodesMap.find(name);
         if(nodeIterator != m_NodesMap.end()) {
            RDConfigNode* pNode = *nodeIterator;
            pNode->setUpdated();
            pNode->setReportInterval(cspReport->ReportInterval);

            cspReport->Status[sizeof(cspReport->Status) - 1]     = 0x00;
            cspReport->Location[sizeof(cspReport->Location) - 1] = 0x00;
            pNode->setWorkload(CSR_GET_WORKLOAD(cspReport->Workload));
            pNode->setStatusText(cspReport->Status);
            pNode->setLocationText(cspReport->Location);

            pNode->getConnectedUIDsMap().clear();
            pNode->getConnectedUIDsDurationMap().clear();

            ComponentAssociation* association = (ComponentAssociation*)&cspReport->AssociationArray;
            for(uint i = 0;i < cspReport->Associations;i++) {
               association[i].ReceiverID = be64toh(association[i].ReceiverID);
               association[i].Duration   = be64toh(association[i].Duration);
               association[i].Flags      = be16toh(association[i].Flags);
               association[i].ProtocolID = be16toh(association[i].ProtocolID);
               association[i].PPID       = be32toh(association[i].PPID);

               snprintf(nameBuffer, sizeof(nameBuffer), "%llx",
                        (unsigned long long)association[i].ReceiverID);
               QString peerName(nameBuffer);
               QMap<QString, RDConfigNode*>::iterator nodeIterator;
               if((nodeIterator = m_NodesMap.find(peerName)) != m_NodesMap.end()) {
                  pNode->getConnectedUIDsDurationMap()[peerName] = association[i].Duration;
                  pNode->getConnectedUIDsMap()[peerName]         = association[i].PPID;
               }
#if 0
               else {
                  std::cerr << "WARNING: Received status for unknown connection from "
                            << name.toLocal8Bit().constData() << " to "
                            << peerName.toLocal8Bit().constData() << std::endl;
               }
#endif
            }
         }
         else {
            std::cerr << "WARNING: Received status for unknown ID " << name.toLocal8Bit().constData() << std::endl;
         }
      }
   }
}


// ###### Get system time in microseconds ###################################
unsigned long long CSPListener::getMicroTime()
{
   struct timeval tv;
   gettimeofday(&tv,NULL);
   return(((unsigned long long)tv.tv_sec * (unsigned long long)1000000) +
             (unsigned long long)tv.tv_usec);
}
