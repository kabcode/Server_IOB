#include "server_iob.h"

//******************//
//   Server class   //
//******************//

// constructor
Server_IOB::Server_IOB(QWidget *parent)
	: QMainWindow(parent),
	mWebSocketServer(new QWebSocketServer(QStringLiteral("InOutBoard"), QWebSocketServer::NonSecureMode, this))
{
	mClients.clear();
	
	// load the XML document with the known clients
	mClientList = loadXMLDocument(mFileName);

	// parse the client list
	setClientList(mClientList, mClients);
	
	// start server
	this->startServer();
		
	// create UI
	ui.setupUi(this);
} // END constructor

// destructor
Server_IOB::~Server_IOB()
{
	mWebSocketServer->close();
	writeClientToXml();
	mClients.clear();
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

// create a QList from xml document
void Server_IOB::setClientList(QDomDocument mClientList, QList<Client*>& list)
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
			// create new client
			QString name = client.firstChildElement("name").text();
			QString location = client.firstChildElement("location").text();
			QString phone = client.firstChildElement("phone").text();
			QString notes = client.firstChildElement("notes").text();
			QDateTime lastUpdated = QDateTime::fromString(client.firstChildElement("updateTime").text(), "yyyy:MM:dd HH : mm:ss");

			QUuid uuid = id;
			Client* cl = new Client(uuid, name);
			cl->setStatus(Client::STATUS::ABSENT);
			cl->setLocation(location);
			cl->setPhone(phone);
			cl->setNotes(notes);
			cl->setLastUpdateDateTime(lastUpdated);
			
			cl->print();
			list.push_back(cl);
		}
		client = client.nextSibling();
	}
}// END setClientList

// set up server and listening ip and port
void Server_IOB::startServer()
{
	QHostAddress addr = QHostAddress::LocalHost;
	qint16 port = 9000;
	if(mWebSocketServer->listen(addr, port))
	{
		qDebug() << "InOutBoard Server listening on port" << port;
		connect(mWebSocketServer, &QWebSocketServer::newConnection, this, &Server_IOB::onNewConnection);
		connect(mWebSocketServer, &QWebSocketServer::closed, this, &Server_IOB::closed);
	}
	else
	{
		qDebug() << "Problem with starting server.";
	}
	
}

// todo
void Server_IOB::onNewConnection()
{
	QWebSocket *pSocket = mWebSocketServer->nextPendingConnection();

	connect(pSocket, &QWebSocket::textMessageReceived, this, &Server_IOB::processTextMessage);
	connect(pSocket, &QWebSocket::disconnected, this, &Server_IOB::socketDisconnected);

	mPendingSockets << pSocket;
}

// receiving messages, registration process is handled here
void Server_IOB::processTextMessage(QString telegram)
{
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	QStringList controls = telegram.split("#");
	QStringList::Iterator iter = controls.begin();
	/*
	for (iter; iter != controls.end(); ++iter)
	{
		qDebug() << *iter;
	}
	*/

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
			if (this->isKnownClient(uuid))
			{
				// client is known
				Client* cl = this->getClientFromQList(uuid);
				if (cl != Q_NULLPTR)
				{
					qDebug() << "Client: ";
					cl->setName(controls.at(2));
					cl->setStatus(controls.at(3).toInt());
					cl->setLocation(controls.at(4));
					cl->setPhone(controls.at(5));
					cl->setNotes(controls.at(6));
					cl->setLastUpdateDateTime();
					cl->setWebsocket(pClient);
					qDebug() << "Client is updated: " << uuid;
					cl->print();
				}
				else
				{
					qDebug() << "Nullpointer returned.";
				}
			}
			else
			{
				// unknown clients, add client to QList
				QUuid id = controls.at(1);
				Client* cl = new Client(id,controls.at(2));
				cl->setStatus(controls.at(3).toInt());
				cl->setLocation(controls.at(4));
				cl->setPhone(controls.at(5));
				cl->setNotes(controls.at(6));
				cl->setLastUpdateDateTime();
				cl->setWebsocket(pClient);

				mClients.push_back(cl);
				qDebug() << "Client is added to QList.";
				cl->print();
			}
			// give a answer that the registration succeded
			QString ack("Registration is done.");
			if (pClient) {
				pClient->sendTextMessage(ack);
			}
			
			// boadcast the update to all clients
		}
		else
		{
			qDebug() << "ID is not valid!";
			pClient->sendTextMessage(QString::number(MESSAGEID::REFUSAL).append("#ID is not correct."));
			// give a answer that the registration failedl
			QString ack("Your ID is not valid.");
			if (pClient) {
				pClient->sendTextMessage(ack);
				pClient->close();
			}
		}
	}
		break;
	case MESSAGEID::CLOSING:
	{
		Client* cl = getClientFromQList(controls.at(1));
		cl->closeWebSocket();
		cl->setLastUpdateDateTime();
		break;
	}
	default:
		qDebug() << "Unknown request!";
	}
}

// todo
void Server_IOB::socketDisconnected()
{
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	qDebug() << "socketDisconnected:" << pClient;
	if (pClient) {
		mPendingSockets.removeAll(pClient);
		pClient->deleteLater();
	}
}

// check if new registered client is known
bool Server_IOB::isKnownClient(QString uuid)
{
	QList<Client*>::ConstIterator cIter = mClients.constBegin();
	bool known = false;
	for (cIter; cIter != mClients.constEnd(); ++cIter)
	{
		// if the id is known, set bool to true and break
		if ((*cIter)->getUuid().toString() == uuid)
		{
			known = true;
			qDebug() << "ID is known: " << (*cIter)->getUuid();
			break;
		}
	}
	return known;
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

// write the currently known clients to xml file. It closes the websocket and delete the client
void Server_IOB::writeClientToXml()
{
	// open xml file
	QFile file(mFileName);
	if (!file.open(QFile::WriteOnly))
	{
		// error message
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Unable to write XML file!!");
		messageBox.setFixedSize(500, 200);
		return;
	}

	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartElement("clientList");

	// write all the clients to xml file
	QList<Client*>::ConstIterator cIter = mClients.constBegin();
	for (cIter; cIter != mClients.constEnd(); ++cIter)
	{
		writer.writeStartElement("client");
		writer.writeAttribute("id", (*cIter)->getUuid().toString());
		writer.writeTextElement("name", (*cIter)->getName());
		writer.writeTextElement("status", QString::number((*cIter)->getStatus()));
		writer.writeTextElement("location", (*cIter)->getLocation());
		writer.writeTextElement("phone", (*cIter)->getPhone());
		writer.writeTextElement("notes", (*cIter)->getNotes());
		writer.writeTextElement("updateTime", (*cIter)->getLastUpdateDateTime().toString());
		writer.writeEndElement();

		// close websocket and relase the client
		//(*cIter)->closeWebSocket();
		delete (*cIter);
	}

	writer.writeEndElement();
	file.close();
	qDebug() << "Wrote XML file:" << mFileName;
}

// get the pointer to the client from the client list
Client* Server_IOB::getClientFromQList(QString uuid)
{
	QList<Client*>::ConstIterator cIter = mClients.constBegin();
	
	for (cIter; cIter != mClients.constEnd(); ++cIter)
	{
		// if the id is known, set bool to true and break
		if ((*cIter)->getUuid().toString() == uuid)
		{
			return (*cIter);
		}
	}
	Client* noCl = Q_NULLPTR;
	return noCl;
}