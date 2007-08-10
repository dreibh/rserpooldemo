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
 * ############- An Efficient RSerPool Prototype Implementation #############
 *
 *   Authors: Thomas Dreibholz, dreibh@exp-math.uni-essen.de               *
 *            Sebastian Rohde, rohde@exp-math.uni-essen.de                 *
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

#ifndef CANVASNODE_H
#define CANVASNODE_H

#include <qcanvas.h>
#include <qpopupmenu.h>
#include <qmap.h>
#include "canvas.h"


class CNode;


class CCanvasNode : public QCanvasSprite
{
   public:
   CCanvasNode(CCanvas*             canvas,
               CNode*               node,
               QValueList<QPixmap>& pixmapList);
   virtual ~CCanvasNode();

   virtual void advance(int phase);

   void getAnchor(int& _rX, int& _rY);
   void updatePostion();

   QPopupMenu*       m_ContextMenu;

   private:
   double            m_ZPosition;
   CCanvas*          m_Canvas;
   CNode*            m_Node;

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
