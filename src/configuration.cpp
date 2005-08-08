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

#include "configuration.h"

#include <qdom.h>
#include <qfile.h>
#include <qmessagebox.h>
#include "networklistener.h"


const char g_ResolutionXTag[]          = "DiagramResolutionX";
const char g_ResolutionYTag[]          = "DiagramResolutionY";
const char g_RefreshTimeTag[]          = "RefreshTime";
const char g_ProtocolColorTag[]        = "ProtocolColor";
const char g_ProtocolColorIDTag[]      = "ID";
const char g_ListenPortTag[]           = "ListenPort";
const char g_RSPNodeTag[]              = "RSPNode";
const char g_UIDTag[]                  = "UID";
const char g_displayNameTag[]          = "DisplayName";
const char g_refreshTimeoutTag[]       = "RefreshTimeout";
const char g_ActiveImageURLTag[]       = "ActiveImageURL";
const char g_InactiveImageURLTag[]     = "InactiveImageURL";
const char g_NotReachableImageURLTag[] = "NotReachableImageURL";
const char g_ContextMenuEntryTag[]     = "ContextMenuEntry";
const char g_NameTag[]                 = "Name";
const char g_CMDTag[]                  = "CMD";
const char g_NodePositionXTag[]        = "PositionX";
const char g_NodePositionYTag[]        = "PositionY";
const char g_BackgroundImageTag[]      = "BackgroundImage";


CConfiguration::CConfiguration(QString _ConfigFile)
   : m_DisplaySizeX(0),
     m_DisplaySizeY(0)
{
   m_RSerPoolNodes.setAutoDelete(true);

   QDomDocument doc("mydocument");
   QFile file(_ConfigFile);
   if(!file.open(IO_ReadOnly)) {
      throw ELoadFileException();
   }

   QString errorMessage;
   int errorLine;
   if(!doc.setContent(&file , true , &errorMessage, &errorLine)) {
      file.close();
      throw EXMLSyntaxException(errorMessage, errorLine);
   }
   file.close();

   QDomNode currentNode = doc.documentElement().firstChild();
   while(!currentNode.isNull()) {
      if(QDomNode::ElementNode == currentNode.nodeType()) {
         if(currentNode.toElement().tagName() == QString(g_RefreshTimeTag)) {
            m_RefreshTime = currentNode.toElement().text().toInt();
         }
         else if(currentNode.toElement().tagName() == QString(g_ListenPortTag)) {
            m_ListenPort = currentNode.toElement().text().toInt();
         }
         else if(currentNode.toElement().tagName() == QString(g_ResolutionXTag)) {
            m_DisplaySizeX = currentNode.toElement().text().toInt();
         }
         else if(currentNode.toElement().tagName() == QString(g_ResolutionYTag)) {
            m_DisplaySizeY = currentNode.toElement().text().toInt();
         }
         else if(currentNode.toElement().tagName() == QString(g_ProtocolColorTag)) {
            QDomAttr Attribute = currentNode.toElement().attributeNode(QString(g_ProtocolColorIDTag));
            if(!Attribute.isNull()) {
               m_ColorMap[Attribute.value().toInt()] = currentNode.toElement().text();
            }
         }
         else if(currentNode.toElement().tagName() == QString(g_BackgroundImageTag)) {
            m_BackgroundImageName = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_RSPNodeTag)) {
            CRSerPoolNode* pNewNode = createRSPNode(currentNode.toElement());
            m_RSerPoolNodes.append(pNewNode);
            m_RSerPoolNodesMap[pNewNode->getUniqueID()] = pNewNode;
         }
         else {
            QMessageBox::critical( 0, "Error!", "Found unknown tag in config file: " + currentNode.toElement().tagName());
         }
      }
      currentNode = currentNode.nextSibling();
   }

   m_NetworkListener = new CNetworkListener(m_ListenPort, m_RSerPoolNodesMap);
}


CConfiguration::~CConfiguration()
{
}


CRSerPoolNode *CConfiguration::createRSPNode(QDomElement _RSPElement)
{
   QPtrList<CContextMenuConfig> contextNodes;
   QString                      uniqueID;
   QString                      displayName;
   QString                      imageActive;
   QString                      imageInactive;
   QString                      imageNotReachable;
   int                          refreshTimeout = 0;
   int                          positionX      = 0;
   int                          positionY      = 0;

   QDomNode currentNode = _RSPElement.firstChild();
   while(!currentNode.isNull()) {
      if(QDomNode::ElementNode == currentNode.nodeType()) {
         if(currentNode.toElement().tagName() == QString(g_UIDTag)) {
            uniqueID = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_displayNameTag))
         {
            displayName = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_refreshTimeoutTag))
         {
            refreshTimeout = currentNode.toElement().text().toInt();
         }
         else if(currentNode.toElement().tagName() == QString(g_ActiveImageURLTag))
         {
            imageActive = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_InactiveImageURLTag))
         {
            imageInactive = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_NotReachableImageURLTag))
         {
            imageNotReachable = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_NodePositionXTag))
         {
            positionX = currentNode.toElement().text().toInt();
            if((positionX < 0) || (positionX > 100))
            {
               QMessageBox::critical( 0, "Error!", "Wrong SizeX: " + positionX);
            }
         }
         else if(currentNode.toElement().tagName() == QString(g_NodePositionYTag))
         {
            positionY = currentNode.toElement().text().toInt();
            if((positionY < 0) || (positionY > 100))
            {
               QMessageBox::critical( 0, "Error!", "Wrong SizeY: " + positionY);
            }
         }
         else if(currentNode.toElement().tagName() == QString(g_ContextMenuEntryTag))
         {
            contextNodes.append(createContextMenuEntry(currentNode.toElement()));
         }
         else
         {
            QMessageBox::critical( 0, "Error!", "Found unknown tag in config file: " + currentNode.toElement().tagName());
         }
      }

      currentNode = currentNode.nextSibling();
   }
   CRSerPoolNode *pNewRSerNode = new CRSerPoolNode(uniqueID,displayName,imageActive,imageInactive,imageNotReachable,positionX, positionY, refreshTimeout);
   pNewRSerNode->getContextMenuConfig() = contextNodes;
   return pNewRSerNode;
}


CContextMenuConfig *CConfiguration::createContextMenuEntry(QDomElement _RSPElement)
{
   QString  nodeName;
   QString  commandLine;
   QDomNode currentNode = _RSPElement.firstChild();

   while(!currentNode.isNull()) {
      if(QDomNode::ElementNode == currentNode.nodeType()) {
         if(currentNode.toElement().tagName() == QString(g_CMDTag)) {
            commandLine = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_NameTag)) {
            nodeName = currentNode.toElement().text();
         }
         else {
            QMessageBox::critical( 0, "Error!", "Found unknown tag in config file: " + currentNode.toElement().tagName());
         }
      }
      currentNode = currentNode.nextSibling();
   }

   CContextMenuConfig* pNewNode = new CContextMenuConfig(nodeName, commandLine);
   return pNewNode;
}


void CConfiguration::updateNodeData()
{
   m_NetworkListener->update();

}
