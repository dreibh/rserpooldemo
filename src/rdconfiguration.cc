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

#include <QDomElement>
#include <QFile>
#include <QMessageBox>

#include "rdconfiguration.h"
#include "csplistener.h"


const char g_ResolutionXTag[]          = "DiagramResolutionX";
const char g_ResolutionYTag[]          = "DiagramResolutionY";
const char g_RefreshTimeTag[]          = "RefreshTime";
const char g_ProtocolColorTag[]        = "ProtocolColor";
const char g_ProtocolColorIDTag[]      = "ID";
const char g_ListenPortTag[]           = "ListenPort";
const char g_NodeTag[]                 = "Node";
const char g_UIDTag[]                  = "UID";
const char g_DisplayNameTag[]          = "DisplayName";
const char g_DisplayLocationTag[]      = "DisplayLocation";
const char g_RefreshTimeoutTag[]       = "RefreshTimeout";
const char g_ActiveImageURLTag[]       = "ActiveImageURL";
const char g_InactiveImageURLTag[]     = "InactiveImageURL";
const char g_ContextMenuEntryTag[]     = "ContextMenuEntry";
const char g_ContextMenuSeparatorTag[] = "ContextMenuSeparator";
const char g_NameTag[]                 = "Name";
const char g_CMDTag[]                  = "Command";
const char g_NodePositionXTag[]        = "PositionX";
const char g_NodePositionYTag[]        = "PositionY";
const char g_BackgroundImageTag[]      = "BackgroundImage";
const char g_CaptionTag[]              = "Caption";


// ###### Constructor #######################################################
RDConfiguration::RDConfiguration(QWidget*       canvasWidget,
                                 const QString& configFile)
   : m_GraphicsSceneWidget(canvasWidget),
     m_DisplaySizeX(0),
     m_DisplaySizeY(0)
{
   QDomDocument doc("ScenarioSetup");
   QFile file(configFile);
   if(!file.open(QIODevice::ReadOnly)) {
      throw ELoadFileException();
   }

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
   QDomDocument::ParseResult result = doc.setContent(&file);
   if(!result.errorMessage.isEmpty()) {
      file.close();
      throw EXMLSyntaxException(result.errorMessage, result.errorLine);
   }
#else
   QString errorMessage;
   int errorLine;
   if(!doc.setContent(&file , true , &errorMessage, &errorLine)) {
      file.close();
      throw EXMLSyntaxException(errorMessage, errorLine);
   }
#endif
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
         else if(currentNode.toElement().tagName() == QString(g_CaptionTag)) {
            m_Caption = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_NodeTag)) {
            RDConfigNode* node = createNode(currentNode.toElement());
            m_ConfigNodes.push_back(node);
            m_ConfigNodesMap[node->getUniqueID()] = node;
         }
         else {
            QMessageBox::critical(0, "Error!", "Found unknown tag in config file: " +
                                  currentNode.toElement().tagName());
         }
      }
      currentNode = currentNode.nextSibling();
   }

   m_NetworkListener = new CSPListener(m_ListenPort, m_ConfigNodesMap);
}


// ###### Destructor ########################################################
RDConfiguration::~RDConfiguration()
{
   while (!m_ConfigNodes.empty()) {
      delete m_ConfigNodes.front();
      m_ConfigNodes.pop_front();
   }
}


// ###### Create config node ################################################
RDConfigNode* RDConfiguration::createNode(QDomElement element)
{
   std::list<RDContextMenuConfig*> contextNodes;
   QString                         uniqueID;
   QString                         displayName;
   QString                         displayLocation;
   QString                         imageActive;
   QString                         imageInactive;
   int                             refreshTimeout = 0;
   int                             positionX      = 0;
   int                             positionY      = 0;

   QDomNode currentNode = element.firstChild();
   while(!currentNode.isNull()) {
      if(QDomNode::ElementNode == currentNode.nodeType()) {
         if(currentNode.toElement().tagName() == QString(g_UIDTag)) {
            uniqueID = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_DisplayNameTag)) {
            displayName = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_DisplayLocationTag)) {
            displayLocation = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_RefreshTimeoutTag)) {
            refreshTimeout = currentNode.toElement().text().toInt();
         }
         else if(currentNode.toElement().tagName() == QString(g_ActiveImageURLTag)) {
            imageActive = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_InactiveImageURLTag)) {
            imageInactive = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_NodePositionXTag)) {
            positionX = currentNode.toElement().text().toInt();
            if((positionX < 0) || (positionX > 100)) {
               QMessageBox::critical(0, "Error!", "Wrong SizeX: " + QString::number(positionX));
            }
         }
         else if(currentNode.toElement().tagName() == QString(g_NodePositionYTag)) {
            positionY = currentNode.toElement().text().toInt();
            if((positionY < 0) || (positionY > 100)) {
               QMessageBox::critical(0, "Error!", "Wrong SizeY: " + QString::number(positionY));
            }
         }
         else if(currentNode.toElement().tagName() == QString(g_ContextMenuEntryTag)) {
            contextNodes.push_back(createContextMenuEntry(displayName, currentNode.toElement()));
         }
         else if(currentNode.toElement().tagName() == QString(g_ContextMenuSeparatorTag)) {
            contextNodes.push_back(new RDContextMenuConfig(m_GraphicsSceneWidget, "", "", ""));
         }
         else {
            QMessageBox::critical(0, "Error!", "Found unknown tag in config file: " +
                                  currentNode.toElement().tagName());
         }
      }

      currentNode = currentNode.nextSibling();
   }
   RDConfigNode* node = new RDConfigNode(uniqueID, displayName, displayLocation,
                                         imageActive, imageInactive,
                                         positionX, positionY, refreshTimeout);
   node->getContextMenuConfig() = contextNodes;
   return node;
}


// ###### Create entry for context menu #####################################
RDContextMenuConfig* RDConfiguration::createContextMenuEntry(const QString&     nodeName,
                                                             const QDomElement& element)
{
   QString  itemName;
   QString  commandLine;
   QDomNode currentNode = element.firstChild();

   while(!currentNode.isNull()) {
      if(QDomNode::ElementNode == currentNode.nodeType()) {
         if(currentNode.toElement().tagName() == QString(g_CMDTag)) {
            commandLine = currentNode.toElement().text();
         }
         else if(currentNode.toElement().tagName() == QString(g_NameTag)) {
            itemName = currentNode.toElement().text();
         }
         else {
            QMessageBox::critical(0, "Error!", "Found unknown tag in config file: " +
                                  currentNode.toElement().tagName());
         }
      }
      currentNode = currentNode.nextSibling();
   }

   RDContextMenuConfig* node = new RDContextMenuConfig(m_GraphicsSceneWidget, nodeName, itemName, commandLine);
   return node;
}


// ###### Update configuration from CSP messages ############################
void RDConfiguration::update()
{
   m_NetworkListener->update();
}
