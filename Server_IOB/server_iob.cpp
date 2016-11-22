#include "server_iob.h"

//******************//
//   Server class   //
//******************//

// constructor
Server_IOB::Server_IOB(QWidget *parent)
	: QMainWindow(parent),
	mWebSocketServer(new QWebSocketServer(QStringLiteral("InOutBoard"), QWebSocketServer::NonSecureMode, this)),
	mClients()
{
	// load the XML document with the known clients
	mClientList = loadXMLDocument(mFileName);

	// parse the client list
	setClientList(mClientList);

	// start server
	this->startServer();
		
	// create UI
	ui.setupUi(this);
} // END constructor

// destructor
Server_IOB::~Server_IOB()
{
	mWebSocketServer->close();
	qDeleteAll(mClients.begin(), mClients.end());
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
	}
}// END setClientList

void Server_IOB::startServer()
{
	QHostAddress addr = QHostAddress::LocalHost;
	qint16 port = 9000;
	mWebSocketServer->listen(addr, port);
	qDebug() << "InOutBoard Server listening on port" << port;
	connect(mWebSocketServer, &QWebSocketServer::newConnection, this, &Server_IOB::onNewConnection);
	connect(mWebSocketServer, &QWebSocketServer::closed, this, &Server_IOB::closed);
}

void Server_IOB::onNewConnection()
{
	QWebSocket *pSocket = mWebSocketServer->nextPendingConnection();

	connect(pSocket, &QWebSocket::textMessageReceived, this, &Server_IOB::processTextMessage);
	connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Server_IOB::processBinaryMessage);
	connect(pSocket, &QWebSocket::disconnected, this, &Server_IOB::socketDisconnected);

	mPendingSockets << pSocket;
}

void Server_IOB::processTextMessage(QString telegram)
{
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	QStringList controls = telegram.split("#");
	QStringList::Iterator iter = controls.begin();
	for (iter; iter != controls.end(); ++iter)
	{
		qDebug() << *iter;
	}
	int control = controls.at(0).toInt();
	
	if (control == MESSAGEID::REGISTRATION)
	{
		qDebug() << "Registration requested!";
		if (controls.at(1) == QUuid::QUuid().toString())
		{
			qDebug() << "No ID!";
			// create new ID and send it to client
			QUuid quuid = QUuid::createUuid();
			QString uuid = quuid.toString();
			if (pClient) {
				pClient->sendTextMessage(uuid);
			}
		}
		else
		{
			qDebug() << "ID is ok!";
			// give a answer that the registration succeded
		}
	}

	
}

void Server_IOB::processBinaryMessage(QByteArray message)
{
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	qDebug() << "Binary Message received:" << message;
	if (pClient) {
		pClient->sendBinaryMessage(message);
	}
}

void Server_IOB::socketDisconnected()
{
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	qDebug() << "socketDisconnected:" << pClient;
	if (pClient) {
		mPendingSockets.removeAll(pClient);
		pClient->deleteLater();
	}
}