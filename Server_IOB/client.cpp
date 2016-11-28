#include "client.h"


Client::Client(QUuid uuid, QString name)
	:
	name(name),
	uuid(uuid)
{
	status = STATUS::ABSENT;
	websocket = Q_NULLPTR;
}

Client::~Client()
{

}

 void Client::setName(QString name)		  {	this->name = name;}
 void Client::setLocation(QString location) { this->location = location; }
 void Client::setPhone(QString phone)		  { this->phone = phone; }
 void Client::setNotes(QString notes)		  { this->notes = notes; }
 void Client::setStatus(int status)		  { this->status = status; }
 void Client::setLastUpdateDateTime()		  {	this->lastUpdateDateTime = QDateTime::currentDateTime();}
 void Client::setLastUpdateDateTime(QDateTime dateTime) { this->lastUpdateDateTime = dateTime; }
 void Client::setWebsocket(QWebSocket* socket)
 {
	 if (!websocket)
	 {
		 this->websocket = socket;
	 }
	 else
	 {
		 websocket->disconnect();
		 this->websocket = socket;
	 }
 }

 QUuid	 Client::getUuid() { return uuid; }
 QString Client::getName() { return name; };
 int	 Client::getStatus() { return status; };
 QString Client::getLocation() { return location; };
 QString Client::getPhone() { return phone; };
 QString Client::getNotes() { return notes; };
 QDateTime Client::getLastUpdateDateTime() { return lastUpdateDateTime; };

 // reveal the clients infomation
 void Client::print()
 {
	 qDebug() <<"=================================================" ;
	 qDebug() << "ID: " << this->getUuid();
	 qDebug() << "Name: " << this->getName();
	 qDebug() << "Status: " << this->getStatus();
	 qDebug() << "Location: " << this->getLocation();
	 qDebug() << "Phone: " << this->getPhone();
	 qDebug() << "Notes: " << this->getNotes();
	 qDebug() << "Last updated: " << this->getLastUpdateDateTime().toString();
	 if (!websocket)
	 {
		 qDebug() << "Websocket: not connected";
	 }
	 else
	 {
		 qDebug() << "Websocket: " << this->websocket;
	 }
	 qDebug() << "=================================================";
 }

 // close websocket
 void Client::closeWebSocket()
 {
	 websocket->close();
 }

