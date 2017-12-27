#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "udpclass.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;	
	UdpClass udp;
	engine.rootContext()->setContextProperty("udp", &udp);
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	return app.exec();
}
