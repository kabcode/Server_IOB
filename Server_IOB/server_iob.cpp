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
	QDomNode client =  root.firstChildElement("client");
	while (!client.isNull())
	{
		QString id = client.toElement().attribute("id");
		if (isValidQUuid(id))
		{
			QString name     = client.firstChildElement("name").text();
			QString status   = client.firstChildElement("status").text();
			QString location = client.firstChildElement("location").text();
			QString phone    = client.firstChildElement("phone").text();
			QString notes    = client.firstChildElement("notes").text();
			QDateTime lastUpdated = QDateTime::fromString(client.firstChildElement("updateTime").text(),"yyyy:MM:dd HH : mm:ss");
			qDebug() << name;
			qDebug() << status;
			qDebug() << location;
			qDebug() << phone;
			qDebug() << notes;
			qDebug() << lastUpdated.toString();
		}
		client = client.nextSibling();
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

	switch (control)
	{
	case MESSAGEID::REGISTRATION:
	{
		qDebug() << "Registration requested!";
		// check id provided id is valid
		QString uuid = controls.at(1);
		if (uuid != QUuid::QUuid().toString() && isValidQUuid(uuid))
		{
			qDebug() << "ID is valid!";
			if (!this->isClient(uuid))
			{
				// client is known
			}
			else
			{
				// unknown clients
			}
			QString ack("Registration is done.");
			if (pClient) {
				pClient->sendTextMessage(ack);
			}
		}
		else
		{
			qDebug() << "ID is not valid!";
			pClient->sendTextMessage(QString::number(MESSAGEID::REFUSAL).append("#ID is not correct."));
			// give a answer that the registration succeded
		}
	}
		break;
	default:
		qDebug() << "Unknown request!";
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

// check if new client is known
bool Server_IOB::isClient(QString uuid)
{
	QList<Client*>::ConstIterator cIter = mClients.constBegin();
	for (cIter; cIter != mClients.constEnd(); ++cIter)
	{
		
	}
	return true;
}

// check a QUuid for validity (check length & symbols)
bool Server_IOB::isValidQUuid(QString uuid)
{
	bool valid = true;
	uuid.replace(QRegularExpression("[{}]"), "");
	QRegularExpression hexMatcher("^[0-9a-f\-]{36}$");
	QRegularExpressionMatch match = hexMatcher.match(uuid);
	if (!match.hasMatch()) { valid = false; }
	QStringList t = uuid.split("-");
	if (t.at(0).length() != 8) { valid = false; }
	if (t.at(1).length() != 4) { valid = false; }
	if (t.at(2).length() != 4) { valid = false; }
	if (t.at(3).length() != 4) { valid = false; }
	if (t.at(4).length() != 12) { valid = false; }

	return valid;
}