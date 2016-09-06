#ifndef SERVER_IOB_H
#define SERVER_IOB_H

#include <QtWidgets/QMainWindow>
#include "ui_server_iob.h"

class Server_IOB : public QMainWindow
{
	Q_OBJECT

public:
	Server_IOB(QWidget *parent = 0);
	~Server_IOB();

private:
	Ui::Server_IOBClass ui;
};

#endif // SERVER_IOB_H
