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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <qptrlist.h>
#include <qdom.h>
#include "rsernode.h"


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
   CConfiguration(const QString& configFile);
   ~CConfiguration();

   inline int getDisplaySizeX() {
      return m_DisplaySizeX;
   }
   inline int getDisplaySizeY() {
      return m_DisplaySizeY;
   }
   inline int getRefreshTime() {
      return m_RefreshTime;
   }
   inline int getListenPort() {
      return m_ListenPort;
   }
   inline const QString getBackgroundImageName() {
      return m_BackgroundImageName;
   }
   inline QPtrList<CNode>& getNodes() {
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

   int                   m_DisplaySizeX;
   int                   m_DisplaySizeY;
   int                   m_RefreshTime;
   int                   m_ListenPort;
   QString               m_BackgroundImageName;
   QPtrList<CNode>       m_Nodes;
   QMap<QString, CNode*> m_NodesMap;
   QMap<int, QString>    m_ColorMap;
   CNetworkListener*     m_NetworkListener;
};

#endif
