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
	Client(QUuid, QString);
	~Client();

	//Client(const Client&) = default;
//	Client(Client&&) = default;

	enum STATUS
	{
		AVAILABE,
		BUSY,
		ABSENT
	};

	// getter and setter functions
	void setName(QString);
	void setStatus(int);
	void setLocation(QString);
	void setPhone(QString);
	void setNotes(QString);
	void setLastUpdateDateTime();
	void setLastUpdateDateTime(QDateTime);
	void setWebsocket(QWebSocket*);
	void closeWebSocket();

	QUuid		getUuid();
	QString		getName();
	int			getStatus();
	QString		getLocation();
	QString		getPhone();
	QString		getNotes();
	QDateTime   getLastUpdateDateTime();
	QWebSocket* getWebsocket();

	void Client::print();

private:
	QUuid uuid;
	QString name;
	int		status;
	QString location;
	QString phone;
	QString notes;

	QWebSocket* websocket;
	QDateTime lastUpdateDateTime;
	
};

#endif // CLIENT_H
