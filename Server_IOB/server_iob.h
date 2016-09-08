#ifndef SERVER_IOB_H
#define SERVER_IOB_H
// standard includes
#include <QtWidgets/QMainWindow>
#include "ui_server_iob.h"
#include <QString>

// library for processing xml documents
#include <QtXml>
// librar for processing files
#include <QFile>
#include <QFileInfo>
// library for QDialog
#include <QMessageBox>
// library for network operations
#include <QtNetwork>

class Server_IOB : public QMainWindow
{
	Q_OBJECT

public:
	Server_IOB(QWidget *parent = 0);
	~Server_IOB();

private:
	Ui::Server_IOBClass ui;

	// member variables
	QString mFileName = "clientList.xml"; // client list name
	QDomDocument mClientList; // xml client document
	QTcpServer mTCPServer; // server
	QNetworkSession mNetworkSession;

private:
	// functions
	QDomDocument loadXMLDocument(QString);
};

#endif // SERVER_IOB_H
