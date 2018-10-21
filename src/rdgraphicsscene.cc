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

#include "rdgraphicsscene.h"
#include "rdmainwindow.h"


// ###### Constructor #######################################################
RDGraphicsScene::RDGraphicsScene(QObject* parent, RDConfiguration* configuration)
   : QGraphicsScene(parent),
     m_Configuration(configuration)
{
    m_AdvanceTimer = new QTimer(this);
    Q_CHECK_PTR(m_AdvanceTimer);
    QObject::connect(m_AdvanceTimer, SIGNAL(timeout()), this, SLOT(advance()));
}


// ###### Destructor ########################################################
RDGraphicsScene::~RDGraphicsScene()
{
}


// ###### Set advance period ################################################
void RDGraphicsScene::setAdvancePeriod(int ms)
{
    m_AdvanceTimer->setInterval(ms);
    m_AdvanceTimer->start();
}


// ###### Update scenario ###################################################
void RDGraphicsScene::advance()
{
   m_Configuration->update();
   QGraphicsScene::advance();
}
