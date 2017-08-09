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

#include <QFont>
#include <QMessageBox>
#include <QPixmap>
#include <QResizeEvent>
#include <Q3ValueList>
#include <Q3PtrList>

#include <mainwidget.h>
#include "canvasnode.h"
#include "canvasview.h"
#include "canvas.h"



CMainWidget::CMainWidget(const QString& configFile)
   : Q3MainWindow(NULL, NULL, Qt::WDestructiveClose),
     m_Configuration(this, configFile)
{
   setCaption(m_Configuration.getCaption() + " - " + configFile);
   m_Canvas = new CCanvas(this);
   m_Canvas->resize(m_Configuration.getDisplaySizeX(),
                    m_Configuration.getDisplaySizeY());
   m_BackgroundImage = QPixmap(m_Configuration.getBackgroundImageName());
   QImage tempImage = m_BackgroundImage;
   tempImage.smoothScale(m_Configuration.getDisplaySizeX(),
                         m_Configuration.getDisplaySizeY()); // in pixels

   m_Canvas->setBackgroundPixmap(QPixmap(tempImage));
   m_CanvasView = new CSerPoolCanvasView(m_Canvas, this);
   m_CanvasView->setVScrollBarMode(Q3ScrollView::AlwaysOff);
   m_CanvasView->setHScrollBarMode(Q3ScrollView::AlwaysOff);
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
   Q3PtrList<CNode>& rNodeList = m_Configuration.getNodes();
   for (CNode* pNode = rNodeList.first();pNode;pNode = rNodeList.next()) {
      Q3ValueList<QPixmap> pixmapList;
      pixmapList.push_back(pNode->getImageInactive());
      pixmapList.push_back(pNode->getImageActive());
      CCanvasNode* pSprite = new CCanvasNode(m_Canvas, pNode, pixmapList);
      pSprite->setAnimated(true);
      pSprite->show();
   }
}


void CMainWidget::resizeEvent(QResizeEvent* event)
{
   m_Canvas->resize(event->size().width(), event->size().height());
   m_Canvas->setBackgroundPixmap(QPixmap(m_BackgroundImage.smoothScale(event->size().width(), event->size().height())));
   m_Configuration.updateDisplaySize(event->size().width(), event->size().height());

   QMap<QString, CCanvasNode*>& rNodeMap = m_Canvas->getCanvasNodesMap();
   for(QMap<QString, CCanvasNode*>::iterator it = rNodeMap.begin();it != rNodeMap.end();++it) {
      it.data()->updatePostion();
   }
}
