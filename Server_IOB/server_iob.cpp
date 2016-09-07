#include "server_iob.h"
// library for processing xml documents
#include <QtXml>
// librar for processing files
#include <QFile>
#include <QFileInfo>
// library for QDialog
#include <QMessageBox>

/********************/
//   Server class   //
/********************/
// Member variables
QString mFileName = "clientList.xml"; // client list
QDomDocument mClientList;


// Constructor
Server_IOB::Server_IOB(QWidget *parent)	: QMainWindow(parent)
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
QDomDocument Server_IOB::loadXMLDocument(QString file)
{
	// check if client list exist
	QFileInfo checkFile(file);
	if (!checkFile.exists() && !checkFile.isFile())
	{
		// create a new xml document
		QFile file(mFileName);
		if (file.open(QIODevice::WriteOnly))
		{
			file.close();
		}
		else
		{
			// show error message box
			QMessageBox messageBox;
			messageBox.critical(0, "Error", "Not allowed to create file!");
			messageBox.setFixedSize(500, 200);
		}
	}
	

} // END loadXMLDocument
