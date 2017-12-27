#ifndef WSSERVER_H
#define WSSERVER_H

#include <QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtCore/QHash>
#include <QStringList>
#include <QSettings>

#include <QUdpSocket>
#include <QHostAddress>


QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)


#define UDPPORT 20102

class WsServer : public QObject
{
    Q_OBJECT
public:
    explicit WsServer(quint16 port, QObject *parent = NULL);
    ~WsServer();

	void sendMessage(QWebSocket *socket, QString message);
	Q_INVOKABLE void send2all(QString message);
	Q_INVOKABLE void setUdpAddresses(QString addresses);
	Q_INVOKABLE void updateUdpAddresses(QString addresses);
	Q_INVOKABLE QString getUdpAddresses();
	Q_INVOKABLE QString getLocalAddress();
	Q_INVOKABLE void setPort(quint16 port);

Q_SIGNALS:
    void closed();
    void newConnection(int connectionsCount);
	void udpAddressesChanged(QString adresses);

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    //void processBinaryMessage(QByteArray message);

public Q_SLOTS:
	void socketDisconnected();


private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
	QStringList udpAddresses;
	QUdpSocket *socket;

	quint16 m_port;

};



#endif // WSSERVER_H
