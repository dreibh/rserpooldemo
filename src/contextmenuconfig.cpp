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

#include "contextmenuconfig.h"
#include <qmessagebox.h>
#include <qprocess.h>


QProcess *CContextMenuConfig::m_pProcess = NULL;


CContextMenuConfig::CContextMenuConfig(QString _Name, QString _CommandLine)
   : m_Name(_Name),
     m_CommandLine(_CommandLine)
{
}


void CContextMenuConfig::execute()
{
   if(m_pProcess) {
      QMessageBox::critical( 0, "Error!",
         "Former process has not been ended: " + m_CommandLine + "\nstderr:" + m_pProcess->readLineStderr() + "\nstdout:" + m_pProcess->readLineStdout());
      return;
   }

   QStringList commandList = QStringList::split(" ", QString("nice " + m_CommandLine));

   m_pProcess = new QProcess(commandList);
   connect(m_pProcess, SIGNAL(processExited()), this, SLOT(processFinished()));
   if(!m_pProcess->start()) {
      QMessageBox::critical( 0, "Error!",
         "Failed to start program: " + m_CommandLine + "\nstderr:" + m_pProcess->readLineStderr() + "\nstdout:" + m_pProcess->readLineStdout());
      delete m_pProcess;
   }
}


void CContextMenuConfig::processFinished()
{
   delete m_pProcess;
   m_pProcess = 0;
}
