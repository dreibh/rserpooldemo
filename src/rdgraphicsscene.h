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
 *   Copyright (C) 2002-2024 by Thomas Dreibholz
 *
 *   Authors: Thomas Dreibholz, thomas.dreibholz@gmail.com
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
 * Contact: thomas.dreibholz@gmail.com
 */

#ifndef CANVAS_H
#define CANVAS_H

#include <QMap>
#include <QString>
#include <QTimer>
#include <QGraphicsScene>

#include "rdconfiguration.h"


class RDGraphicsNode;


class RDGraphicsScene : public QGraphicsScene
{
   Q_OBJECT
   public:
   RDGraphicsScene(QObject* parent, RDConfiguration* configuration);
   ~RDGraphicsScene();

   void setAdvancePeriod(int ms);
   inline QMap<QString, RDGraphicsNode*>& getCanvasNodesMap() {
      return m_GraphicsNodeMap;
   }

   public slots:
   void advance();

   private:
   RDConfiguration*               m_Configuration;
   QTimer*                        m_AdvanceTimer;
   QMap<QString, RDGraphicsNode*> m_GraphicsNodeMap;
};


#endif
