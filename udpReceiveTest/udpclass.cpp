#include "udpclass.h"
#include <QNetworkInterface>
//#include <QNetworkDatagram> // OK from  Qt 5.9
#include <QThread>


UdpClass::UdpClass(QObject *parent)
{

	m_port = 7007;
	m_socket  = new QUdpSocket(this);
	// receiving
	m_socket->bind(QHostAddress::Any, m_port );

	connect(m_socket, SIGNAL(readyRead()),
			this, SLOT(readPendingDatagrams()));

}

UdpClass::~UdpClass() {
	m_socket->close();
}

void UdpClass::setPort(quint16 port) // TODO: test this? If the old port is released? do we need to open/close/recreate the socket?
{
	m_port = port;
	if (m_socket->isOpen()) {
		m_socket->close();
	}
	m_socket->bind(QHostAddress::Any, m_port ); // DOES NOT WORK -  not closed so quickly
}


// to test you can do in bash: echo -n "hello2" >/dev/udp/127.0.0.1/60606
void UdpClass::readPendingDatagrams()
{
	while (m_socket->hasPendingDatagrams()) {
		QByteArray buffer; // for older Qt
		buffer.resize(m_socket->pendingDatagramSize());
		m_socket->readDatagram(buffer.data(), buffer.size());
		QString response = QString(buffer);

		qDebug()<<"Recieved " << response;
		emit newMessage(response);

	}
}

QString UdpClass::getLocalAddress()
{
	QString address = QString();
	QList <QHostAddress> localAddresses = QNetworkInterface::allAddresses();
	for(int i = 0; i < localAddresses.count(); i++) {

		if(!localAddresses[i].isLoopback())
			if (localAddresses[i].protocol() == QAbstractSocket::IPv4Protocol ) {
				address = localAddresses[i].toString();
				qDebug() << "YOUR IP: " << address;
				break; // get the first address (avoid bridges etc)

		}

	}
	return address;
}
