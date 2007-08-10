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
 *   Authors: Thomas Dreibholz, dreibh@exp-math.uni-essen.de
 *            Sebastian Rohde, rohde@exp-math.uni-essen.de
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

#include <qobject.h>
#include <qaction.h>
#include <qmessagebox.h>
#include <qstringlist.h>
#include <qdatetime.h>

#include "canvasnode.h"
#include "node.h"
#include "mainwidget.h"


#define MARGIN_WORKLOAD   4
#define MARGIN_DURATION   4
#define MARGIN_BACKGROUND 8


 CCanvasNode::CCanvasNode(CCanvas*             nodeCanvas,
                          CNode*               node,
                          QValueList<QPixmap>& pixmapList)
   : QCanvasSprite(new QCanvasPixmapArray(pixmapList), nodeCanvas),
     m_Canvas(nodeCanvas),
     m_Node(node)
 {
   int sizeX = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeX();
   int sizeY = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeY();
   move(m_Node->getPositionX(sizeX), m_Node->getPositionY(sizeY));
   m_ZPosition = 1000000000 + ((((m_Node->getPositionX(sizeX) % 1024) << 10) +
                                 (m_Node->getPositionY(sizeY) % 1024)) << 4);
   setZ(m_ZPosition + 10);

   const int spriteHeight = height();
   m_pTitle = new QCanvasText(m_Node->getDisplayName(), canvas());
   m_pTitle->setFont(QFont("Helvetica", 12, QFont::Bold ));
   m_pTitle->move((width() / 2) - ((m_pTitle->boundingRect().right() - m_pTitle->boundingRect().left())/ 2) + m_Node->getPositionX(sizeX), m_Node->getPositionY(sizeY) + spriteHeight);
   m_pTitle->setZ(m_ZPosition + 6);
   m_pTitle->show();

   m_pWorkload = new QCanvasText("", canvas());
   m_pWorkload->setFont(QFont("Helvetica", 12, QFont::Bold));
   m_pWorkload->setColor(QColor("#222222"));
   m_pWorkload->setTextFlags(Qt::AlignCenter);
   m_pWorkload->setZ(2000000001);

   QFontMetrics workloadFM(m_pWorkload->font());
   m_pWorkloadBackground = new QCanvasRectangle(m_Canvas);
   m_pWorkloadBackground->setZ(2000000000);
   m_pWorkloadBackground->setSize(MARGIN_WORKLOAD + workloadFM.width("100%"),
                                  MARGIN_WORKLOAD + workloadFM.height());

   m_pStatusText = new QCanvasText(m_Node->getStatusText(), canvas());
   m_pStatusText->setFont(QFont("Helvetica", 10, QFont::Bold ));
   m_pStatusText->setZ(m_ZPosition + 6);

   m_pLocationText = new QCanvasText(m_Node->getLocationText(), canvas());
   m_pLocationText->setFont(QFont("Helvetica", 6));
   m_pLocationText->setZ(m_ZPosition + 6);

   m_pBackground = new QCanvasRectangle(m_Canvas);
   m_pBackground->setZ(m_ZPosition + 5);
   m_pBackground->setBrush(QBrush(QColor("#FFD700")));
   m_pBackground->setPen(QPen(QColor("#7C7777")));

   m_ContextMenu = new QPopupMenu(dynamic_cast<QCanvasView *>(canvas()->parent()));
   QPtrList<CContextMenuConfig>& rNodeList = m_Node->getContextMenuConfig();
   for(CContextMenuConfig* pNode = rNodeList.first(); pNode; pNode = rNodeList.next()) {
      if(pNode->getName() != "") {
         QAction* action = new QAction(pNode->getName(),QKeySequence(),canvas()->parent());
         Q_CHECK_PTR(action);
         action->addTo(m_ContextMenu);
         QObject::connect(action, SIGNAL(activated()), pNode, SLOT(execute()));
      }
      else {
         m_ContextMenu->insertSeparator();
      }
   }

   const QString uid = m_Node->getUniqueID();
   (m_Canvas->getCanvasNodesMap())[uid] = this;
}


CCanvasNode::~CCanvasNode()
{
}


void CCanvasNode::advance(int phase)
{
   if(phase == 1) {
      const int sizeX = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeX();
      const int sizeY = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeY();

      m_Node->updateStatus();

      m_pWorkloadBackground->move(m_Node->getPositionX(sizeX) + (width() / 2),
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
         m_pWorkload->setColor(workloadColor);

         m_pWorkloadBackground->setBrush(QBrush(QColor("#D7D7ff")));
         m_pWorkloadBackground->setPen(QPen(QColor("#202020")));
         m_pWorkloadBackground->show();
      }
      else {
         m_pWorkloadBackground->hide();
      }
      m_pWorkload->setText(m_Node->getWorkloadString());
      m_pWorkload->move(m_pWorkloadBackground->x() + (MARGIN_WORKLOAD / 2) + ((m_pWorkloadBackground->boundingRect().right() - m_pWorkloadBackground->boundingRect().left()) / 2),
                        m_pWorkloadBackground->y() - (MARGIN_WORKLOAD / 2) - ((m_pWorkloadBackground->boundingRect().top() - m_pWorkloadBackground->boundingRect().bottom()) / 2));
      m_pWorkload->show();

      m_pStatusText->setText(m_Node->getStatusText());
      m_pStatusText->move((width() / 2) - ((m_pStatusText->boundingRect().right() - m_pStatusText->boundingRect().left())/ 2)
         + m_Node->getPositionX(sizeX), m_Node->getPositionY(sizeY) +  m_pTitle->boundingRect().bottom() - m_pTitle->boundingRect().top() + height());
      m_pStatusText->show();

      m_pLocationText->setText(m_Node->getLocationText());
      m_pLocationText->move((width() / 2) - ((m_pLocationText->boundingRect().width())/ 2)
         + m_Node->getPositionX(sizeX), m_Node->getPositionY(sizeY) +  m_pTitle->boundingRect().height() + m_pStatusText->boundingRect().height() + height());
      m_pLocationText->show();

      const int textWidth       = m_pTitle->boundingRect().width();
      const int statusTextWidth = m_pStatusText->boundingRect().width();
      const int ipTextWidth     = m_pLocationText->boundingRect().width();
      int boundingWidth         = textWidth > statusTextWidth ? textWidth : statusTextWidth;
      boundingWidth             = boundingWidth > ipTextWidth ? boundingWidth : ipTextWidth;
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

      m_pBackground->setSize(boundingWidth, boundingHeight);
      m_pBackground->move((width() / 2) - (boundingWidth/ 2) + m_Node->getPositionX(sizeX), m_Node->getPositionY(sizeY) + height() - (MARGIN_BACKGROUND / 2));
      m_pBackground->show();

      int thisX = 0;
      int thisY = 0;
      getAnchor(thisX, thisY);
      QMap<QString, int> &rMap = m_Node->getConnectedUIDsMap();
      QStringList DeletionList;
      for(QMap<QString, QCanvasLine*>::iterator it = m_ConUIDsLinesMap.begin();it != m_ConUIDsLinesMap.end();++it) {
         if(rMap.find(it.key()) == rMap.end()) {
            DeletionList.push_back(it.key());
            QMap<QString, LinkText*>::iterator FindText = m_ConUIDsTextMap.find(it.key());
            if(FindText != m_ConUIDsTextMap.end()) {
               LinkText* lt = FindText.data();
               if(lt != NULL) {
                  delete lt->m_pDurationText;
                  lt->m_pDurationText = NULL;
                  delete lt->m_pBackground;
                  lt->m_pBackground = NULL;
                  delete lt;
                  m_ConUIDsTextMap.erase(FindText);
               }
            }
            delete(it.data());
         }
      }
      for (QStringList::Iterator it = DeletionList.begin();it != DeletionList.end();++it) {
         m_ConUIDsLinesMap.remove(*it);
      }
      for(QMap<QString, int>::iterator it = rMap.begin(); it != rMap.end(); ++it) {
         if(m_Canvas->getCanvasNodesMap().find(it.key()) != m_Canvas->getCanvasNodesMap().end()) {
            CCanvasNode* pOtherNode = m_Canvas->getCanvasNodesMap().find(it.key()).data();
            int otherX = 0;
            int otherY = 0;
            pOtherNode->getAnchor(otherX, otherY);
            if(m_ConUIDsLinesMap.find(it.key()) == m_ConUIDsLinesMap.end()) {
               QCanvasLine* line = new QCanvasLine(m_Canvas);
               Q_CHECK_PTR(line);
               line->setPoints(thisX, thisY, otherX, otherY);
               m_ConUIDsLinesMap[it.key()] = line;
               QColor newColor;
               QMap<int, QString>::iterator colorName = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getColorMap().find(it.data());
               if(colorName != static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getColorMap().end()) {
                  newColor.setNamedColor(colorName.data());
               }
               else {
                  newColor.setNamedColor("black");
               }
               QPen linePen(newColor, 5);
               line->setZ(0);
               line->setPen(linePen);
               line->show();
            }

            uint64_t duration = m_Node->getConnectedUIDsDurationMap()[it.key()];
            if(duration != ~0ULL) {
               const double z = (((m_Node->getPositionX(sizeX) % 1024) << 10) +
                                  (m_Node->getPositionY(sizeY) % 1024)) << 4;
               if(m_ConUIDsTextMap.find(it.key()) == m_ConUIDsTextMap.end()) {
                  m_ConUIDsTextMap[it.key()] = new LinkText();
                  m_ConUIDsTextMap[it.key()]->m_pDurationText = new QCanvasText(m_Canvas);
                  m_ConUIDsTextMap[it.key()]->m_pDurationText->setZ(z + 1);
                  m_ConUIDsTextMap[it.key()]->m_pDurationText->setFont(QFont("Helvetica", 11, QFont::Bold ));
                  m_ConUIDsTextMap[it.key()]->m_pBackground = new QCanvasRectangle(m_Canvas);
                  m_ConUIDsTextMap[it.key()]->m_pBackground->setBrush(QBrush(QColor("#FFFF00")));
                  m_ConUIDsTextMap[it.key()]->m_pBackground->setPen(QPen(QColor("#7C7777")));
                  m_ConUIDsTextMap[it.key()]->m_pBackground->setZ(z);
               }
               QCanvasText*      pDurationText      = m_ConUIDsTextMap[it.key()]->m_pDurationText;
               QCanvasRectangle* pDurationRectangle = m_ConUIDsTextMap[it.key()]->m_pBackground;

               char timeBuffer[30];
               sprintf(timeBuffer, "%2llu:%02llum",
                       (unsigned long long)duration / 60000000,
                       (unsigned long long)(duration / 1000000) % 60);
               pDurationText->setText(QString(timeBuffer));
               QFontMetrics fm(pDurationText->font());
               const int offsetX = -fm.width(pDurationText->text()) / 2;
               const int offsetY = -fm.height() / 2;
               pDurationText->move(((thisX + otherX) / 2) + offsetX, ((thisY + otherY)/2) + offsetY);
               pDurationRectangle->move(((thisX + otherX) / 2) + offsetX - (MARGIN_DURATION / 2), ((thisY + otherY)/2) + offsetY - (MARGIN_DURATION / 2));
               pDurationRectangle->setSize(pDurationText->boundingRect().width() + MARGIN_DURATION, pDurationText->boundingRect().height() + MARGIN_DURATION);

               QColor newColor;
               QMap<int, QString>::iterator colorName = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getColorMap().find(it.data());
               if(colorName != static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getColorMap().end()) {
                  newColor.setNamedColor(colorName.data());
               }
               else {
                  newColor.setNamedColor("black");
               }
               pDurationText->setColor(newColor.dark());
               pDurationText->show();
               pDurationRectangle->show();
            }
         }
      }
      setFrame(static_cast<int>(m_Node->getStatus()));
   }
}


void CCanvasNode::getAnchor(int &_rX, int &_rY)
{
   int sizeX = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeX();
   int sizeY = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeY();

   _rX = m_Node->getPositionX(sizeX) + (width()/2);
   _rY = m_Node->getPositionY(sizeY) + (height()/2);
}


void CCanvasNode::updatePostion()
{
   int sizeX = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeX();
   int sizeY = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeY();
   move(m_Node->getPositionX(sizeX), m_Node->getPositionY(sizeY));
   int spriteHeight = height();
   m_pTitle->move((width() / 2) - ((m_pTitle->boundingRect().right() - m_pTitle->boundingRect().left())/ 2) + m_Node->getPositionX(sizeX), m_Node->getPositionY(sizeY) + spriteHeight);
   m_pWorkload->move(m_Node->getPositionX(sizeX) + (width() - m_pTitle->boundingRect().width()),
                     m_Node->getPositionY(sizeY));
   m_pStatusText->move((width() / 2) - ((m_pStatusText->boundingRect().right() - m_pStatusText->boundingRect().left())/ 2)
      + m_Node->getPositionX(sizeX), m_Node->getPositionY(sizeY) +  m_pTitle->boundingRect().bottom() - m_pTitle->boundingRect().top() + height());
   m_pLocationText->move((width() / 2) - (m_pLocationText->boundingRect().width()/ 2)
      + m_Node->getPositionX(sizeX), m_Node->getPositionY(sizeY) +  m_pTitle->boundingRect().height() + m_pStatusText->boundingRect().height() + height());

   int thisX = 0;
   int thisY = 0;
   int textWidth = m_pTitle->boundingRect().width();
   int statusTextWidth = m_pStatusText->boundingRect().width();
   int ipTextWidth = m_pLocationText->boundingRect().width();
   int boundingWidth = textWidth > statusTextWidth ? textWidth : statusTextWidth;
   boundingWidth = boundingWidth > ipTextWidth ? boundingWidth : ipTextWidth;
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

   m_pBackground->setSize(boundingWidth, boundingHeight);
   m_pBackground->move((width() / 2) - (boundingWidth/ 2) + m_Node->getPositionX(sizeX),
                       m_Node->getPositionY(sizeY) + height() - (MARGIN_BACKGROUND / 2));

   getAnchor(thisX, thisY);
   for(QMap<QString, QCanvasLine*>::iterator it = m_ConUIDsLinesMap.begin();it != m_ConUIDsLinesMap.end();++it) {
      if(m_Canvas->getCanvasNodesMap().find(it.key()) != m_Canvas->getCanvasNodesMap().end()) {
         CCanvasNode* pOtherNode = m_Canvas->getCanvasNodesMap().find(it.key()).data();
         int otherX = 0;
         int otherY = 0;
         pOtherNode->getAnchor(otherX, otherY);
         QCanvasLine *line = m_ConUIDsLinesMap[it.key()];
         if(line) {
            line->setPoints(thisX, thisY, otherX, otherY);

            uint64_t duration = m_Node->getConnectedUIDsDurationMap()[it.key()];
            LinkText* pLinkText = m_ConUIDsTextMap[it.key()];
            if(duration != ~0ULL && pLinkText && pLinkText->m_pDurationText && pLinkText->m_pBackground) {
               const int offsetX = 0;
               const int offsetY = 0;
               pLinkText->m_pDurationText->move(((thisX + otherX) / 2) + offsetX, ((thisY + otherY)/2) + offsetY);
               pLinkText->m_pBackground->move(((thisX + otherX) / 2) + offsetX - (MARGIN_BACKGROUND / 2), ((thisY + otherY)/2) + offsetY - (MARGIN_BACKGROUND / 2));
            }
         }
      }
   }
}
