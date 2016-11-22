#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QUuid>
#include <QDateTime>
#include <QWebsocket>

class Client : public QObject
{
	Q_OBJECT

public:
	Client(QObject *parent);
	~Client();

	enum STATUS
	{
		AVAILABE,
		BUSY,
		ABSENT
	};

private:
	QUuid uuid;
	QString name;
	int		status;
	QString location;
	QString phone;
	QString notes;

	QWebSocket* websocket;
	QDateTime lastUpdateDateTime;

	// getter and setter functions
	void Client::setName(QString);
	void Client::setStatus(int);
	void Client::setLocation(QString);
	void Client::setPhone(QString);
	void Client::setNotes(QString);
	void Client::setLastUpdateDateTime();
	void Client::setWebsocket(QWebSocket);

	QUuid	Client::getUuid();
	QString Client::getName();
	int		Client::getStatus();
	QString	Client::getLocation();
	QString	Client::getPhone();
	QString	Client::getNotes();
	QDateTime Client::getLastUpdateDateTime();
};

#endif // CLIENT_H
