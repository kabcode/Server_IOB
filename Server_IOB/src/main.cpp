#include "server_iob.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Server_IOB w;
	w.show();
	return a.exec();
}
