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
// library for networking
#include <QtNetwork>

//******************//
//   Server class   //
//******************//

class Server_IOB : public QMainWindow
{
	Q_OBJECT

public:
	explicit Server_IOB(QWidget *parent = 0);
	~Server_IOB();

private slots:
	void sessionOpened();
	void sendGreetings();

private:
	// member variables
	QString              mFileName = "knownClientList.xml"; // client list name
	QDomDocument         mClientList; // xml client document
	QHash<int, QString>  mClientHash;
	
	// network variables
	QTcpServer          *mTcpServer; // server
	QNetworkSession     *mNetworkSession;

	// network functions

	// private functions
	QDomDocument loadXMLDocument(QString);
	void         setClientList(QDomDocument);

	// UI variables
	Ui::Server_IOBClass ui;
};

#endif // SERVER_IOB_H
