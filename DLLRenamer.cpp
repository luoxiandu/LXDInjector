#include "DLLRenamer.h"
extern QString getRandomString(int length, bool use_symbols);
DLLRenamer::DLLRenamer()
	: QObject()
{
	mem.setKey("DLLStringList");
	mem.create(81920);
	mem.attach();
	this->renameTimer = new QTimer(this);
	connect(this->renameTimer, SIGNAL(timeout()), this, SLOT(dorename()));
	this->renameTimer->start(200);
}

DLLRenamer::~DLLRenamer()
{
	VM_START
	QStringList::iterator i = this->paths.begin();
	while (i != this->paths.end())
	{
		QFile dll(*i);
		dll.open(QIODevice::Truncate);
		dll.write(QByteArray("fuck"));
		dll.close();
		while(!dll.remove());
		i++;
	}
	VM_END
}

void DLLRenamer::dorename()
{
	VM_START
	int crackcount = 0, CheckVar2;
	CHECK_CODE_INTEGRITY(CheckVar2, 0x1AB896E5)
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
	mem.lock();
	qDebug() << mem.errorString();
	tomem = (QStringList *)mem.data();
	this->paths = newpaths;
	memcpy(tomem, &newpaths, newpaths.size() * sizeof(QString));
	mem.unlock();
	if (CheckVar2 != 0x1AB896E5)
	{
		if (++crackcount == 5)
		{
			qApp->quit();
		}
	}
	VM_END
}

void DLLRenamer::setpath(QString path)
{
	this->paths << path;
}
