#include "client.h"


Client::Client(QObject *parent)
	: QObject(parent),
	name(""),
	location(""),
	phone(""),
	notes("")
{
	uuid = QUuid::createUuid();
	status = STATUS::ABSENT;
}

Client::~Client()
{
}

inline void Client::setName(QString name)		  {	this->name = name;}
inline void Client::setLocation(QString location) { this->location = location; }
inline void Client::setPhone(QString phone)		  { this->phone = phone; }
inline void Client::setNotes(QString notes)		  { this->notes = notes; }
inline void Client::setStatus(int status)		  { this->status = status; }
inline void Client::setLastUpdateDateTime()		  {	this->lastUpdateDateTime = QDateTime::currentDateTime();}
inline void Client::setWebsocket(QWebSocket socket) { this->websocket = &socket;  }

inline QUuid	 Client::getUuid() { return uuid; }
inline QString   Client::getName() { return name; };
inline int		 Client::getStatus() { return status; };
inline QString   Client::getLocation() { return location; };
inline QString   Client::getPhone() { return phone; };
inline QString	 Client::getNotes() { return notes; };
inline QDateTime Client::getLastUpdateDateTime() { return lastUpdateDateTime; };

