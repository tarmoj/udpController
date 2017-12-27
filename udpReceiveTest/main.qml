import QtQuick 2.5
import QtQuick.Controls 1.4

ApplicationWindow {
    visible: true
    width: 640
    height: 400
    title: qsTr("UDP receive test")
    id: window

    Connections {
        target: udp

        onNewMessage: {
            console.log(message)
            messageArea.append(message)
        }
    }

    Column {
        anchors.fill: parent
        spacing: 10

        Label {
            id: ipLabel
            text: qsTr("My ip: ") + udp.getLocalAddress();
        }

        Row {
            spacing: 5
            Label {text:"Port (hardcoded): 7007"}

//            SpinBox {
//                id: portSpinBox
//                minimumValue: 1024
//                maximumValue: 65535
//                stepSize: 1
//                value: 6006
//            }

//            Button {
//                text: qsTr("Update")
//                onClicked: udp.setPort(portSpinBox.value)
//            }

        }

        Label {text: "Received messages"}

        TextArea {
            id: messageArea

            width: parent.width
            height: parent.height
        }


    }


}
