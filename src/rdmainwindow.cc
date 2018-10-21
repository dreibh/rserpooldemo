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

#include <rdmainwindow.h>
#include "rdgraphicsnode.h"
#include "rdgraphicsview.h"
#include "rdgraphicsscene.h"



// ###### Constructor #######################################################
RDMainWindow::RDMainWindow(const QString& configFile)
   : QMainWindow(NULL, NULL),
     m_Configuration(this, configFile)
{
   setWindowTitle(m_Configuration.getCaption() + " - " + configFile);

   m_GraphicsScene = new RDGraphicsScene(this, &m_Configuration);
   Q_CHECK_PTR(m_GraphicsScene);
   m_GraphicsScene->setSceneRect(0, 0,
                          m_Configuration.getDisplaySizeX(),
                          m_Configuration.getDisplaySizeY());

   m_BackgroundImage = QImage(m_Configuration.getBackgroundImageName());
   QImage tempImage = m_BackgroundImage.scaled(m_Configuration.getDisplaySizeX(),
                                               m_Configuration.getDisplaySizeY()); // in pixels

   m_GraphicsScene->setBackgroundBrush(tempImage);

   m_GraphicsView = new RDGraphicsView(m_GraphicsScene, this);
   Q_CHECK_PTR(m_GraphicsView);
   m_GraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
   m_GraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
   setCentralWidget(m_GraphicsView);

   createNodes();
   m_GraphicsScene->setAdvancePeriod(m_Configuration.getRefreshTime());

   m_GraphicsView->show();
}


// ###### Destructor ########################################################
RDMainWindow::~RDMainWindow()
{
}


// ###### Create graphics nodes #############################################
void RDMainWindow::createNodes()
{
   QLinkedList<RDConfigNode*>& rNodeList = m_Configuration.getNodes();
   for(QLinkedList<RDConfigNode*>::iterator it = rNodeList.begin();it != rNodeList.end();++it) {
      RDConfigNode* pNode = *it;
      RDGraphicsNode* node = new RDGraphicsNode(m_GraphicsScene, pNode, &m_Configuration,
                                            QPixmap(pNode->getImageInactive()),
                                            QPixmap(pNode->getImageActive()));
      Q_CHECK_PTR(node);
      node->show();
   }
}


// ###### Resize ############################################################
void RDMainWindow::resizeEvent(QResizeEvent* event)
{
   m_GraphicsScene->setSceneRect(0, 0, event->size().width(), event->size().height());
   m_GraphicsScene->setBackgroundBrush(m_BackgroundImage.scaled(event->size().width(), event->size().height()));
   m_Configuration.updateDisplaySize(event->size().width(), event->size().height());

   QMap<QString, RDGraphicsNode*>& rNodeMap = m_GraphicsScene->getCanvasNodesMap();
   for(QMap<QString, RDGraphicsNode*>::iterator it = rNodeMap.begin();it != rNodeMap.end();++it) {
      (*it)->updatePosition();
   }
}
