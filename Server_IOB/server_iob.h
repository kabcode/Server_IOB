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
#include <QtWebSockets>
// include custom classes
#include "client.h"

//******************//
//   Server class   //
//******************//

class Server_IOB : public QMainWindow
{
	Q_OBJECT

public:
	explicit Server_IOB(QWidget *parent = 0);
	~Server_IOB();

	// message control
	enum MESSAGEID
	{
		REQUEST,
		REGISTRATION,
		UPDATE,
		TEST
	};

signals:
	void closed();
	
private slots:
	void onNewConnection();
	void processTextMessage(QString message);
	void processBinaryMessage(QByteArray message);
	void socketDisconnected();

private:
	// member variables
	QString				 mFileName = "knownClientList.xml"; // client list name
	QDomDocument		 mClientList; // xml client document
	
	// network variables
	QWebSocketServer    *mWebSocketServer;
	QList<Client* >		 mClients;
	QList<QWebSocket *>  mPendingSockets;

	// private functions
	QDomDocument loadXMLDocument(QString);
	void         setClientList(QDomDocument);
	void		 startServer();

	// UI variables
	Ui::Server_IOBClass ui;
};

#endif // SERVER_IOB_H
