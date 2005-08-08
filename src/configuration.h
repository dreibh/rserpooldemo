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
   EXMLSyntaxException(QString _Message, int _Line) : m_Message(_Message), m_Line(_Line) {}

   QString m_Message;
   int    m_Line;
};


class CNetworkListener;

class CConfiguration
{
   public:
   CConfiguration(QString _ConfigFile);
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
   inline QPtrList<CRSerPoolNode>& getRSerPoolNodes() {
      return m_RSerPoolNodes;
   }
   inline QMap<QString, CRSerPoolNode*>& getRSerPoolNodesMap() {
      return m_RSerPoolNodesMap;
   }
   inline QMap<int, QString>& getColorMap() {
      return m_ColorMap;
   }

   inline void updateDisplaySize(int _X, int _Y)
   {
      m_DisplaySizeX = _X;
      m_DisplaySizeY = _Y;
   }
   void updateNodeData();

   private:
   CRSerPoolNode* createRSPNode(QDomElement _RSPElement);
   CContextMenuConfig* createContextMenuEntry(QDomElement _RSPElement);

   int                           m_DisplaySizeX;
   int                           m_DisplaySizeY;
   int                           m_RefreshTime;
   int                           m_ListenPort;
   QString                       m_BackgroundImageName;
   QPtrList<CRSerPoolNode>       m_RSerPoolNodes;
   QMap<QString, CRSerPoolNode*> m_RSerPoolNodesMap;
   QMap<int, QString>            m_ColorMap;
   CNetworkListener*             m_NetworkListener;
};

#endif
