/* $Id$ */
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

#include <qapplication.h>
#include <qstring.h>
#include <iostream>

#include "mainwidget.h"


int main(int argc, char** argv)
{
   try {
      const QString configFileTag = "-config=";
      QString configFile;
      const QString ttag = "-t=";
      QString t;
      for(int i = 1;i < argc;i++) {
         QString command = argv[i];
         if(command.find(configFileTag) == 0) {
            configFile = command.mid(configFileTag.length());
         }
         else if(command.find(ttag) == 0) {
            t = command.mid(ttag.length());
         }
      }

//       QProcess m_pProcess;
//       QString commandLine = configFile;
//
//       bool inBlock = FALSE;
//       while(commandLine != "") {
//
//          int pos;
//          pos=commandLine.find("\"");
//          if(!inBlock) {
//             QString part;
//             if(pos >= 0) {
//                inBlock = TRUE;
//                part = commandLine.left(pos).stripWhiteSpace();
//                commandLine = commandLine.mid(pos + 1);
//             }
//             else {
//                part = commandLine.stripWhiteSpace();
//                commandLine = "";
//             }
//
//             int sectionNumber = 0;
//             QString section = part.section(' ', sectionNumber, sectionNumber,
//                                            QString::SectionSkipEmpty);
//             while(!section.isNull()) {
//                m_pProcess.addArgument(section.stripWhiteSpace());
//
//                sectionNumber++;
//                section = part.section(' ', sectionNumber, sectionNumber,
//                                       QString::SectionSkipEmpty);
//             }
//          }
//          else {
//             inBlock = FALSE;
//             const QString section = commandLine.left(pos);
//             m_pProcess.addArgument(section.stripWhiteSpace());
//             commandLine = commandLine.mid(pos + 1);
//             if(pos < 0) {
//                commandLine = "";   // Missing last "
//             }
//          }
//       }
//
//
//  QStringList list = m_pProcess.arguments();
//     QStringList::Iterator it = list.begin();
//     while( it != list.end() ) {
//         std::cout << ">>>>>> " << *it << "." << std::endl;
//         ++it;
//     }
//     exit(1);
//

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
