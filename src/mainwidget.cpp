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
#include <mainwidget.h>
#include "canvasrsernode.h"


#include <qfont.h>
#include <qcanvas.h>
#include <qmessagebox.h>
#include "rserpoolcanvasview.h"
#include "rserpoolcanvas.h"



CMainWidget::CMainWidget(QString _ConfigFile)
   : QMainWindow( 0, 0, WDestructiveClose),
     m_Configuration(_ConfigFile)
{
   m_Canvas = new CRSerPoolCanvas(this);
   m_Canvas->resize(m_Configuration.getDisplaySizeX(),
                    m_Configuration.getDisplaySizeY());
   m_BackgroundImage = QPixmap(m_Configuration.getBackgroundImageName());
   QImage tempImage = m_BackgroundImage;
   tempImage.smoothScale(m_Configuration.getDisplaySizeX(),
                         m_Configuration.getDisplaySizeY()); // in pixels

   m_Canvas->setBackgroundPixmap(tempImage);
   m_CanvasView = new CSerPoolCanvasView(m_Canvas, this);
   m_CanvasView->setVScrollBarMode(QScrollView::AlwaysOff);
   m_CanvasView->setHScrollBarMode(QScrollView::AlwaysOff);
   setCentralWidget( m_CanvasView );

   createCanvasObjects();
   m_Canvas->setAdvancePeriod(m_Configuration.getRefreshTime());
   m_CanvasView->show();
}


CMainWidget::~CMainWidget()
{
}


void CMainWidget::createCanvasObjects()
{
   QPtrList<CRSerPoolNode>& rNodeList = m_Configuration.getRSerPoolNodes();
   for (CRSerPoolNode* pNode = rNodeList.first();pNode;pNode = rNodeList.next()) {
      QValueList<QPixmap> pixmapList;
      pixmapList.push_back(pNode->getImageNotReachable());
      pixmapList.push_back(pNode->getImageInactive());
      pixmapList.push_back(pNode->getImageActive());
      CCanvasRSerNode* pSprite = new CCanvasRSerNode(m_Canvas, pNode, pixmapList);
      pSprite->setAnimated(true);
      pSprite->show();
   }
}


void CMainWidget::resizeEvent(QResizeEvent* _pEvent)
{
   m_Canvas->resize(_pEvent->size().width(), _pEvent->size().height());
   m_Canvas->setBackgroundPixmap(m_BackgroundImage.smoothScale(_pEvent->size().width(), _pEvent->size().height()));
   m_Configuration.updateDisplaySize(_pEvent->size().width(), _pEvent->size().height());

   QMap<QString, CCanvasRSerNode*>& rNodeMap = m_Canvas->getCanvasRSerPoolNodesMap();
   for(QMap<QString, CCanvasRSerNode*>::iterator it = rNodeMap.begin();it != rNodeMap.end();++it) {
      it.data()->updatePostion();
   }
}
