
#include "wsserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QNetworkInterface>
#include <QHostAddress>


QT_USE_NAMESPACE



WsServer::WsServer(quint16 port, QObject *parent) :
    QObject(parent),
	m_pWebSocketServer(new QWebSocketServer(QStringLiteral("vClickServer"),
                                            QWebSocketServer::NonSecureMode, this)),
	m_clients()
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "WsServer listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &WsServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WsServer::closed);
	}
	socket = new QUdpSocket(this); // is it safe to use just one socket?
	m_port = UDPPORT;

}



WsServer::~WsServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}


void WsServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WsServer::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WsServer::socketDisconnected);

	m_clients << pSocket;
	emit newConnection(m_clients.count()); //TODO: kui enamus OSC-ga, siis näita pigem, mitu OSC aadressi reas
}



void WsServer::processTextMessage(QString message)
{
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	if (!pClient) {
		return;
	}
	qDebug()<<message;

	QStringList messageParts = message.split(" ");

	if (messageParts[0]=="hello") { // or just anything, maybe

		QString senderUrl = pClient->peerAddress().toString();
		senderUrl.remove("::ffff:"); // if connected via websocket, this is added to beginning
		if (!senderUrl.isEmpty()) { // append to oscAddresses and send confirmation

			if (!udpAddresses.contains(senderUrl)) {
				udpAddresses<<senderUrl; // probably not necessary to keep that variable.
				pClient->sendTextMessage("Got you, "+senderUrl + "!\n");
				emit updateUdpAddresses(udpAddresses.join(","));
			} else {
				qDebug()<<"Adress already registered: "<<senderUrl;
				pClient->sendTextMessage("I have you already");
			}
		}
		//pClient->close(QWebSocketProtocol::CloseCodeNormal);
	}

}


void WsServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clients.removeAll(pClient);
        emit newConnection(m_clients.count());
        pClient->deleteLater();
	}
}


void WsServer::updateUdpAddresses(QString addresses) // split, add new;
{
	foreach (QString address, addresses.split(",")) {
        address = address.simplified();
		if (!address.isEmpty() && !udpAddresses.contains(address)) {
			udpAddresses.append(address);
			qDebug()<<"Added new address: "<<address;
		}
    }
	emit udpAddressesChanged(udpAddresses.join(","));
}




void WsServer::sendMessage(QWebSocket *socket, QString message )
{
    if (socket == 0)
    {
        return;
    }
    socket->sendTextMessage(message);

}


void WsServer::send2all(QString message)
{
	QByteArray data = message.toLocal8Bit().data();
	qDebug()<<"Send: "<<message;

	foreach (QString address, udpAddresses) {
		if (!address.isEmpty()) {
			qint64 retval=socket->writeDatagram(data, QHostAddress(address), m_port);
			qDebug()<<"IP: " << address << ", bytes sent: "<<retval;
		}
	}
}



void WsServer::setUdpAddresses(QString addresses)
{
	udpAddresses = addresses.split(",");

}

QString WsServer::getUdpAddresses()
{
	return udpAddresses.join(",");
}

QString WsServer::getLocalAddress()
{
	QString address = QString();
	QList <QHostAddress> localAddresses = QNetworkInterface::allAddresses();
	for(int i = 0; i < localAddresses.count(); i++) {

		if(!localAddresses[i].isLoopback())
			if (localAddresses[i].protocol() == QAbstractSocket::IPv4Protocol ) {
				address = localAddresses[i].toString();
				qDebug() << "YOUR IP: " << address;
				break; // get the first address
		}

	}
	return address;
}

void WsServer::setPort(quint16 port)
{
	m_port = port;
}



