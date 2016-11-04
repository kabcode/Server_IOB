#include "server_iob.h"

//******************//
//   Server class   //
//******************//

// constructor
Server_IOB::Server_IOB(QWidget *parent)
	: QMainWindow(parent),
	mTcpServer(new QTcpServer(this)),
	mNetworkSession(Q_NULLPTR)
{
	// load the XML document with the known clients
	mClientList = loadXMLDocument(mFileName);

	// parse the client list
	setClientList(mClientList);

	// start network service and listening
	QNetworkConfigurationManager manager;
	QNetworkConfiguration config = manager.defaultConfiguration();
	mNetworkSession = new QNetworkSession(config, this);
	connect(mNetworkSession, &QNetworkSession::opened, this, &Server_IOB::sessionOpened);
	mNetworkSession->open();
	//sessionOpened();

	// the way incomming connections will be handled
	connect(mTcpServer, &QTcpServer::newConnection, this, &Server_IOB::sendGreetings);
			
	// create UI
	ui.setupUi(this);
} // END constructor

// destructor
Server_IOB::~Server_IOB()
{
} // END destructor

// load the XML document
QDomDocument Server_IOB::loadXMLDocument(QString fileName)
{
	QDomDocument clientList(fileName);

	// check if client list exist
	QFileInfo checkFile(fileName);
	if (!checkFile.exists() && !checkFile.isFile())
	{
		// create a new xml document
		QFile file(mFileName);
		if (file.open(QIODevice::WriteOnly))
		{
			// create the base structure for xml file
			QXmlStreamWriter xmlWriter(&file);
			xmlWriter.setAutoFormatting(true);
			xmlWriter.writeStartDocument();
			xmlWriter.writeStartElement("clientList");
			xmlWriter.writeEndElement();
			xmlWriter.writeEndDocument();
			file.close();
		}
		// show error message box and exit the app
		else
		{
			QMessageBox messageBox;
			messageBox.critical(0, "Error", "Not allowed to create file!");
			messageBox.setFixedSize(500, 200);
			QApplication::exit(EXIT_FAILURE);
		}
	}

	// load the xml file as QDomDocument
	QFile xmlFile(fileName);
	if (!xmlFile.open(QIODevice::ReadOnly))
	{
		xmlFile.close();
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Unable to read XML file!");
		messageBox.setFixedSize(500, 200);
		QApplication::exit(EXIT_FAILURE);
	}
	if (!clientList.setContent(&xmlFile))
	{
		xmlFile.close();
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "File contains wrong content!");
		messageBox.setFixedSize(500, 200);
		QApplication::exit(EXIT_FAILURE);
	}

	// return QDomDocument
	return clientList;
} // END loadXMLDocument

// create a hash map from xml document
void Server_IOB::setClientList(QDomDocument mClientList)
{
	QDomElement root = mClientList.firstChildElement("clientList");
	// if there are no clients in the list return
	if (!root.hasChildNodes())
	{
		return;
	}
	// traverse the sibling nodes to get the clients
	QDomNode child =  mClientList.firstChild();
	while (!child.isNull())
	{
		QDomElement item = child.firstChildElement("id");
		int clientId = item.text().toInt();
		item = item.nextSiblingElement();
		QString clientName = item.text();
		qDebug() << "Client ID: " << clientId;
		qDebug() << "Client name: " << clientName;
		child = child.nextSibling();
		mClientHash.insert(clientId,clientName);
	}
}// END setClientList

// what to do with an incomming connection
void Server_IOB::sendGreetings()
{
	qDebug() << "recieved new connection";
	
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);

	out.setVersion(QDataStream::Qt_5_7);
	QString response("Hello Client!");
	out << response;	

	QTcpSocket *clientConnection = mTcpServer->nextPendingConnection();
	connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QAbstractSocket::deleteLater);
	clientConnection->write(block);
	clientConnection->flush();
	qDebug() << "Wrote message";
	clientConnection->disconnectFromHost();

}// END sendGreetings

// opening a server client session
void Server_IOB::sessionOpened()
{
	mTcpServer = new QTcpServer(this);
	if (!mTcpServer->listen(QHostAddress::LocalHost, 9000)) {
		QMessageBox::critical(this, tr("IOB Server"),
			tr("Unable to start the server: %1.")
			.arg(mTcpServer->errorString()));
		close();
		return;
	}
	QString ipAddress;
	ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
	qDebug() << "The server is running on\nIP: " << ipAddress << "\nPort: " << mTcpServer->serverPort();

}// END sessionOpened