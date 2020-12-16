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
 *   Copyright (C) 2002-2020 by Thomas Dreibholz
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

#include <QApplication>
#include <QString>
#include <QDir>
#include <QFile>
#include <iostream>

#include "rdmainwindow.h"


// ###### Main program ######################################################
int main(int argc, char** argv)
{
   try {
      const QString configFileTag = "-config=";
      QString configFile = "local-setup.xml";
      for(int i = 1;i < argc;i++) {
         const QString command = argv[i];
         if(command.indexOf(configFileTag) == 0) {
            configFile = command.mid(configFileTag.length());
         }
         else if(QFile::exists(command)) {
            configFile = command;
         }
      }

      const QFileInfo configFileInfo(configFile);
      const QString configFileDirectory = QFileInfo(configFile).absoluteDir().absolutePath();
      configFile = configFileInfo.fileName();

      QDir::setCurrent(configFileDirectory);

      std::cout << "Using configuration \"" << configFile.toLocal8Bit().constData()
                << "\" in \"" << configFileDirectory.toLocal8Bit().constData()
                << "\" ..." << std::endl;
      QApplication application(argc, argv);
      RDMainWindow* mainWindow = new RDMainWindow(configFile);
      Q_CHECK_PTR(mainWindow);
      mainWindow->show();

      return application.exec();
   }
   catch(ELoadFileException& e) {
      std::cerr << "Unable to load config file!"  << std::endl;
      return 1;
   }
   catch(EXMLSyntaxException& e) {
      std::cerr << "Unable to load config file!"          << std::endl
                << "Error: " << e.m_Message.toStdString() << std::endl
                << "Line:  " << e.m_Line                  << std::endl;
      return 1;
   }
   catch(...) {
      std::cerr << "Error!" << std::endl;
      return 1;
   }
}
