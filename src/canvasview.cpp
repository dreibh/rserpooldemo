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

#include <qmessagebox.h>
#include <qimage.h>

#include "canvasview.h"
#include "canvasnode.h"
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
