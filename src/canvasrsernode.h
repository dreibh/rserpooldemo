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

#ifndef CANVASRSERNODE_H
#define CANVASRSERNODE_H

#include <qcanvas.h>
#include <qpopupmenu.h>
#include "rserpoolcanvas.h"
#include <qmap.h>


class CRSerPoolNode;


class CCanvasRSerNode : public QCanvasSprite
{
   public:
   CCanvasRSerNode(CRSerPoolCanvas*     _Canvas,
                   CRSerPoolNode*       _RSerNode,
                   QValueList<QPixmap>& _PixmapList);
   virtual ~CCanvasRSerNode();

   virtual void advance(int phase);

   void getAnchor(int& _rX, int& _rY);
   void updatePostion();

   QPopupMenu*       m_ContextMenu;

   private:
   int               m_ZPosition;
   CRSerPoolCanvas*  m_Canvas;
   CRSerPoolNode*    m_RSerNode;

   QCanvasRectangle* m_pBackground;
   QCanvasText*      m_pTitle;
   QCanvasText*      m_pStatusText;
   QCanvasText*      m_pLocationText;
   QCanvasText*      m_pWorkload;
   QCanvasRectangle* m_pWorkloadBackground;

   struct LinkText
   {
      QCanvasText*      m_pDurationText;
      QCanvasRectangle* m_pBackground;
   };

   QMap<QString, QCanvasLine*> m_ConUIDsLinesMap;
   QMap<QString, LinkText*>    m_ConUIDsTextMap;
};


#endif
