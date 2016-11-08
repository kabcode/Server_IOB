#include "server_iob.h"

//******************//
//   Server class   //
//******************//

// constructor
Server_IOB::Server_IOB(QWidget *parent)
	: QMainWindow(parent),
	mTcpServer(new QTcpServer(this))
{
	// load the XML document with the known clients
	mClientList = loadXMLDocument(mFileName);

	// parse the client list
	setClientList(mClientList);

	// the way incomming connections will be handled
	this->startServer();
			
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

// start th server and get it listening
void Server_IOB::startServer()
{
	// server listens at 'localhost', port 9000
	QHostAddress addr = QHostAddress::LocalHost;
	qint16 port = 9000;

	if (!mTcpServer->listen(addr, port))
	{
		qDebug() << "Server couldn't be started!";
	}
	else
	{
		qDebug() << "Server is running at: " << addr << ", Port: " << port;
		connect(mTcpServer, &QTcpServer::newConnection, this, &Server_IOB::newConnection);


	}
}

// how to handle a new connection
void Server_IOB::newConnection()
{
	// open up a socket for client communication
	mTcpSocket = mTcpServer->nextPendingConnection();
	connect(mTcpSocket, &QTcpSocket::disconnected, this, &Server_IOB::disconnected);;
	connect(mTcpSocket, &QTcpSocket::bytesWritten, this, &Server_IOB::bytesWritten);
	connect(mTcpSocket, &QTcpSocket::readyRead, this, &Server_IOB::readyRead);

}

void Server_IOB::connected()
{
	qDebug() << "Connected!";
}
void Server_IOB::disconnected()
{
	qDebug() << "Disconnected!";
	mTcpSocket->close();
}
void Server_IOB::bytesWritten(qint64 bytes)
{
	qDebug() << "Wrote" << bytes << "Bytes.";
}
void Server_IOB::readyRead()
{
	qDebug() << "Reading...";
	QDataStream clientReadStream(mTcpSocket);
	QString message;

	clientReadStream >> message;
	qDebug() << message;

	// send test bytes to the client
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);

	out.setVersion(QDataStream::Qt_5_7);
	QString hello("Hello Cient!");
	out << hello;

	mTcpSocket->write(block);
	mTcpSocket->flush();
	qDebug() << "Message sent";

	
}
