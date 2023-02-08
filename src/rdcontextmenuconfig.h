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
 *   Copyright (C) 2002-2023 by Thomas Dreibholz
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

#ifndef CONTEXTMENUCONFIG_H
#define CONTEXTMENUCONFIG_H

#include <QObject>
#include <QString>
#include <QProcess>


class RDContextMenuConfig : public QObject
{
   Q_OBJECT
   public:
   RDContextMenuConfig(QWidget*       parent,
                       const QString& nodeName,
                       const QString& itemName,
                       const QString& commandLine);
   virtual ~RDContextMenuConfig();

   inline const QString& getName() const {
      return m_ItemName;
   }

   public slots:
   virtual void execute();

   private slots:
   virtual void processFinished(int, QProcess::ExitStatus);
   virtual void readStdout();
   virtual void readStderr();

   private:
   static QStringList splitCommandLine(const QString& commandLine);

   QWidget*         m_Parent;
   QString          m_NodeName;
   QString          m_ItemName;
   QString          m_CommandLine;
   static QProcess* m_pProcess;
};

#endif
