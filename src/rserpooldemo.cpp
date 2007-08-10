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

#include <qapplication.h>
#include <qstring.h>
#include <iostream>

#include "mainwidget.h"


int main(int argc, char** argv)
{
   try {
      const QString configFileTag = "-config=";
      QString configFile;
      for(int i = 1;i < argc;i++) {
         QString command = argv[i];
         if(command.find(configFileTag) == 0) {
            configFile = command.mid(configFileTag.length());
         }
      }

      QApplication application(argc, argv);
      CMainWidget* mainWidget;
      if(configFile.length() > 0) {
         mainWidget = new CMainWidget(configFile);
      }
      else {
         mainWidget = new CMainWidget();
      }
      application.setMainWidget( mainWidget );
      mainWidget->show();

      return application.exec();
   }
   catch(ELoadFileException& e) {
      printf("Unable to load config file!\n");
      return 1;
   }
   catch(EXMLSyntaxException& e) {
      printf("Unable to load config file!\nError: %s\nIn line: %u\n",
             e.m_Message.ascii(), e.m_Line);
      return 1;
   }
   catch(...) {
      printf("Error!\n");
      return 1;
   }
}
