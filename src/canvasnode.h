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
 *   Copyright (C) 2002-2019 by Thomas Dreibholz
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
#include <QMenu>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneContextMenuEvent>

#include "configuration.h"
#include "canvas.h"
#include "node.h"


class CCanvasNode : public QGraphicsPixmapItem
{
   public:
   CCanvasNode(CCanvas*        canvas,
               CNode*          node,
               CConfiguration* configuration,
               const QPixmap&  inactivePixmap,
               const QPixmap&  activePixmap);
   virtual ~CCanvasNode();

   virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
   virtual void advance(int phase);

   QColor getColor(const int colorNumber) const;
   void getAnchor(int& rX, int& rY);
   void updatePosition();

   private:
   CCanvas*                 m_Canvas;
   CNode*                   m_Node;
   CConfiguration*          m_Configuration;
   QPixmap                  m_InactivePixmap;
   QPixmap                  m_ActivePixmap;
   double                   m_ZPosition;
   QGraphicsRectItem*       m_pBackground;
   QGraphicsSimpleTextItem* m_pTitle;
   QGraphicsSimpleTextItem* m_pStatusText;
   QGraphicsSimpleTextItem* m_pLocationText;
   QGraphicsSimpleTextItem* m_pWorkload;
   QGraphicsRectItem*       m_pWorkloadBackground;

   struct LinkText
   {
      QGraphicsTextItem* m_pDurationText;
      QGraphicsRectItem* m_pBackground;
   };

   QMap<QString, QGraphicsLineItem*> m_ConUIDsLinesMap;
   QMap<QString, LinkText*>          m_ConUIDsTextMap;
};


#endif
