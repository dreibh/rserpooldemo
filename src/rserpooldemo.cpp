#include <qapplication.h>
#include <qstring.h>
#include "mainwidget.h"



int main( int argc, char **argv )
{
	try
	{
		QString ConfigFile;
		QString ConfigFileTag = "-config=";
		for(int i = 0; i < argc; ++i)
		{
			QString Command = argv[i];
			if(Command.find(ConfigFileTag) == 0)
			{
				ConfigFile = Command.mid(ConfigFileTag.length());
			}
		}
		QApplication app( argc, argv );

		CMainWidget *mw;
		if (ConfigFile.length() > 0)
			mw = new CMainWidget( ConfigFile );
		else
			mw = new CMainWidget(  );
		app.setMainWidget( mw );
        	mw->show();

        	return app.exec();
	}
	catch(ELoadFileException& e)
	{
		printf("Unable to load config file!\n");
		return 1;
	}
	catch(EXMLSyntaxException& e)
	{
		printf("Unable to load config file!\nError: %s\nIn line: %u\n", e.m_Message.ascii(), e.m_Line);
		return 1;
	}
	catch(...)
	{
		printf("Error!\n");
		return 1;
	}
}
