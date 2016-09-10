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
#include <QtNetwork>;

class Server_IOB : public QMainWindow
{
	Q_OBJECT

public:
	explicit Server_IOB(QWidget *parent = 0);
	~Server_IOB();

public slots:
	void newConnection();

private:
	Ui::Server_IOBClass ui;

	// member variables
	QString              mFileName = "knownClientList.xml"; // client list name
	QDomDocument         mClientList; // xml client document
	QHash<int, QString>  mClientHash;
	QTcpServer          *mTCPServer; // server
	QNetworkSession     *mNetworkSession;

private:
	// functions
	QDomDocument loadXMLDocument(QString);
	void         setClientList(QDomDocument);
};

#endif // SERVER_IOB_H
