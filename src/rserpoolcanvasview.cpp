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

#include <qmessagebox.h>
#include <qimage.h>

#include "rserpoolcanvasview.h"
#include "canvasrsernode.h"
#include "mainwidget.h"


CSerPoolCanvasView::CSerPoolCanvasView(QCanvas *canvas, QWidget* parent = 0)
   : QCanvasView(canvas, parent)
{
}


CSerPoolCanvasView::~CSerPoolCanvasView()
{
}


void CSerPoolCanvasView::contentsContextMenuEvent(QContextMenuEvent* event)
{
   QCanvasItemList list  = canvas()->collisions(event->pos());
   CCanvasNode*    node = 0;
   for(QCanvasItemList::iterator it = list.begin();it != list.end();++it) {
      node = dynamic_cast<CCanvasNode *>(*it) ;
      if(node) {
         node->m_ContextMenu->exec(event->globalPos());
         return;
      }
   }
}
