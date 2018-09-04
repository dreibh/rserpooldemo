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

#include <QPixmap>
#include <QResizeEvent>
#include <QList>
#include <QLinkedList>

#include <mainwidget.h>
#include "canvasnode.h"
#include "canvasview.h"
#include "canvas.h"



CMainWidget::CMainWidget(const QString& configFile)
   : QMainWindow(NULL, NULL),
     m_Configuration(this, configFile)
{
   setWindowTitle(m_Configuration.getCaption() + " - " + configFile);

   printf("QMainWindow=%p\n", (void*)dynamic_cast<QMainWindow*>(this));
   printf("CMainWidget=%p\n", (void*)dynamic_cast<CMainWidget*>(this));

   m_Canvas = new CCanvas(this);
   m_Canvas->setSceneRect(0, 0,
                          m_Configuration.getDisplaySizeX(),
                          m_Configuration.getDisplaySizeY());

   m_BackgroundImage = QImage(m_Configuration.getBackgroundImageName());
   QImage tempImage = m_BackgroundImage.scaled(m_Configuration.getDisplaySizeX(),
                                               m_Configuration.getDisplaySizeY()); // in pixels

   m_Canvas->setBackgroundBrush(tempImage);

   m_CanvasView = new CSerPoolCanvasView(m_Canvas, this);
   m_CanvasView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
   m_CanvasView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
   setCentralWidget(m_CanvasView);

   createCanvasObjects();
//    m_Canvas->setAdvancePeriod(m_Configuration.getRefreshTime());
   puts("FIXME: setAdvancePeriod()!!!");
   m_CanvasView->show();
}


CMainWidget::~CMainWidget()
{
}


void CMainWidget::createCanvasObjects()
{
   QLinkedList<CNode*>& rNodeList = m_Configuration.getNodes();
//    for (CNode* pNode = rNodeList.first();pNode;pNode = rNodeList.next()) {
puts("createCanvasObjects() !!!");
int n=0;

   for(QLinkedList<CNode*>::iterator it = rNodeList.begin();it != rNodeList.end();++it) {
      CNode* pNode = *it;
      CCanvasNode* pSprite = new CCanvasNode(m_Canvas, pNode,
                                             QPixmap(pNode->getImageInactive()),
                                             QPixmap(pNode->getImageActive()));
//       pSprite->setAnimated(true);
      printf("%d: FIXME: setAnimated(true);\n", ++n);
      pSprite->show();
   }
}


void CMainWidget::resizeEvent(QResizeEvent* event)
{
   m_Canvas->setSceneRect(0, 0, event->size().width(), event->size().height());
   m_Canvas->setBackgroundBrush(m_BackgroundImage.scaled(event->size().width(), event->size().height()));
   m_Configuration.updateDisplaySize(event->size().width(), event->size().height());

   QMap<QString, CCanvasNode*>& rNodeMap = m_Canvas->getCanvasNodesMap();
   for(QMap<QString, CCanvasNode*>::iterator it = rNodeMap.begin();it != rNodeMap.end();++it) {
      (*it)->updatePostion();
   }
}
