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
 *   Copyright (C) 2002-2017 by Thomas Dreibholz
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

#ifndef CANVASNODE_H
#define CANVASNODE_H

#include <QMap>
#include <QLinkedList>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QMenu>

#include "canvas.h"


class CNode;


class CCanvasNode : public QGraphicsPixmapItem
{
   public:
   CCanvasNode(CCanvas*        canvas,
               CNode*          node,
               QList<QPixmap>& pixmapList);
   virtual ~CCanvasNode();

   virtual void advance(int phase);

   void getAnchor(int& _rX, int& _rY);
   void updatePostion();

   QMenu*                   m_ContextMenu;

   private:
   double                   m_ZPosition;
   CCanvas*                 m_Canvas;
   CNode*                   m_Node;

   QGraphicsRectItem*       m_pBackground;
   QGraphicsSimpleTextItem* m_pTitle;
   QGraphicsSimpleTextItem* m_pStatusText;
   QGraphicsSimpleTextItem* m_pLocationText;
   QGraphicsSimpleTextItem* m_pWorkload;
   QGraphicsRectItem*       m_pWorkloadBackground;

   struct LinkText
   {
      QGraphicsSimpleTextItem* m_pDurationText;
      QGraphicsRectItem*       m_pBackground;
   };

   QMap<QString, QGraphicsLineItem*> m_ConUIDsLinesMap;
   QMap<QString, LinkText*>          m_ConUIDsTextMap;
};


#endif
