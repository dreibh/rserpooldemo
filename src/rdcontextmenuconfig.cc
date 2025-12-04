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
 *   Copyright (C) 2002-2026 by Thomas Dreibholz
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

#include <QMessageBox>
#include <QCursor>
#include <iostream>

#include "rdcontextmenuconfig.h"


QProcess* RDContextMenuConfig::m_pProcess = NULL;


// ###### Constructor #######################################################
RDContextMenuConfig::RDContextMenuConfig(QWidget*       parent,
                                         const QString& nodeName,
                                         const QString& itemName,
                                         const QString& commandLine)
   : m_Parent(parent),
     m_NodeName(nodeName),
     m_ItemName(itemName),
     m_CommandLine(commandLine)
{
}


// ###### Destructor ########################################################
RDContextMenuConfig::~RDContextMenuConfig()
{
}


// ###### Split command line into arguments list ############################
QStringList RDContextMenuConfig::splitCommandLine(const QString& commandLine)
{
   QStringList                   list;
   QString                       argument;
   enum { Idle, Arg, QuotedArg } state  = Idle;
   bool                          escape = false;

   foreach (QChar const c, commandLine) {
      if(!escape && c == '\\') {
         escape = true; continue;
      }
      switch (state) {
         case Idle:
            if(!escape && c == '"') {
                state = QuotedArg;
            }
            else if(escape || !c.isSpace()) {
                argument += c;
                state = Arg;
            }
          break;
         case Arg:
            if(!escape && c == '"') {
               state = QuotedArg;
            }
            else if(escape || !c.isSpace()) {
               argument += c;
            }
            else {
               list << argument; argument.clear();
               state = Idle;
            }
          break;
         case QuotedArg:
            if(!escape && c == '"') {
               state = argument.isEmpty() ? Idle : Arg;
            }
            else {
               argument += c;
            }
            break;
       }
       escape = false;
    }
    if(!argument.isEmpty()) {
       list << argument;
    }
    return list;
}


void RDContextMenuConfig::execute()
{
   // ====== Ensure that no other process is running ========================
   if(m_pProcess) {
      QMessageBox::critical(0, "Error!",
         "Another command is still running:\n" +
         m_CommandLine +
         "\nSee console output for error messages!");
      return;
   }

   // ====== Get command call ===============================================
   QStringList arguments = splitCommandLine(m_CommandLine);
   QString     command  = arguments.takeFirst();

   QStringList::Iterator iterator = arguments.begin();
   std::cout << "Command> " << command.toStdString() << "\n";
   unsigned int n = 1;
   while(iterator != arguments.end()) {
       std::cout <<  " - A" << n << "=" << iterator->toStdString() << "\n";
       iterator++; n++;
   }
   std::cout << std::endl;

   // ====== Start process ==================================================
   m_Parent->setCursor(Qt::WaitCursor);
   m_pProcess = new QProcess(this);
   connect(m_pProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
   connect(m_pProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdout()));
   connect(m_pProcess, SIGNAL(readyReadStandardError()),  this, SLOT(readStderr()));
   m_pProcess->start(command, arguments);

   // ====== Wait for finish ================================================
   if(!m_pProcess->waitForFinished(30000)) {
      QMessageBox::critical(0, "Error!",
         "Process has not finished, yet:\n" +
         m_CommandLine +
         "\nSee console output for error messages!");
   }
   if(m_pProcess)  {
      delete m_pProcess;
      m_pProcess = NULL;
      m_Parent->setCursor(Qt::ArrowCursor);
   }
}


// ###### Print stdout ######################################################
void RDContextMenuConfig::readStdout()
{
   std::cout << "stdout " << m_NodeName.toStdString() << "> "
             << m_pProcess->readAllStandardOutput().toStdString() << std::endl;
}


// ###### Print stderr ######################################################
void RDContextMenuConfig::readStderr()
{
   std::cerr << "stderr " << m_NodeName.toStdString()<< "> "
             << m_pProcess->readAllStandardError().toStdString() << std::endl;
}


// ###### Handle exit code ##################################################
void RDContextMenuConfig::processFinished(int exitCode, QProcess::ExitStatus)
{
   if(m_pProcess->exitStatus() != 0) {
      QMessageBox::critical(0, "Error!",
         "Command failed with exit code " + QString::number(exitCode) + ":\n" +
         m_CommandLine +
         "\nSee console output for error messages!");
   }
}
