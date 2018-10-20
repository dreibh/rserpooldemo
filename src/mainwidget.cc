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

#include <QPixmap>
#include <QResizeEvent>
#include <QList>
#include <QLinkedList>

#include <mainwidget.h>
#include "rdgraphicsnode.h"
#include "canvasview.h"
#include "rdgraphicsscene.h"



CMainWidget::CMainWidget(const QString& configFile)
   : QMainWindow(NULL, NULL),
     m_Configuration(this, configFile)
{
   setWindowTitle(m_Configuration.getCaption() + " - " + configFile);

   m_Canvas = new RDGraphicsScene(this, &m_Configuration);
   Q_CHECK_PTR(m_Canvas);
   m_Canvas->setSceneRect(0, 0,
                          m_Configuration.getDisplaySizeX(),
                          m_Configuration.getDisplaySizeY());

   m_BackgroundImage = QImage(m_Configuration.getBackgroundImageName());
   QImage tempImage = m_BackgroundImage.scaled(m_Configuration.getDisplaySizeX(),
                                               m_Configuration.getDisplaySizeY()); // in pixels

   m_Canvas->setBackgroundBrush(tempImage);

   m_CanvasView = new CSerPoolCanvasView(m_Canvas, this);
   Q_CHECK_PTR(m_CanvasView);
   m_CanvasView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
   m_CanvasView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
   setCentralWidget(m_CanvasView);

   createCanvasObjects();
   m_Canvas->setAdvancePeriod(m_Configuration.getRefreshTime());

   m_CanvasView->show();
}


CMainWidget::~CMainWidget()
{
}


void CMainWidget::createCanvasObjects()
{
   QLinkedList<CNode*>& rNodeList = m_Configuration.getNodes();
   for(QLinkedList<CNode*>::iterator it = rNodeList.begin();it != rNodeList.end();++it) {
      CNode* pNode = *it;
      RDGraphicsNode* node = new RDGraphicsNode(m_Canvas, pNode, &m_Configuration,
                                            QPixmap(pNode->getImageInactive()),
                                            QPixmap(pNode->getImageActive()));
      Q_CHECK_PTR(node);
      node->show();
   }
}


void CMainWidget::resizeEvent(QResizeEvent* event)
{
   m_Canvas->setSceneRect(0, 0, event->size().width(), event->size().height());
   m_Canvas->setBackgroundBrush(m_BackgroundImage.scaled(event->size().width(), event->size().height()));
   m_Configuration.updateDisplaySize(event->size().width(), event->size().height());

   QMap<QString, RDGraphicsNode*>& rNodeMap = m_Canvas->getCanvasNodesMap();
   for(QMap<QString, RDGraphicsNode*>::iterator it = rNodeMap.begin();it != rNodeMap.end();++it) {
      (*it)->updatePosition();
   }
}
