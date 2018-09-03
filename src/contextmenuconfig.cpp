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
 * ############# An Efficient RSerPool Prototype Implementation #############
 *
 *   Copyright (C) 2002-2017 by Thomas Dreibholz
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

#include <QMessageBox>
#include <QCursor>
#include <iostream>

#include "contextmenuconfig.h"


QProcess* CContextMenuConfig::m_pProcess = NULL;


CContextMenuConfig::CContextMenuConfig(QWidget*       parent,
                                       const QString& nodeName,
                                       const QString& itemName,
                                       const QString& commandLine)
   : m_Parent(parent),
     m_NodeName(nodeName),
     m_ItemName(itemName),
     m_CommandLine(commandLine)
{
}


CContextMenuConfig::~CContextMenuConfig()
{
}


void CContextMenuConfig::execute()
{
   if(m_pProcess) {
      QMessageBox::critical(0, "Error!",
         "Another command is still running:\n" +
         m_CommandLine +
         "\nSee console output for error messages!");
      return;
   }

   m_Parent->setCursor(Qt::WaitCursor);

   QString commandLine = QString("nice " + m_CommandLine);
   m_pProcess = new QProcess(this);

   QStringList arguments;
   bool inBlock = false;
   while(commandLine != "") {

      const int pos = commandLine.indexOf("\"");
      if(!inBlock) {
         QString part;
         if(pos >= 0) {
            inBlock = true;
            part = commandLine.left(pos).trimmed();
            commandLine = commandLine.mid(pos + 1);
         }
         else {
            part = commandLine.trimmed();
            commandLine = "";
         }

         int sectionNumber = 0;
         QString section = part.section(' ', sectionNumber, sectionNumber,
                                        QString::SectionSkipEmpty);
         while(!section.isNull()) {
            arguments.append(section.trimmed());

            sectionNumber++;
            section = part.section(' ', sectionNumber, sectionNumber,
                                   QString::SectionSkipEmpty);
         }
      }
      else {
         inBlock = false;
         const QString section = commandLine.left(pos);
         arguments.append("\"" + section.trimmed() + "\"");
         commandLine = commandLine.mid(pos + 1);
         if(pos < 0) {
            commandLine = "";   // Missing last "
         }
      }
   }

   QStringList commandList = m_pProcess->arguments();
   QStringList::Iterator iterator = commandList.begin();
   std::cout << "Command> ";
   while(iterator != commandList.end()) {
       std::cout << (*iterator).toLocal8Bit().constData() << " ";
       ++iterator;
   }
   std::cout << std::endl;

   m_pProcess->setArguments(arguments);
   connect(m_pProcess, SIGNAL(processExited()), this, SLOT(processFinished()));
   connect(m_pProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdout()));
   connect(m_pProcess, SIGNAL(readyReadStandardError()),  this, SLOT(readStderr()));

   m_pProcess->start();
   if(!m_pProcess->waitForStarted(10000)) {
      QMessageBox::critical(0, "Error!",
         "Failed to run command:\n" +
         m_CommandLine +
         "\nSee console output for error messages!");
      delete m_pProcess;
      m_pProcess = NULL;
      m_Parent->setCursor(Qt::ArrowCursor);
   }
}


void CContextMenuConfig::readStdout()
{
   std::cout << "stdout " << m_NodeName.toLocal8Bit().constData() << "> " << m_pProcess->readAllStandardOutput().toStdString() << std::endl;
}


void CContextMenuConfig::readStderr()
{
   std::cerr << "stderr " << m_NodeName.toLocal8Bit().constData() << "> " << m_pProcess->readAllStandardError().toStdString() << std::endl;
}


void CContextMenuConfig::processFinished()
{
   if(m_pProcess->exitStatus() != 0) {
      QMessageBox::critical(0, "Error!",
         "Command execution failed:\n" +
         m_CommandLine +
         "\nSee console output for error messages!");
   }
   delete m_pProcess;
   m_pProcess = NULL;
   m_Parent->setCursor(Qt::ArrowCursor);
}
