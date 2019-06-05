#include "DLLRenamer.h"
extern QString getRandomString(int length, bool use_symbols);
DLLRenamer::DLLRenamer()
	: QObject()
{
	this->renameTimer = new QTimer(this);
	connect(this->renameTimer, SIGNAL(timeout()), this, SLOT(dorename()));
	this->renameTimer->start(200);
}

DLLRenamer::~DLLRenamer()
{
	QStringList::iterator i = this->paths.begin();
	while (i != this->paths.end())
	{
		QFile dll(*i);
		dll.remove();
		i++;
	}
}

void DLLRenamer::dorename()
{
	QStringList newpaths;
	QStringList::iterator i = this->paths.begin();
	while (i != this->paths.end())
	{
		if (!((*i).isEmpty()))
		{
			QFile dll(*i);
			if (dll.remove())
			{
				;
			}
			else
			{
				QString newdllname = QDir::tempPath() + "/" + getRandomString((rand() % 10) + 20, true);
				if (dll.rename(newdllname))
				{
					newpaths << newdllname;
				}
				else
				{
					newpaths << *i;
				}
			}
		}
		i++;
	}
	this->paths = newpaths;
}

void DLLRenamer::setpath(QString path)
{
	this->paths << path;
}
