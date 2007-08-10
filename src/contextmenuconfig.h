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
 * ############- An Efficient RSerPool Prototype Implementation #############
 *
 *   Authors: Thomas Dreibholz, dreibh@exp-math.uni-essen.de               *
 *            Sebastian Rohde, rohde@exp-math.uni-essen.de                 *
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

#ifndef CONTEXTMENUCONFIG_H
#define CONTEXTMENUCONFIG_H

#include <qobject.h>
#include <qstring.h>


class QProcess;

class CContextMenuConfig : public QObject
{
   Q_OBJECT
   public:
   CContextMenuConfig(QWidget*       parent,
                      const QString& nodeName,
                      const QString& itemName,
                      const QString& commandLine);
   virtual ~CContextMenuConfig();

   inline const QString& getName() const {
      return m_ItemName;
   }

   public slots:
   virtual void execute();

   private slots:
   virtual void processFinished();
   virtual void readStdout();
   virtual void readStderr();

   private:
   QWidget*         m_Parent;
   QString          m_NodeName;
   QString          m_ItemName;
   QString          m_CommandLine;
   static QProcess* m_pProcess;
};

#endif
