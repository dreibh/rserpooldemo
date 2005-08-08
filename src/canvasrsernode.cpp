/***************************************************************************
 *   The RSerPool Demo System                                              *
 *                                                                         *
 *   Authors: Thomas Dreibholz, dreibh@exp-math.uni-essen.de               *
 *            Sebastian Rohde, rohde@exp-math.uni-essen.de                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <qobject.h>
#include <qaction.h>
#include <qmessagebox.h>
#include <qstringlist.h>
#include <qdatetime.h>

#include "canvasrsernode.h"
#include "rsernode.h"
#include "mainwidget.h"


 CCanvasRSerNode::CCanvasRSerNode(CRSerPoolCanvas*     _Canvas,
                                  CRSerPoolNode*       _RSerNode,
                                  QValueList<QPixmap>& _PixmapList )
   : QCanvasSprite(new QCanvasPixmapArray(_PixmapList), _Canvas),
     m_Canvas(_Canvas),
     m_RSerNode(_RSerNode)
 {
   int sizeX = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeX();
   int sizeY = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeY();
   move(m_RSerNode->getPositionX(sizeX), m_RSerNode->getPositionY(sizeY));
   m_ZPosition = (((m_RSerNode->getPositionX(sizeX) % 1024) << 10) + (m_RSerNode->getPositionY(sizeY) % 1024)) << 4;
   setZ(m_ZPosition + 10);

   const int spriteHeight = height();
   m_pText = new QCanvasText(m_RSerNode->getDisplayName(), canvas());
   m_pText->setFont(QFont("Helvetica", 14, QFont::Bold ));
   m_pText->move((width() / 2) - ((m_pText->boundingRect().right() - m_pText->boundingRect().left())/ 2) + m_RSerNode->getPositionX(sizeX), m_RSerNode->getPositionY(sizeY) + spriteHeight);
   m_pText->setZ(m_ZPosition + 6);
   m_pText->show();
   m_pStatusText = new QCanvasText(m_RSerNode->getStatusText(), canvas());
   m_pStatusText->setFont(QFont("Helvetica", 12, QFont::Bold ));
   m_pStatusText->move((width() / 2) - ((m_pStatusText->boundingRect().right() - m_pStatusText->boundingRect().left())/ 2)
      + m_RSerNode->getPositionX(sizeX), m_RSerNode->getPositionY(sizeY) +  m_pText->boundingRect().bottom() - m_pText->boundingRect().top() + height());
   m_pStatusText->setZ(m_ZPosition + 6);
   m_pStatusText->show();

   m_pIPText = new QCanvasText(m_RSerNode->getIPText(), canvas());
   m_pIPText->setFont(QFont("Helvetica", 8, QFont::Bold ));
   m_pIPText->move((width() / 2) - ((m_pIPText->boundingRect().width())/ 2)
      + m_RSerNode->getPositionX(sizeX), m_RSerNode->getPositionY(sizeY) +  m_pText->boundingRect().height() +  m_pStatusText->boundingRect().height() + height());
   m_pIPText->setZ(m_ZPosition + 6);
   m_pIPText->show();

   const int margin = 10;
   int textWidth = m_pText->boundingRect().width();
   int statustextWidth = m_pStatusText->boundingRect().width();
   int ipTextWidth = m_pIPText->boundingRect().width();
   int boundingWidth = textWidth > statustextWidth ? textWidth : statustextWidth;
   boundingWidth = boundingWidth > ipTextWidth ? boundingWidth : ipTextWidth;
   boundingWidth += margin;

   int boundingHeight;
   if(m_pStatusText->text() == "" && m_pIPText->text() == "") {
      boundingHeight = m_pText->boundingRect().height();
   }
   else if (m_pIPText->text() == "") {
      boundingHeight = m_pText->boundingRect().height() + m_pStatusText->boundingRect().height();
   }
   else {
      boundingHeight = m_pText->boundingRect().height() + m_pStatusText->boundingRect().height() + m_pIPText->boundingRect().height();
   }
   boundingHeight += margin;

   m_pTextBackground = new QCanvasRectangle((width() / 2) - (boundingWidth/ 2) + m_RSerNode->getPositionX(sizeX), m_RSerNode->getPositionY(sizeY) + spriteHeight - (margin / 2), boundingWidth, boundingHeight,_Canvas);
   m_pTextBackground->setZ(m_ZPosition + 5);
   m_pTextBackground->setBrush(QBrush(QColor("#FFD700")));
   m_pTextBackground->setPen(QPen(QColor("#7C7777")));
   m_pTextBackground->show();

   m_ContextMenu = new QPopupMenu(dynamic_cast<QCanvasView *>(canvas()->parent()));
   QPtrList<CContextMenuConfig>& rNodeList = m_RSerNode->getContextMenuConfig();
   for(CContextMenuConfig* pNode = rNodeList.first(); pNode; pNode = rNodeList.next()) {
      QAction* pNewAction = new QAction(pNode->getName(),QKeySequence(),canvas()->parent());
      QObject::connect(pNewAction, SIGNAL(activated()), pNode, SLOT(execute()));
      pNewAction->addTo(m_ContextMenu);
   }

   const QString uid = m_RSerNode->getUniqueID();
   (m_Canvas->getCanvasRSerPoolNodesMap())[uid] = this;
}


CCanvasRSerNode::~CCanvasRSerNode()
{
}


void CCanvasRSerNode::advance(int phase)
{
   if(phase == 1) {
      const int sizeX = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeX();
      const int sizeY = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeY();
      int thisX = 0;
      int thisY = 0;

      m_RSerNode->updateStatus();
      m_pStatusText->setText(m_RSerNode->getStatusText());
      m_pStatusText->move((width() / 2) - ((m_pStatusText->boundingRect().right() - m_pStatusText->boundingRect().left())/ 2)
         + m_RSerNode->getPositionX(sizeX), m_RSerNode->getPositionY(sizeY) +  m_pText->boundingRect().bottom() - m_pText->boundingRect().top() + height());
      m_pIPText->setText(m_RSerNode->getIPText());
      m_pIPText->move((width() / 2) - ((m_pIPText->boundingRect().width())/ 2)
         + m_RSerNode->getPositionX(sizeX), m_RSerNode->getPositionY(sizeY) +  m_pText->boundingRect().height() +  m_pStatusText->boundingRect().height() + height());

      const int margin          = 10;
      const int textWidth       = m_pText->boundingRect().width();
      const int statusTextWidth = m_pStatusText->boundingRect().width();
      const int ipTextWidth     = m_pIPText->boundingRect().width();
      int boundingWidth   = textWidth > statusTextWidth ? textWidth : statusTextWidth;
      boundingWidth = boundingWidth > ipTextWidth ? boundingWidth : ipTextWidth;
      boundingWidth += margin;

      int boundingHeight;
      if(m_pStatusText->text() == "" && m_pIPText->text() == "") {
         boundingHeight = m_pText->boundingRect().height();
      }
      else if (m_pIPText->text() == "") {
         boundingHeight = m_pText->boundingRect().height() + m_pStatusText->boundingRect().height();
      }
      else {
         boundingHeight = m_pText->boundingRect().height() + m_pStatusText->boundingRect().height() + m_pIPText->boundingRect().height();
      }

      boundingHeight += margin;

      m_pTextBackground->setSize(boundingWidth, boundingHeight);
      m_pTextBackground->move((width() / 2) - (boundingWidth/ 2) + m_RSerNode->getPositionX(sizeX), m_RSerNode->getPositionY(sizeY) + height() - (margin / 2));


      getAnchor(thisX, thisY);
      QMap<QString, int> &rMap = m_RSerNode->getConnectedUIDsMap();
      QStringList DeletionList;
      for(QMap<QString, QCanvasLine*>::iterator it = m_ConUIDsLinesMap.begin();it != m_ConUIDsLinesMap.end();++it) {
         if(rMap.find(it.key()) == rMap.end()) {
            DeletionList.push_back(it.key());
            QMap<QString, LinkText*>::iterator FindText = m_ConUIDsTextMap.find(it.key());
            if(FindText != m_ConUIDsTextMap.end()) {
               LinkText* lt = FindText.data();
               if(lt != NULL) {
                  delete lt->m_pText;
                  lt->m_pText = NULL;
                  delete lt->m_pBoundingRectangle;
                  lt->m_pBoundingRectangle = NULL;
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
         if(m_Canvas->getCanvasRSerPoolNodesMap().find(it.key()) != m_Canvas->getCanvasRSerPoolNodesMap().end()) {
            CCanvasRSerNode* pOtherNode = m_Canvas->getCanvasRSerPoolNodesMap().find(it.key()).data();
            int otherX = 0;
            int otherY = 0;
            pOtherNode->getAnchor(otherX, otherY);
            if(m_ConUIDsLinesMap.find(it.key()) == m_ConUIDsLinesMap.end()) {
               QCanvasLine* pLine = new QCanvasLine(m_Canvas);
               pLine->setPoints(thisX, thisY, otherX, otherY);
               m_ConUIDsLinesMap[it.key()] = pLine;
               QColor newColor;
               QMap<int, QString>::iterator colorName = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getColorMap().find(it.data());
               if(colorName != static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getColorMap().end()) {
                  newColor.setNamedColor(colorName.data());
               }
               else {
                  newColor.setNamedColor("black");
               }
               QPen linePen(newColor, 5);
               pLine->setPen(linePen);
               pLine->show();

            }

            uint64_t duration = m_RSerNode->getConnectedUIDsDurationMap()[it.key()];
            if(duration != ~0ULL) {
               if(m_ConUIDsTextMap.find(it.key()) == m_ConUIDsTextMap.end()) {
                  m_ConUIDsTextMap[it.key()] = new LinkText();
                  m_ConUIDsTextMap[it.key()]->m_pText = new QCanvasText(m_Canvas);
                  m_ConUIDsTextMap[it.key()]->m_pText->setFont(QFont("Helvetica", 14, QFont::Bold ));
                  m_ConUIDsTextMap[it.key()]->m_pBoundingRectangle = new QCanvasRectangle(m_Canvas);
                  m_ConUIDsTextMap[it.key()]->m_pBoundingRectangle->setBrush(QBrush(QColor("#FFFF00")));
                  m_ConUIDsTextMap[it.key()]->m_pBoundingRectangle->setPen(QPen(QColor("#7C7777")));
                  m_ConUIDsTextMap[it.key()]->m_pBoundingRectangle->setZ(m_ZPosition + 3);
               }
               QCanvasText*      pDurationText      = m_ConUIDsTextMap[it.key()]->m_pText;
               QCanvasRectangle* pDurationRectangle = m_ConUIDsTextMap[it.key()]->m_pBoundingRectangle;

               char timeBuffer[30];
               sprintf(timeBuffer, "%2Lu:%02Lu.%01Lus", duration / 60000000, (duration / 1000000) % 60, (duration % 1000000) / 100000);
               pDurationText->setText(QString(timeBuffer));
               int offsetX = 0;
               int offsetY = 0;
               if(otherY > thisY) {
                  offsetX += 15;
               }
               else {
                  offsetX += 15;
               }
               const int margin = 10;
               pDurationText->move(((thisX+otherX) / 2) + offsetX, ((thisY+ otherY)/2) + offsetY);
               pDurationRectangle->move(((thisX+otherX) / 2) + offsetX - (margin / 2), ((thisY+ otherY)/2) + offsetY - (margin / 2));
               pDurationRectangle->setSize(pDurationText->boundingRect().width() + margin, pDurationText->boundingRect().height() + margin);

               QColor newColor;
               QMap<int, QString>::iterator colorName = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getColorMap().find(it.data());
               if(colorName != static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getColorMap().end()) {
                  newColor.setNamedColor(colorName.data());
               }
               else {
                  newColor.setNamedColor("black");
               }
               newColor = newColor.dark();
               pDurationText->setColor(newColor);
               pDurationText->setZ(m_ZPosition + 4);
               pDurationText->show();
               pDurationRectangle->show();
            }
         }
      }
      setFrame(static_cast<int>(m_RSerNode->getStatus()));
   }
}


void CCanvasRSerNode::getAnchor(int &_rX, int &_rY)
{
   int sizeX = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeX();
   int sizeY = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeY();

   _rX = m_RSerNode->getPositionX(sizeX) + (width()/2);
   _rY = m_RSerNode->getPositionY(sizeY) + (height()/2);
}


void CCanvasRSerNode::updatePostion()
{
   int sizeX = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeX();
   int sizeY = static_cast<CMainWidget *>(canvas()->parent())->m_Configuration.getDisplaySizeY();
   move(m_RSerNode->getPositionX(sizeX), m_RSerNode->getPositionY(sizeY));
   int spriteHeight = height();
   m_pText->move((width() / 2) - ((m_pText->boundingRect().right() - m_pText->boundingRect().left())/ 2) + m_RSerNode->getPositionX(sizeX), m_RSerNode->getPositionY(sizeY) + spriteHeight);
   m_pStatusText->move((width() / 2) - ((m_pStatusText->boundingRect().right() - m_pStatusText->boundingRect().left())/ 2)
      + m_RSerNode->getPositionX(sizeX), m_RSerNode->getPositionY(sizeY) +  m_pText->boundingRect().bottom() - m_pText->boundingRect().top() + height());
   m_pIPText->move((width() / 2) - (m_pIPText->boundingRect().width()/ 2)
      + m_RSerNode->getPositionX(sizeX), m_RSerNode->getPositionY(sizeY) +  m_pText->boundingRect().height() + m_pStatusText->boundingRect().height() + height());

   int thisX = 0;
   int thisY = 0;
   const int margin = 10;
   int textWidth = m_pText->boundingRect().width();
   int statusTextWidth = m_pStatusText->boundingRect().width();
   int ipTextWidth = m_pIPText->boundingRect().width();
   int boundingWidth = textWidth > statusTextWidth ? textWidth : statusTextWidth;
   boundingWidth = boundingWidth > ipTextWidth ? boundingWidth : ipTextWidth;
   boundingWidth += margin;

   int boundingHeight;
   if(m_pStatusText->text() == "" && m_pIPText->text() == "") {
      boundingHeight = m_pText->boundingRect().height();
   }
   else if (m_pIPText->text() == "") {
      boundingHeight = m_pText->boundingRect().height() + m_pStatusText->boundingRect().height();
   }
   else {
      boundingHeight = m_pText->boundingRect().height() + m_pStatusText->boundingRect().height() + m_pIPText->boundingRect().height();
   }
   boundingHeight += margin;

   m_pTextBackground->setSize(boundingWidth, boundingHeight);
   m_pTextBackground->move((width() / 2) - (boundingWidth/ 2) + m_RSerNode->getPositionX(sizeX), m_RSerNode->getPositionY(sizeY) + height() - (margin / 2));

   getAnchor(thisX, thisY);
   for(QMap<QString, QCanvasLine*>::iterator it = m_ConUIDsLinesMap.begin();it != m_ConUIDsLinesMap.end();++it) {
      if(m_Canvas->getCanvasRSerPoolNodesMap().find(it.key()) != m_Canvas->getCanvasRSerPoolNodesMap().end()) {
         CCanvasRSerNode* pOtherNode = m_Canvas->getCanvasRSerPoolNodesMap().find(it.key()).data();
         int otherX = 0;
         int otherY = 0;
         pOtherNode->getAnchor(otherX, otherY);
         QCanvasLine *pLine = m_ConUIDsLinesMap[it.key()];
         if(pLine) {
            pLine->setPoints(thisX, thisY, otherX, otherY);

            uint64_t duration = m_RSerNode->getConnectedUIDsDurationMap()[it.key()];
            LinkText *pLinkText = m_ConUIDsTextMap[it.key()];
            if(duration != ~0ULL && pLinkText && pLinkText->m_pText && pLinkText->m_pBoundingRectangle) {
               int offsetX = 0;
               int offsetY = 0;
               offsetX += 15;
               const int margin = 10;
               pLinkText->m_pText->move(((thisX+otherX) / 2) + offsetX, ((thisY+ otherY)/2) + offsetY);
               pLinkText->m_pBoundingRectangle->move(((thisX+otherX) / 2) + offsetX - (margin / 2), ((thisY+ otherY)/2) + offsetY - (margin / 2));
            }
         }
      }
   }
}
