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

#include <QObject>
#include <QAction>
#include <QStringList>
#include <QDateTime>
#include <QPixmap>
#include <QLinkedList>
#include <QMenu>
#include <QGraphicsView>

#include "canvasnode.h"
#include "node.h"
#include "mainwidget.h"


#define MARGIN_WORKLOAD   4
#define MARGIN_DURATION   4
#define MARGIN_BACKGROUND 8


CCanvasNode::CCanvasNode(CCanvas*        canvas,
                         CNode*          node,
                         CConfiguration* configuration,
                         const QPixmap&  inactivePixmap,
                         const QPixmap&  activePixmap)
 : QGraphicsPixmapItem(inactivePixmap),
   m_Canvas(canvas),
   m_Node(node),
   m_Configuration(configuration),
   m_InactivePixmap(inactivePixmap),
   m_ActivePixmap(activePixmap)
 {
   // ====== Create node icon ===============================================
   const int displaySizeX = m_Configuration->getDisplaySizeX();
   const int displaySizeY = m_Configuration->getDisplaySizeY();
//    setPos(m_Node->getPositionX(displaySizeX), m_Node->getPositionY(displaySizeY));
   m_ZPosition = 1000000000 + ((((m_Node->getPositionX(displaySizeX) % 1024) << 10) +
                                 (m_Node->getPositionY(displaySizeY) % 1024)) << 4);
   setZValue(m_ZPosition + 10);


   // ====== Create node title ==============================================
   m_pTitle = new QGraphicsSimpleTextItem(m_Node->getDisplayName(), this);
   m_pTitle->setFont(QFont("Helvetica", 12, QFont::Bold));
//    const int x = m_Node->getPositionX(displaySizeX) +
//                     ((boundingRect().width() / 2) -
//                      ((m_pTitle->boundingRect().right() - m_pTitle->boundingRect().left()) / 2));
//    const int y = m_Node->getPositionY(displaySizeY) + boundingRect().height();
//    m_pTitle->setPos(x, y);
   m_pTitle->setZValue(m_ZPosition + 6);
   m_Canvas->addItem(m_pTitle); 

   m_pBackground = new QGraphicsRectItem(this);
   m_pBackground->setZValue(m_ZPosition + 5);
   m_pBackground->setBrush(QBrush(QColor("#FFD700")));
   m_pBackground->setPen(QPen(QColor("#7C7777")));
   m_Canvas->addItem(m_pBackground);


   // ====== Create workload label ==========================================
   m_pWorkload = new QGraphicsSimpleTextItem("", this);
   m_pWorkload->setFont(QFont("Helvetica", 12, QFont::Bold));
   m_pWorkload->setPen(QColor("#222222"));
puts("setTextFlags(Qt::AlignCenter) !!!");
//    m_pWorkload->setTextFlags(Qt::AlignCenter);
   m_pWorkload->setZValue(2000000001);
   m_Canvas->addItem(m_pWorkload); 

   QFontMetrics workloadFM(m_pWorkload->font());
   m_pWorkloadBackground = new QGraphicsRectItem(0, 0,
                                                 MARGIN_WORKLOAD + workloadFM.width("100%"),
                                                 MARGIN_WORKLOAD + workloadFM.height(),
                                                 this);
   m_pWorkloadBackground->setZValue(2000000000);
// ???????
//    m_pWorkloadBackground->setSize(MARGIN_WORKLOAD + workloadFM.width("100%"),
//                                   MARGIN_WORKLOAD + workloadFM.height());
   m_Canvas->addItem(m_pWorkloadBackground); 


   // ====== Create status label ============================================
   m_pStatusText = new QGraphicsSimpleTextItem(m_Node->getStatusText(), this);
   m_pStatusText->setFont(QFont("Helvetica", 10, QFont::Bold ));
   m_pStatusText->setZValue(m_ZPosition + 6);
   m_Canvas->addItem(m_pStatusText); 


   // ====== Create location label ==========================================
   m_pLocationText = new QGraphicsSimpleTextItem(m_Node->getLocationText(), this);
   m_pLocationText->setFont(QFont("Helvetica", 6));
   m_pLocationText->setZValue(m_ZPosition + 6);
   m_Canvas->addItem(m_pLocationText); 


   // ====== Create context menus ===========================================
   m_ContextMenu = new QMenu(dynamic_cast<QGraphicsView*>(m_Canvas->parent()));
   QLinkedList<CContextMenuConfig*>& rNodeList = m_Node->getContextMenuConfig();
//    for(CContextMenuConfig* pNode = rNodeList.first(); pNode; pNode = rNodeList.next()) {
   for(QLinkedList<CContextMenuConfig*>::iterator it = rNodeList.begin();it != rNodeList.end();++it) {
      CContextMenuConfig* pNode = *it;
      if(pNode->getName() != "") {
         QAction* action = new QAction(pNode->getName(), m_Canvas->parent());
         Q_CHECK_PTR(action);
         m_ContextMenu->addAction(action);
//          action->addTo(m_ContextMenu);  ???
         QObject::connect(action, SIGNAL(activated()), pNode, SLOT(execute()));
      }
      else {
         m_ContextMenu->addSeparator();
      }
   }

   // ====== Create ID to nodes map =========================================
   const QString uid = m_Node->getUniqueID();
   (m_Canvas->getCanvasNodesMap())[uid] = this;
    
   m_Canvas->addItem(this);   
   updatePostion();
}


CCanvasNode::~CCanvasNode()
{
}


void CCanvasNode::advance(int phase)
{
    puts("CCanvasNode::advance()");
#if 0
   if(phase == 1) {
      const int sizeX = static_cast<CMainWidget *>(m_Canvas->parent())->m_Configuration.getDisplaySizeX();
      const int sizeY = static_cast<CMainWidget *>(m_Canvas->parent())->m_Configuration.getDisplaySizeY();

      m_Node->updateStatus();

      m_pWorkloadBackground->setPos(m_Node->getPositionX(sizeX) + (m_Canvas->width() / 2),
                                    m_Node->getPositionY(sizeY));
      const double workload = m_Node->getWorkload();
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

         m_pWorkloadBackground->setBrush(QBrush(QColor("#D7D7ff")));
         m_pWorkloadBackground->setPen(QPen(QColor("#202020")));
         m_pWorkloadBackground->show();
      }
      else {
         m_pWorkloadBackground->hide();
      }
      m_pWorkload->setText(m_Node->getWorkloadString());
      m_pWorkload->setPos(m_pWorkloadBackground->x() + (MARGIN_WORKLOAD / 2) + ((m_pWorkloadBackground->boundingRect().right() - m_pWorkloadBackground->boundingRect().left()) / 2),
                        m_pWorkloadBackground->y() - (MARGIN_WORKLOAD / 2) - ((m_pWorkloadBackground->boundingRect().top() - m_pWorkloadBackground->boundingRect().bottom()) / 2));
      m_pWorkload->show();

      m_pStatusText->setText(m_Node->getStatusText());
      m_pStatusText->setPos((m_Canvas->width() / 2) - ((m_pStatusText->boundingRect().right() - m_pStatusText->boundingRect().left())/ 2)
         + m_Node->getPositionX(sizeX), m_Node->getPositionY(sizeY) +  m_pTitle->boundingRect().bottom() - m_pTitle->boundingRect().top() + m_Canvas->height());
      m_pStatusText->show();

      m_pLocationText->setText(m_Node->getLocationText());
      m_pLocationText->setPos((m_Canvas->width() / 2) - ((m_pLocationText->boundingRect().width())/ 2)
         + m_Node->getPositionX(sizeX), m_Node->getPositionY(sizeY) +  m_pTitle->boundingRect().height() + m_pStatusText->boundingRect().height() + m_Canvas->height());
      m_pLocationText->show();

      const int titleWidth       = m_pTitle->boundingRect().width();
      const int statusTextWidth = m_pStatusText->boundingRect().width();
      const int locationTextWidth     = m_pLocationText->boundingRect().width();
      int boundingWidth         = titleWidth > statusTextWidth ? titleWidth : statusTextWidth;
      boundingWidth             = boundingWidth > locationTextWidth ? boundingWidth : locationTextWidth;
      boundingWidth += MARGIN_BACKGROUND;

      int boundingHeight;
      if(m_pStatusText->text() == "" && m_pLocationText->text() == "") {
         boundingHeight = m_pTitle->boundingRect().height();
      }
      else if (m_pLocationText->text() == "") {
         boundingHeight = m_pTitle->boundingRect().height() + m_pStatusText->boundingRect().height();
      }
      else {
         boundingHeight = m_pTitle->boundingRect().height() + m_pStatusText->boundingRect().height() + m_pLocationText->boundingRect().height();
      }


      boundingHeight += MARGIN_BACKGROUND;

      m_pBackground->setRect((m_Canvas->width() / 2) - (boundingWidth/ 2) + m_Node->getPositionX(sizeX), m_Node->getPositionY(sizeY) + m_Canvas->height() - (MARGIN_BACKGROUND / 2),
                             boundingWidth, boundingHeight);
      m_pBackground->show();

      int thisX = 0;
      int thisY = 0;
      getAnchor(thisX, thisY);
      QMap<QString, int> &rMap = m_Node->getConnectedUIDsMap();
      QStringList DeletionList;
      for(QMap<QString, QGraphicsLineItem*>::iterator it = m_ConUIDsLinesMap.begin();it != m_ConUIDsLinesMap.end();++it) {
         if(rMap.find(it.key()) == rMap.end()) {
            DeletionList.push_back(it.key());
            QMap<QString, LinkText*>::iterator found = m_ConUIDsTextMap.find(it.key());
            if(found != m_ConUIDsTextMap.end()) {
               LinkText* lt = *found;
               if(lt != NULL) {
                  delete lt->m_pDurationText;
                  lt->m_pDurationText = NULL;
                  delete lt->m_pBackground;
                  lt->m_pBackground = NULL;
                  delete lt;
                  m_ConUIDsTextMap.erase(found);
               }
            }
            delete *it;
         }
      }
      for (QStringList::Iterator it = DeletionList.begin();it != DeletionList.end();++it) {
         m_ConUIDsLinesMap.remove(*it);
      }
      for(QMap<QString, int>::iterator it = rMap.begin(); it != rMap.end(); ++it) {
         if(m_Canvas->getCanvasNodesMap().find(it.key()) != m_Canvas->getCanvasNodesMap().end()) {
            CCanvasNode* pOtherNode = *(m_Canvas->getCanvasNodesMap().find(it.key()));
            int otherX = 0;
            int otherY = 0;
            pOtherNode->getAnchor(otherX, otherY);
            if(m_ConUIDsLinesMap.find(it.key()) == m_ConUIDsLinesMap.end()) {
               QGraphicsLineItem* line = new QGraphicsLineItem(this);
               Q_CHECK_PTR(line);
               line->setLine(thisX, thisY, otherX, otherY);
               m_ConUIDsLinesMap[it.key()] = line;

               QColor newColor;
               QMap<int, QString>::iterator colorNameIterator = static_cast<CMainWidget *>(m_Canvas->parent())->m_Configuration.getColorMap().find(*it);
               if(colorNameIterator != static_cast<CMainWidget *>(m_Canvas->parent())->m_Configuration.getColorMap().end()) {
                  newColor = QColor(*colorNameIterator);
               }
               else {
                  newColor = QColor("black");
               }
               QPen linePen(newColor, 5);
               line->setZValue(0);
               line->setPen(linePen);
               line->show();
            }

            uint64_t duration = m_Node->getConnectedUIDsDurationMap()[it.key()];
            if(duration != ~0ULL) {
               const double z = (((m_Node->getPositionX(sizeX) % 1024) << 10) +
                                  (m_Node->getPositionY(sizeY) % 1024)) << 4;
               if(m_ConUIDsTextMap.find(it.key()) == m_ConUIDsTextMap.end()) {
                  m_ConUIDsTextMap[it.key()] = new LinkText();
                  m_ConUIDsTextMap[it.key()]->m_pDurationText = new QGraphicsSimpleTextItem(this);
                  m_ConUIDsTextMap[it.key()]->m_pDurationText->setZValue(z + 1);
                  m_ConUIDsTextMap[it.key()]->m_pDurationText->setFont(QFont("Helvetica", 11, QFont::Bold ));
                  m_ConUIDsTextMap[it.key()]->m_pBackground = new QGraphicsRectItem(this);
                  m_ConUIDsTextMap[it.key()]->m_pBackground->setBrush(QBrush(QColor("#FFFF00")));
                  m_ConUIDsTextMap[it.key()]->m_pBackground->setPen(QPen(QColor("#7C7777")));
                  m_ConUIDsTextMap[it.key()]->m_pBackground->setZValue(z);
               }
               QGraphicsSimpleTextItem* pDurationText      = m_ConUIDsTextMap[it.key()]->m_pDurationText;
               QGraphicsRectItem*       pDurationRectangle = m_ConUIDsTextMap[it.key()]->m_pBackground;

               char timeBuffer[30];
               sprintf(timeBuffer, "%2llu:%02llum",
                       (unsigned long long)duration / 60000000,
                       (unsigned long long)(duration / 1000000) % 60);
               pDurationText->setText(QString(timeBuffer));
               QFontMetrics fm(pDurationText->font());
               const int offsetX = -fm.width(pDurationText->text()) / 2;
               const int offsetY = -fm.height() / 2;
               pDurationText->setPos(((thisX + otherX) / 2) + offsetX, ((thisY + otherY)/2) + offsetY);
//                pDurationRectangle->setPos(((thisX + otherX) / 2) + offsetX - (MARGIN_DURATION / 2), ((thisY + otherY)/2) + offsetY - (MARGIN_DURATION / 2));
//                ?????
               pDurationRectangle->setRect(((thisX + otherX) / 2) + offsetX - (MARGIN_DURATION / 2), ((thisY + otherY)/2) + offsetY - (MARGIN_DURATION / 2),
                                           pDurationText->boundingRect().width() + MARGIN_DURATION, pDurationText->boundingRect().height() + MARGIN_DURATION);

               QColor newColor;
               QMap<int, QString>::iterator colorNameIterator = static_cast<CMainWidget *>(m_Canvas->parent())->m_Configuration.getColorMap().find(*it);
               if(colorNameIterator != static_cast<CMainWidget *>(m_Canvas->parent())->m_Configuration.getColorMap().end()) {
                  newColor = QColor(*colorNameIterator);
               }
               else {
                  newColor = QColor("black");
               }
               pDurationText->setPen(newColor.dark());
               pDurationText->show();
               pDurationRectangle->show();
            }
         }
      }
      printf("FIXME: setFrame()!!!   status=%d\n", static_cast<int>(m_Node->getStatus()));
//       setFrame(static_cast<int>(m_Node->getStatus()));
   }
#endif
}


void CCanvasNode::getAnchor(int& rX, int& rY)
{
   const int displaySizeX = m_Configuration->getDisplaySizeX();
   const int displaySizeY = m_Configuration->getDisplaySizeY();

   rX = m_Node->getPositionX(displaySizeX) + (m_Canvas->width() / 2);
   rY = m_Node->getPositionY(displaySizeY) + (m_Canvas->height() / 2);
}


void CCanvasNode::updatePostion()
{
   // ====== Update node icon ===============================================
   puts("CCanvasNode::updatePostion()");
   const int displaySizeX = m_Configuration->getDisplaySizeX();
   const int displaySizeY = m_Configuration->getDisplaySizeY();
   setPos(m_Node->getPositionX(displaySizeX),
          m_Node->getPositionY(displaySizeY));

   // ====== Update node title ==============================================
   const int tx = m_Node->getPositionX(displaySizeX) +
                     ((boundingRect().width() / 2) -
                      ((m_pTitle->boundingRect().right() - m_pTitle->boundingRect().left()) / 2));
   const int ty = m_Node->getPositionY(displaySizeY) + boundingRect().height();
   m_pTitle->setPos(tx, ty);

   // ====== Update status label ============================================
   const int sx = m_Node->getPositionX(displaySizeX) +
                       ((boundingRect().width() / 2) -
                        ((m_pStatusText->boundingRect().right() - m_pStatusText->boundingRect().left()) / 2));
   const int sy = m_Node->getPositionY(displaySizeY) +  m_pTitle->boundingRect().bottom() - m_pTitle->boundingRect().top() + boundingRect().height();
   m_pStatusText->setPos(sx, sy);

   // ====== Update location label ==========================================
   const int lx = m_Node->getPositionX(displaySizeX) +
                       ((boundingRect().width() / 2) -
                        ((m_pLocationText->boundingRect().right() - m_pLocationText->boundingRect().left()) / 2));
   const int ly = m_Node->getPositionY(displaySizeY) +  m_pTitle->boundingRect().height() + m_pLocationText->boundingRect().height() + boundingRect().height();
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

   const int bx = m_Node->getPositionX(displaySizeX) + (boundingRect().width() / 2) - (boundingWidth/ 2);
   const int by = m_Node->getPositionY(displaySizeY) + boundingRect().height() - (MARGIN_BACKGROUND / 2);
   m_pBackground->setRect(bx, by, boundingWidth, boundingHeight);

   // ====== Update links ===================================================
   int thisX;
   int thisY;
   getAnchor(thisX, thisY);
   for(QMap<QString, QGraphicsLineItem*>::iterator iterator = m_ConUIDsLinesMap.begin();iterator != m_ConUIDsLinesMap.end();++iterator) {
      if(m_Canvas->getCanvasNodesMap().find(iterator.key()) != m_Canvas->getCanvasNodesMap().end()) {
         CCanvasNode* pOtherNode = *(m_Canvas->getCanvasNodesMap().find(iterator.key()));
         int otherX = 0;
         int otherY = 0;
         pOtherNode->getAnchor(otherX, otherY);
         QGraphicsLineItem *line = m_ConUIDsLinesMap[iterator.key()];
         if(line) {
            line->setLine(thisX, thisY, otherX, otherY);

            const uint64_t duration = m_Node->getConnectedUIDsDurationMap()[iterator.key()];
            LinkText* pLinkText = m_ConUIDsTextMap[iterator.key()];
            if( (duration != ~0ULL) && (pLinkText != NULL) &&
                (pLinkText->m_pDurationText) && (pLinkText->m_pBackground) ) {
               const int offsetX = 0;
               const int offsetY = 0;
               pLinkText->m_pDurationText->setPos(((thisX + otherX) / 2) + offsetX,
                                                  ((thisY + otherY) / 2) + offsetY);
               pLinkText->m_pBackground->setPos(((thisX + otherX) / 2) + offsetX - (MARGIN_BACKGROUND / 2),
                                                ((thisY + otherY)/2) + offsetY - (MARGIN_BACKGROUND / 2));
            }
         }
      }
   }
}
