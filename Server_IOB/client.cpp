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
 QDateTime Client::getLastUpdateDateTime() { return lastUpdateDateTime; }
 QWebSocket* Client::getWebsocket() { return websocket; };

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

	 typedef QAbstractSocket::SocketState StateType;
	 if (websocket == Q_NULLPTR)
	 {
		 qDebug() << "Websocket: not availiable";
	 }
	 else
	 {
		 qDebug() << "Websocket: " << this->websocket;
		 StateType state = websocket->state();
		 switch (state)
		 {
		 case StateType::ConnectedState:
			 qDebug() << "Websocket: is connected."; break;
		 case StateType::BoundState:
			 qDebug() << "Websocket: is bound."; break;
		 case StateType::UnconnectedState:
			 qDebug() << "Websocket: is not connected."; break;
		 case StateType::ListeningState:
			 qDebug() << "Websocket: is listening."; break;
		 default:
			 qDebug() << "Websocket: something happened. I dont know for sure.";;
		 }
	 }
	 
	 qDebug() << "=================================================";
 }

 // close websocket
 void Client::closeWebSocket()
 {
	 websocket->close();
	 this->setWebsocket(Q_NULLPTR);
 }

