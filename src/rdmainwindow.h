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
 *   Copyright (C) 2002-2025 by Thomas Dreibholz
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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include <QMap>
#include <QImage>
#include <QResizeEvent>

#include "rdconfiguration.h"
#include "rdgraphicsnode.h"
#include "rdgraphicsscene.h"


class RDConfiguration;
class RDGraphicsView;


class RDMainWindow : public QMainWindow
{
   Q_OBJECT
   public:
   RDMainWindow(const QString& configFile);
   virtual ~RDMainWindow();

   protected:
   void resizeEvent(QResizeEvent* event);

   private:
   void createNodes();

   RDConfiguration  m_Configuration;
   QImage           m_BackgroundImage;
   RDGraphicsScene* m_GraphicsScene;
   RDGraphicsView*  m_GraphicsView;
};


#endif
