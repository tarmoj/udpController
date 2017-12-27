/*
	Copyright (C) 2016 Tarmo Johannes
	trmjhnns@gmail.com

	This file is part of vClick.

	vClick is free software; you can redistribute it and/or modify it under
	the terms of the GNU GENERAL PUBLIC LICENSE Version 3, published by
	Free Software Foundation, Inc. <http://fsf.org/>

	vClick is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with vClick; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA
*/
#include <QApplication>
#include <QQmlApplicationEngine>
#include "wsserver.h"
#include <QQmlContext>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	WsServer *wsServer;
	wsServer = new WsServer(12021);  // hiljem muuda, nt 12021

	// QML engine and connections
    QQmlApplicationEngine engine;
	//bind object before load

	engine.rootContext()->setContextProperty("wsServer", wsServer); // forward c++ object that can be reached form qml by object name "wsSerrver"

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
