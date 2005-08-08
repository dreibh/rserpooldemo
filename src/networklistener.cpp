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

#include <iostream>
#include <qmessagebox.h>
#include <qstring.h>

#include "networklistener.h"


CNetworkListener::CNetworkListener(int                            _ListenPort,
                                   QMap<QString, CRSerPoolNode*>& _RSerPoolNodesMap)
   : m_ListenPort(_ListenPort),
     m_RSerPoolNodesMap(_RSerPoolNodesMap),
     m_SocketDevice(0)
{
   m_SocketDevice = new QSocketDevice(QSocketDevice::Datagram);
   m_SocketDevice->setAddressReusable(TRUE);
   if(m_SocketDevice->bind(QHostAddress(), m_ListenPort) == false) {
      QMessageBox::critical( 0, "Error!", "Error binding Address!");
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
   char buffer[2048];
   while(m_SocketDevice->bytesAvailable()) {
      uint received = m_SocketDevice->readBlock(buffer, sizeof(buffer));
      if(received >= sizeof(ComponentStatusProtocolHeader)) {
         ComponentStatusProtocolHeader* pHeader = reinterpret_cast<struct ComponentStatusProtocolHeader *>(buffer);
         ComponentStatusProtocolHeader  csph;
         csph.Type = ntohs(pHeader->Type);
         if(csph.Type != CSPHT_STATUS) {
            std::cerr << "WARNING: Wrong status message type received!" << std::endl;
            continue;
         }
         csph.Version = ntohs(pHeader->Version);
         if(csph.Version != CSP_VERSION) {
            std::cerr << "WARNING: Status message of wrong CSP protocol version received!" << std::endl;
            continue;
         }
         csph.Length   = ntohl(pHeader->Length);
         csph.SenderID = ntoh64(pHeader->SenderID);

         char nameBuffer[32];
         snprintf(nameBuffer, sizeof(nameBuffer), "%Lx", csph.SenderID);
         QString name(nameBuffer);
         QMap<QString, CRSerPoolNode*>::iterator nodeIterator;
         if((nodeIterator = m_RSerPoolNodesMap.find(name)) != m_RSerPoolNodesMap.end())
         {
            CRSerPoolNode *pNode = nodeIterator.data();
            pNode->setUpdated();

            csph.SenderTimeStamp = ntoh64(pHeader->SenderTimeStamp);
            csph.ReportInterval  = ntoh64(pHeader->ReportInterval);
            pNode->setReportInterval(csph.ReportInterval);

            strncpy((char*)&csph.StatusText, pHeader->StatusText, sizeof(csph.StatusText));
            QString StatusText(csph.StatusText);
            pNode->setStatusText(StatusText);

            strncpy((char*)&csph.ComponentAddress, pHeader->ComponentAddress, sizeof(csph.ComponentAddress));
            QString ComponentAddress(csph.ComponentAddress);
            pNode->setIPText(ComponentAddress);

            pNode->getConnectedUIDsMap().clear();
            pNode->getConnectedUIDsDurationMap().clear();

            csph.Associations = ntohl(pHeader->Associations);

            for(uint i = 0;i < csph.Associations;i++) {
               ComponentAssociationEntry assocEntry;
               assocEntry.ReceiverID = ntoh64(pHeader->AssociationArray[i].ReceiverID);
               assocEntry.Duration   = ntoh64(pHeader->AssociationArray[i].Duration);
               assocEntry.Flags      = ntohs(pHeader->AssociationArray[i].Flags);
               assocEntry.ProtocolID = ntohs(pHeader->AssociationArray[i].ProtocolID);
               assocEntry.PPID       = ntohl(pHeader->AssociationArray[i].PPID);

               snprintf(nameBuffer, sizeof(nameBuffer), "%Lx", assocEntry.ReceiverID);
               QString peerName(nameBuffer);
               QMap<QString, CRSerPoolNode*>::iterator nodeIterator;
               if((nodeIterator = m_RSerPoolNodesMap.find(peerName)) != m_RSerPoolNodesMap.end()) {
                  pNode->getConnectedUIDsDurationMap()[peerName] = assocEntry.Duration;
                  pNode->getConnectedUIDsMap()[peerName]         = assocEntry.PPID;
               }
               else {
                  std::cerr << "WARNING: Received status about unknwon connection from " << name << " to " << peerName << std::endl;
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
