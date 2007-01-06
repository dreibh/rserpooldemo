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

#include <qmessagebox.h>
#include <qprocess.h>
#include <iostream>

#include "contextmenuconfig.h"


QProcess* CContextMenuConfig::m_pProcess = NULL;


CContextMenuConfig::CContextMenuConfig(const QString& nodeName,
                                       const QString& itemName,
                                       const QString& commandLine)
   : m_NodeName(nodeName),
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

   QString commandLine = QString("nice " + m_CommandLine);
   m_pProcess = new QProcess(this);

   bool inBlock = FALSE;
   while(commandLine != "") {

      int pos;
      pos=commandLine.find("\"");
      if(!inBlock) {
         QString part;
         if(pos >= 0) {
            inBlock = TRUE;
            part = commandLine.left(pos).stripWhiteSpace();
            commandLine = commandLine.mid(pos + 1);
         }
         else {
            part = commandLine.stripWhiteSpace();
            commandLine = "";
         }

         int sectionNumber = 0;
         QString section = part.section(' ', sectionNumber, sectionNumber,
                                        QString::SectionSkipEmpty);
         while(!section.isNull()) {
            m_pProcess->addArgument(section.stripWhiteSpace());

            sectionNumber++;
            section = part.section(' ', sectionNumber, sectionNumber,
                                   QString::SectionSkipEmpty);
         }
      }
      else {
         inBlock = FALSE;
         const QString section = commandLine.left(pos);
         m_pProcess->addArgument("\"" + section.stripWhiteSpace() + "\"");
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
       std::cout << *iterator << " ";
       ++iterator;
   }
   std::cout << std::endl;

   connect(m_pProcess, SIGNAL(processExited()), this, SLOT(processFinished()));
   connect(m_pProcess, SIGNAL(readyReadStdout()), this, SLOT(readStdout()));
   connect(m_pProcess, SIGNAL(readyReadStderr()), this, SLOT(readStderr()));
   if(!m_pProcess->start()) {
      QMessageBox::critical(0, "Error!",
         "Failed to run command:\n" +
         m_CommandLine +
         "\nSee console output for error messages!");
      delete m_pProcess;
      m_pProcess = NULL;
   }
}


void CContextMenuConfig::readStdout()
{
   std::cout << "stdout " << m_NodeName << "> " << m_pProcess->readLineStdout() << std::endl;
}


void CContextMenuConfig::readStderr()
{
   std::cerr << "stderr " << m_NodeName << "> " << m_pProcess->readLineStderr() << std::endl;
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
}
