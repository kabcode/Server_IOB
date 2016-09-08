#include "server_iob.h"


/********************/
//   Server class   //
/********************/


// Constructor
Server_IOB::Server_IOB(QWidget *parent)	: QMainWindow(parent),
mTCPServer(Q_NULLPTR),
mNetworkSession(0)
{
	// load the XML document with the known clients
	mClientList = loadXMLDocument(mFileName);

		
	// create UI
	ui.setupUi(this);
} // END constructor

// Destructor
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
		messageBox.critical(0, "Error", "Unable to read XML file!!");
		messageBox.setFixedSize(500, 200);
		QApplication::exit(EXIT_FAILURE);
	}
	if (!clientList.setContent(&xmlFile))
	{
		xmlFile.close();
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "File contains wrong contaent!");
		messageBox.setFixedSize(500, 200);
		QApplication::exit(EXIT_FAILURE);
	}

	// return QDomDocument
	return clientList;
} // END loadXMLDocument
