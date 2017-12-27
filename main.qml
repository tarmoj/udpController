import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("udpController")
    property string version: "0.1.0"


    menuBar: MenuBar {
        Menu {
            title: qsTr("&Menu")            

            MenuItem {
                text: qsTr("&Update IP address")
                onTriggered:  ipLabel.text = qsTr("My IP: ")+ wsServer.getLocalAddress()
            }
            MenuItem {
                text: qsTr("&About")
                onTriggered: messageDialog.show(qsTr("<b>udpController "+ version + "</b><br>To send upd messages to several clients.<br>Tarmo Johannes 2017<br><br>Built using Qt SDK."));
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    MessageDialog {
        id: messageDialog
        //title: qsTr("Message")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }


    Settings {
        id: settings
        property alias udpAddresses: udpAddresses.text
        property alias port: portSpinBox.value
        property alias command1: command1.text
        property alias command2: command2.text
        property alias command3: command3.text
    }

    Component.onCompleted: {
        wsServer.setUdpAddresses(udpAddresses.text)
        wsServer.setPort(portSpinBox.value)
    }

    Connections {
        target: wsServer
        onNewConnection: {
            //console.log(connectionsCount)
            clientsLabel.text = qsTr("Clients: ") + connectionsCount;
        }

        onUdpAddressesChanged: {
            udpAddresses.text = adresses;
        }


    }


    Rectangle {
        id: mainRect
        color: "#3e5501"
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#b3f50a";
            }

            GradientStop {
                position: 1.00;
                color: "#ffffff";
            }
        }
        anchors.fill: parent

/*
        //Shortcuts
        focus: true
        Keys.onPressed:  { // hack for playing tanja1.sco on F1 and tanja2.sco on F2 -  TODO: implement dialog window for multiple score files + shortcuts
            if (event.key == Qt.Key_F1 || ((event.key == Qt.Key_1) && ( event.modifiers & Qt.ControlModifier) ) ) {

            }
            if (event.key == Qt.Key_F2) {

                console.log("Starting: ", name)
            }
             if (event.key == Qt.Key_F10) {
             }
        }
*/

        Column {
            id: mainColumn
            spacing: 10
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 10
            anchors.topMargin: 5
            anchors.fill: parent

            Label {
                id: ipLabel
                text: qsTr("My ip: ") + wsServer.getLocalAddress();
            }

            Label {
                id: clientsLabel
                visible:  false
                text: qsTr("WS clients: ")
            }


            Row {
                id: portRow
                spacing: 5

                Label {text: qsTr("Destination port: ")}

                SpinBox {
                    id: portSpinBox
                    minimumValue: 1024
                    maximumValue: 65535
                    stepSize: 1
                    value: 10101
                }

                Button {
                    text: qsTr("Update")
                    onClicked: wsServer.setPort(portSpinBox.value)
                }
            }


            Row {
                id: clientsRow
                width: parent.width
                //height: 30
                spacing: 5

                Label {
                    //id: oscLabel
                    text: qsTr("Clients: ")
                }

                TextArea {
                    id: udpAddresses
                    width: 400
                    //placeholderText: qsTr("OSC addresses")
                    text: wsServer.getUdpAddresses();


                }

                Button {
                    id: editOscButton
                    text: qsTr("&Set")
                    onClicked: wsServer.setUdpAddresses(udpAddresses.text);
                }

                Button {
                    id: addLocalhostButton
                    text: qsTr("+ this computer")
                    tooltip: qsTr("To send messages to client in the same computer")
                    onClicked: {
                        if (udpAddresses.text.indexOf("127.0.0.1")<0 ) {
                            udpAddresses.text += ",127.0.0.1 ";
                            wsServer.updateUdpAddresses(udpAddresses.text)
                        }

                    }
                }
            }


            Row {
                spacing: 5

                Label {text: qsTr("Command 1")}

                TextField {
                    id: command1
                    text: "start"
                }

                Button {
                    id: send1
                    text: qsTr("Send")
                    onClicked: {
                        wsServer.updateUdpAddresses(udpAddresses.text) // for any case if some has been added
                        wsServer.send2all(command1.text)
                    }
                }
            }

            //TODO: rather use a repeater...
            Row {
                spacing: 5

                Label {text: qsTr("Command 2")}

                TextField {
                    id: command2
                    text: "stop"
                }

                Button {
                    id: send2
                    text: qsTr("Send")
                    onClicked: {
                        wsServer.updateUdpAddresses(udpAddresses.text)
                        wsServer.send2all(command2.text)
                    }
                }
            }

            Row {
                spacing: 5

                Label {text: qsTr("Command 3")}

                TextField {
                    id: command3
                    text: "seek 0"
                }

                Button {
                    id: send3
                    text: qsTr("Send")
                    onClicked: {
                        wsServer.updateUdpAddresses(udpAddresses.text)
                        wsServer.send2all(command3.text)
                    }
                }
            }

            TextArea {
                id: messageArea
                visible: false
                readOnly: true
                font.pointSize: 8
                font.family: "Courier"
                height: (mainColumn.y+mainColumn.height)-y
                width: parent.width
                text:"Output"
            }

        }






    }
}
