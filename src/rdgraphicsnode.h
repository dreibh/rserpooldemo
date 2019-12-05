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
 *   Copyright (C) 2002-2020 by Thomas Dreibholz
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

#ifndef RDGRAPHICSNODE_H
#define RDGRAPHICSNODE_H

#include <QMap>
#include <QLinkedList>
#include <QPixmap>
#include <QMenu>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneContextMenuEvent>

#include "rdconfiguration.h"
#include "rdgraphicsscene.h"
#include "rdconfignode.h"


class RDGraphicsNode : public QGraphicsPixmapItem
{
   public:
   RDGraphicsNode(RDGraphicsScene* canvas,
                  RDConfigNode*    configNode,
                  RDConfiguration* configuration,
                  const QPixmap&   inactivePixmap,
                  const QPixmap&   activePixmap);
   virtual ~RDGraphicsNode();

   virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
   virtual void advance(int phase);

   QColor getColor(const int colorNumber) const;
   void getAnchor(int& rX, int& rY);
   void updatePosition();

   private:
   RDGraphicsScene*         m_GraphicsScene;
   RDConfigNode*            m_ConfigNode;
   RDConfiguration*         m_Configuration;
   QPixmap                  m_InactivePixmap;
   QPixmap                  m_ActivePixmap;
   QGraphicsRectItem*       m_pBackground;
   QGraphicsSimpleTextItem* m_pTitle;
   QGraphicsSimpleTextItem* m_pStatusText;
   QGraphicsSimpleTextItem* m_pLocationText;
   QGraphicsSimpleTextItem* m_pWorkload;
   QGraphicsRectItem*       m_pWorkloadBackground;
   double                   m_ZPosition;

   struct LinkText
   {
      QGraphicsTextItem* m_pDurationText;
      QGraphicsRectItem* m_pBackground;
   };

   QMap<QString, QGraphicsLineItem*> m_ConUIDsLinesMap;
   QMap<QString, LinkText*>          m_ConUIDsTextMap;
};


#endif
