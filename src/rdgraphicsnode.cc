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
 *   Copyright (C) 2002-2021 by Thomas Dreibholz
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

#include <QObject>
#include <QAction>
#include <QStringList>
#include <QDateTime>
#include <QPixmap>
#include <QMenu>
#include <QGraphicsView>

#include "rdgraphicsnode.h"
#include "rdconfignode.h"
#include "rdmainwindow.h"


#define MARGIN_WORKLOAD   4
#define MARGIN_DURATION   4
#define MARGIN_BACKGROUND 8


// ###### Constructor #######################################################
RDGraphicsNode::RDGraphicsNode(RDGraphicsScene* canvas,
                               RDConfigNode*    node,
                               RDConfiguration* configuration,
                               const QPixmap&   inactivePixmap,
                               const QPixmap&   activePixmap)
 : QGraphicsPixmapItem(inactivePixmap),
   m_GraphicsScene(canvas),
   m_ConfigNode(node),
   m_Configuration(configuration),
   m_InactivePixmap(inactivePixmap),
   m_ActivePixmap(activePixmap)
 {
   // ====== Create node icon ===============================================
   const int displaySizeX = m_Configuration->getDisplaySizeX();
   const int displaySizeY = m_Configuration->getDisplaySizeY();
   m_ZPosition = 1000000000 + ((((m_ConfigNode->getPositionX(displaySizeX) % 1024) << 10) +
                                 (m_ConfigNode->getPositionY(displaySizeY) % 1024)) << 4);
   setZValue(m_ZPosition + 10);


   // ====== Create node title ==============================================
   m_pTitle = new QGraphicsSimpleTextItem(m_ConfigNode->getDisplayName(), this);
   Q_CHECK_PTR(m_pTitle);
   m_pTitle->setFont(QFont("Helvetica", 12, QFont::Bold));
   m_pTitle->setZValue(m_ZPosition + 6);
   m_GraphicsScene->addItem(m_pTitle);

   m_pBackground = new QGraphicsRectItem(this);
   Q_CHECK_PTR(m_pBackground);
   m_pBackground->setZValue(m_ZPosition + 5);
   m_pBackground->setBrush(QBrush(QColor("#FFD700")));
   m_pBackground->setPen(QPen(QColor("#7C7777")));
   m_GraphicsScene->addItem(m_pBackground);


   // ====== Create workload label ==========================================
   m_pWorkload = new QGraphicsSimpleTextItem("", this);
   Q_CHECK_PTR(m_pWorkload);
   m_pWorkload->setFont(QFont("Helvetica", 12, QFont::Bold));
   m_pWorkload->setPen(QColor("#222222"));
   m_pWorkload->setZValue(2000000001);
   m_GraphicsScene->addItem(m_pWorkload);

   QFontMetrics workloadFM(m_pWorkload->font());
   m_pWorkloadBackground = new QGraphicsRectItem(0, 0,
                                                 MARGIN_WORKLOAD + workloadFM.boundingRect("100%").width(),
                                                 MARGIN_WORKLOAD + workloadFM.boundingRect("100%").height(),
                                                 this);
   Q_CHECK_PTR(m_pWorkloadBackground);
   m_pWorkloadBackground->setZValue(2000000000);
   m_pWorkloadBackground->setBrush(QBrush(QColor("#D7D7FF")));
   m_pWorkloadBackground->setPen(QPen(QColor("#202020")));
   m_pWorkloadBackground->hide();   // It is hidden until there is workload to report!
   m_GraphicsScene->addItem(m_pWorkloadBackground);


   // ====== Create status label ============================================
   m_pStatusText = new QGraphicsSimpleTextItem(m_ConfigNode->getStatusText(), this);
   Q_CHECK_PTR(m_pStatusText);
   m_pStatusText->setFont(QFont("Helvetica", 10, QFont::Bold ));
   m_pStatusText->setZValue(m_ZPosition + 6);
   m_GraphicsScene->addItem(m_pStatusText);


   // ====== Create location label ==========================================
   m_pLocationText = new QGraphicsSimpleTextItem(m_ConfigNode->getLocationText(), this);
   Q_CHECK_PTR(m_pLocationText);
   m_pLocationText->setFont(QFont("Helvetica", 6));
   m_pLocationText->setZValue(m_ZPosition + 6);
   m_GraphicsScene->addItem(m_pLocationText);

   // ====== Create ID to nodes map =========================================
   const QString uid = m_ConfigNode->getUniqueID();
   (m_GraphicsScene->getCanvasNodesMap())[uid] = this;

   m_GraphicsScene->addItem(this);
   advance(1);
   updatePosition();
}


// ###### Destructor ########################################################
RDGraphicsNode::~RDGraphicsNode()
{
}


// ###### Create context menus ##############################################
void RDGraphicsNode::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
   QMenu contextMenu;
   std::list<RDContextMenuConfig*>& rNodeList = m_ConfigNode->getContextMenuConfig();
   for(std::list<RDContextMenuConfig*>::iterator iterator = rNodeList.begin();
       iterator != rNodeList.end(); ++iterator) {
      RDContextMenuConfig* pNode = *iterator;
      if(pNode->getName() != "") {
         QAction* action = contextMenu.addAction(pNode->getName());
         Q_CHECK_PTR(action);
         QObject::connect(action, SIGNAL(triggered()), pNode, SLOT(execute()));
      }
      else {
         contextMenu.addSeparator();
      }
   }
   contextMenu.exec(event->screenPos());
}


// ###### Obtain anchor coordinates #########################################
void RDGraphicsNode::getAnchor(int& rX, int& rY)
{
   const int displaySizeX = m_Configuration->getDisplaySizeX();
   const int displaySizeY = m_Configuration->getDisplaySizeY();

   rX = m_ConfigNode->getPositionX(displaySizeX) + (boundingRect().width() / 2);
   rY = m_ConfigNode->getPositionY(displaySizeY) + (boundingRect().height() / 2);
}


// ###### Obtain colour #####################################################
QColor RDGraphicsNode::getColor(const int colorNumber) const
{
    QMap<int, QString>::iterator colorNameIterator = m_Configuration->getColorMap().find(colorNumber);
    QColor                       color;
    if(colorNameIterator != m_Configuration->getColorMap().end()) {
        color = QColor(*colorNameIterator);
    }
    else {
        color = QColor("black");
    }
    return color;
}


// ###### Update position ###################################################
void RDGraphicsNode::updatePosition()
{
   // ====== Update node icon ===============================================
   const int displaySizeX = m_Configuration->getDisplaySizeX();
   const int displaySizeY = m_Configuration->getDisplaySizeY();
   setPos(m_ConfigNode->getPositionX(displaySizeX),
          m_ConfigNode->getPositionY(displaySizeY));


   // ====== Update node title ==============================================
   const int tx = m_ConfigNode->getPositionX(displaySizeX) +
                     ((boundingRect().width() / 2) -
                      ((m_pTitle->boundingRect().right() - m_pTitle->boundingRect().left()) / 2));
   const int ty = m_ConfigNode->getPositionY(displaySizeY) + boundingRect().height();
   m_pTitle->setPos(tx, ty);


   // ====== Update workload label ==========================================
   const int wx = m_ConfigNode->getPositionX(displaySizeX) + (boundingRect().width() / 2);
   const int wy = m_ConfigNode->getPositionY(displaySizeY);
   m_pWorkloadBackground->setPos(wx, wy);

   m_pWorkload->setPos(m_pWorkloadBackground->x() + (MARGIN_WORKLOAD / 2) + ((m_pWorkloadBackground->boundingRect().right() - m_pWorkloadBackground->boundingRect().left()) / 2),
                       m_pWorkloadBackground->y() - (MARGIN_WORKLOAD / 2) - ((m_pWorkloadBackground->boundingRect().top() - m_pWorkloadBackground->boundingRect().bottom()) / 2));


   // ====== Update status label ============================================
   const int sx = m_ConfigNode->getPositionX(displaySizeX) +
                       ((boundingRect().width() / 2) -
                        ((m_pStatusText->boundingRect().right() - m_pStatusText->boundingRect().left()) / 2));
   const int sy = m_ConfigNode->getPositionY(displaySizeY) +  m_pTitle->boundingRect().bottom() - m_pTitle->boundingRect().top() + boundingRect().height();
   m_pStatusText->setPos(sx, sy);


   // ====== Update location label ==========================================
   const int lx = m_ConfigNode->getPositionX(displaySizeX) +
                       ((boundingRect().width() / 2) -
                        ((m_pLocationText->boundingRect().right() - m_pLocationText->boundingRect().left()) / 2));
   const int ly = m_ConfigNode->getPositionY(displaySizeY) +  m_pTitle->boundingRect().height() + m_pStatusText->boundingRect().height() + boundingRect().height();
   m_pLocationText->setPos(lx, ly);


   // ====== Update background ==============================================
   const int titleWidth        = m_pTitle->boundingRect().width();
   const int statusTextWidth   = m_pStatusText->boundingRect().width();
   const int locationTextWidth = m_pLocationText->boundingRect().width();

   const int boundingWidth = MARGIN_BACKGROUND + std::max(std::max(titleWidth, statusTextWidth), locationTextWidth);
   int boundingHeight;
   if( (m_pStatusText->text() == "") && (m_pLocationText->text() == "") ) {
      boundingHeight = m_pTitle->boundingRect().height();
   }
   else if(m_pLocationText->text() == "") {
      boundingHeight = m_pTitle->boundingRect().height() + m_pStatusText->boundingRect().height();
   }
   else {
      boundingHeight = m_pTitle->boundingRect().height() + m_pStatusText->boundingRect().height() + m_pLocationText->boundingRect().height();
   }
   boundingHeight += MARGIN_BACKGROUND;

   const int bx = m_ConfigNode->getPositionX(displaySizeX) + (boundingRect().width() / 2) - (boundingWidth/ 2);
   const int by = m_ConfigNode->getPositionY(displaySizeY) + boundingRect().height() - (MARGIN_BACKGROUND / 2);
   m_pBackground->setRect(bx, by, boundingWidth, boundingHeight);


   // ====== Update links ===================================================
   int thisX;
   int thisY;
   getAnchor(thisX, thisY);
   for(QMap<QString, QGraphicsLineItem*>::iterator iterator = m_ConUIDsLinesMap.begin();
       iterator != m_ConUIDsLinesMap.end(); ++iterator) {
      if(m_GraphicsScene->getCanvasNodesMap().find(iterator.key()) != m_GraphicsScene->getCanvasNodesMap().end()) {
         RDGraphicsNode* pOtherNode = *(m_GraphicsScene->getCanvasNodesMap().find(iterator.key()));

         // ====== Update link ==============================================
         // printf("%s: %s -> %s\n", iterator.key().toStdString().c_str(),
         //                          m_ConfigNode->getDisplayName().toStdString().c_str(),
         //                          pOtherNode->m_ConfigNode->getDisplayName().toStdString().c_str());
         int otherX = 0;
         int otherY = 0;
         pOtherNode->getAnchor(otherX, otherY);
         QGraphicsLineItem* line = m_ConUIDsLinesMap[iterator.key()];
         if(line) {
            line->setLine(thisX, thisY, otherX, otherY);

            // ====== Update link duration ==================================
            const uint64_t duration = m_ConfigNode->getConnectedUIDsDurationMap()[iterator.key()];
            if(duration != ~0ULL) {
               LinkText* linkText = m_ConUIDsTextMap[iterator.key()];
               if( (linkText != NULL) && (linkText->m_pDurationText) && (linkText->m_pBackground) ) {
                  const QFontMetrics fontMetrics(linkText->m_pDurationText->font());
                  const int offsetX = -fontMetrics.boundingRect(linkText->m_pDurationText->toPlainText()).width() / 2;
                  const int offsetY = -fontMetrics.boundingRect(linkText->m_pDurationText->toPlainText()).height() / 2;

                  const int tx = ((thisX + otherX) / 2) + offsetX;
                  const int ty = ((thisY + otherY) / 2) + offsetY;
                  linkText->m_pDurationText->setPos(tx, ty);

                  const int rx = ((thisX + otherX) / 2) + offsetX - (MARGIN_DURATION / 2);
                  const int ry = ((thisY + otherY) / 2) + offsetY - (MARGIN_DURATION / 2);
                  const int rw = linkText->m_pDurationText->boundingRect().width() + MARGIN_DURATION;
                  const int rh = linkText->m_pDurationText->boundingRect().height() + MARGIN_DURATION;
                  linkText->m_pBackground->setRect(rx, ry, rw, rh);
               }
            }
         }
      }
   }
}


// ###### Update layout #####################################################
void RDGraphicsNode::advance(int phase)
{
   if(phase == 1) {
      m_ConfigNode->updateStatus();

      // ====== Update workload label =======================================
      const double workload = m_ConfigNode->getWorkload();
      if(workload >= 0.00) {
         QColor workloadColor;
         if(workload > 0.75) {
            workloadColor = Qt::red;
         }
         else if(workload >= 0.50) {
            workloadColor = Qt::yellow;
         }
         else if(workload > 0.001) {
            workloadColor = Qt::green;
         }
         else {
            workloadColor = QColor("#202020");
         }
         m_pWorkload->setPen(workloadColor);
         m_pWorkloadBackground->show();
      }
      else {
         m_pWorkloadBackground->hide();
      }
      m_pWorkload->setText(m_ConfigNode->getWorkloadString());

      // ====== Update status label =========================================
      m_pStatusText->setText(m_ConfigNode->getStatusText());

      // ====== Update location label =======================================
      m_pLocationText->setText(m_ConfigNode->getLocationText());

      // ====== Update links ================================================

      // ------ Remove links ------------------------------------------------
      QMap<QString, int>& rMap = m_ConfigNode->getConnectedUIDsMap();
      QMap<QString, QGraphicsLineItem*>::iterator iterator = m_ConUIDsLinesMap.begin();
      while(iterator != m_ConUIDsLinesMap.end()) {
         QGraphicsLineItem* lineItem = *iterator;
         if(rMap.find(iterator.key()) == rMap.end()) {

            QMap<QString, LinkText*>::iterator found = m_ConUIDsTextMap.find(iterator.key());
            if(found != m_ConUIDsTextMap.end()) {
               LinkText* linkText = *found;
               if(linkText != NULL) {
//                   printf("DEL: %s\n", found.key().toStdString().c_str());
                  m_GraphicsScene->removeItem(linkText->m_pDurationText);
//                   delete linkText->m_pDurationText;
                  linkText->m_pDurationText = NULL;
                  m_GraphicsScene->removeItem(linkText->m_pBackground);
//                   delete linkText->m_pBackground;
                  linkText->m_pBackground = NULL;
                  delete linkText;
                  m_ConUIDsTextMap.erase(found);
               }
            }
            m_GraphicsScene->removeItem(lineItem);
//             delete lineItem;

            iterator = m_ConUIDsLinesMap.erase(iterator);
         }
         else {
            iterator++;
         }
      }

      // ------ Add new links -----------------------------------------------
      const int displaySizeX = m_Configuration->getDisplaySizeX();
      const int displaySizeY = m_Configuration->getDisplaySizeY();
      for(QMap<QString, int>::iterator iterator = rMap.begin(); iterator != rMap.end(); ++iterator) {
         if(m_GraphicsScene->getCanvasNodesMap().find(iterator.key()) != m_GraphicsScene->getCanvasNodesMap().end()) {
            // const RDGraphicsNode* pOtherNode = *(m_GraphicsScene->getCanvasNodesMap().find(iterator.key()));
            // printf("%s -> %s\n", m_ConfigNode->getDisplayName().toStdString().c_str(),
            //                      pOtherNode->m_ConfigNode->getDisplayName().toStdString().c_str());
            if(m_ConUIDsLinesMap.find(iterator.key()) == m_ConUIDsLinesMap.end()) {
               QGraphicsLineItem* line = m_GraphicsScene->addLine(QLineF(), QPen(getColor(*iterator), 5));
               Q_CHECK_PTR(line);
               m_ConUIDsLinesMap[iterator.key()] = line;
               line->setZValue(0);
               line->show();
            }

            const uint64_t duration = m_ConfigNode->getConnectedUIDsDurationMap()[iterator.key()];
            if(duration != ~0ULL) {
               // ====== Create link text, if not already existing ==========
               QGraphicsTextItem* pDurationText;
               if(m_ConUIDsTextMap.find(iterator.key()) == m_ConUIDsTextMap.end()) {
                  const double z = (((m_ConfigNode->getPositionX(displaySizeX) % 1024) << 10) +
                                     (m_ConfigNode->getPositionY(displaySizeY) % 1024)) << 4;

                  LinkText* linkText = new LinkText;
                  Q_CHECK_PTR(linkText);
                  m_ConUIDsTextMap[iterator.key()] = linkText;

                  pDurationText = m_GraphicsScene->addText("", QFont("Helvetica", 11, QFont::Bold));
                  Q_CHECK_PTR(pDurationText);
                  pDurationText->setZValue(z + 1);
                  pDurationText->setDefaultTextColor(getColor(*iterator));
                  pDurationText->show();
                  linkText->m_pDurationText = pDurationText;

                  QGraphicsRectItem* pDurationRectangle = m_GraphicsScene->addRect(
                     QRectF(), QPen(QColor("#7C7777"), 5), QBrush(QColor("#FFFF00")));
                  Q_CHECK_PTR(pDurationRectangle);
                  pDurationRectangle->setZValue(z);
                  pDurationRectangle->show();
                  linkText->m_pBackground = pDurationRectangle;
               }
               else {
                  pDurationText = m_ConUIDsTextMap[iterator.key()]->m_pDurationText;
               }

               // ====== Update duration label ==============================
               char timeBuffer[64];
               snprintf(timeBuffer, sizeof(timeBuffer), "%2llu:%02llum",
                       (unsigned long long)duration / 60000000,
                       (unsigned long long)(duration / 1000000) % 60);
               pDurationText->setPlainText(QString(timeBuffer));
            }
         }
      }

      // ====== Update pixmap ===============================================
      setPixmap((m_ConfigNode->getStatus() == 0) ? m_InactivePixmap : m_ActivePixmap);

      // ====== Update positions ============================================
      updatePosition();
   }
}
