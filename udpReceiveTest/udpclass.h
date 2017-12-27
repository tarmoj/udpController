#ifndef UDPCLASS_H
#define UDPCLASS_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

#define RECEIVE_PORT 60606 // hardcoded for now

class UdpClass : public QObject
{
    Q_OBJECT
public:
    explicit UdpClass(QObject *parent = NULL);
    ~UdpClass();

	Q_INVOKABLE void setPort(quint16 port);

	Q_INVOKABLE QString getLocalAddress();
signals:
	void newMessage(QString message);

private:

	quint16 m_port;
	QUdpSocket * m_socket;

private slots:
	void readPendingDatagrams();

};

#endif // UDPCLASS_H
