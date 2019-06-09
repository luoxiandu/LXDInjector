#include <QString>
#include <QDateTime>

QString getRandomString(int length, bool use_symbols)
{
	qsrand(QDateTime::currentMSecsSinceEpoch());//Ϊ���ֵ�趨һ��seed

	const char* chrs;
	if (!use_symbols)
		chrs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	else
		chrs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&()_+-=[]{}";
	int chrs_size = strlen(chrs);

	char* ch = new char[length + 1];
	memset(ch, 0, length + 1);
	int randomx = 0;
	for (int i = 0; i < length; ++i)
	{
		randomx = rand() % (chrs_size - 1);
		ch[i] = chrs[randomx];
	}

	QString ret(ch);
	delete[] ch;
	return ret;
}