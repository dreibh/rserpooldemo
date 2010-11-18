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
 *   Copyright (C) 2002-2010 by Thomas Dreibholz
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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Q3PtrList>
#include <QDomElement>

#include "node.h"


class ECriticalException
{};

class ELoadFileException
{};

class EXMLSyntaxException
{
   public:
   EXMLSyntaxException(QString message, int line)
      : m_Message(message), m_Line(line) {}

   QString m_Message;
   int     m_Line;
};


class CNetworkListener;

class CConfiguration
{
   public:
   CConfiguration(QWidget*       canvasWidet,
                  const QString& configFile);
   ~CConfiguration();

   inline int getDisplaySizeX() const {
      return m_DisplaySizeX;
   }
   inline int getDisplaySizeY() const {
      return m_DisplaySizeY;
   }
   inline int getRefreshTime() const {
      return m_RefreshTime;
   }
   inline int getListenPort() const {
      return m_ListenPort;
   }
   inline const QString getCaption() const {
      return m_Caption;
   }
   inline const QString getBackgroundImageName() const {
      return m_BackgroundImageName;
   }
   inline Q3PtrList<CNode>& getNodes() {
      return m_Nodes;
   }
   inline QMap<QString, CNode*>& getNodesMap() {
      return m_NodesMap;
   }
   inline QMap<int, QString>& getColorMap() {
      return m_ColorMap;
   }
   inline void updateDisplaySize(int x, int y)
   {
      m_DisplaySizeX = x;
      m_DisplaySizeY = y;
   }
   void updateNodeData();

   private:
   CNode* createNode(QDomElement element);
   CContextMenuConfig* createContextMenuEntry(const QString&     displayName,
                                              const QDomElement& element);

   QWidget*              m_CanvasWidget;
   int                   m_DisplaySizeX;
   int                   m_DisplaySizeY;
   int                   m_RefreshTime;
   int                   m_ListenPort;
   QString               m_Caption;
   QString               m_BackgroundImageName;
   Q3PtrList<CNode>       m_Nodes;
   QMap<QString, CNode*> m_NodesMap;
   QMap<int, QString>    m_ColorMap;
   CNetworkListener*     m_NetworkListener;
};

#endif
