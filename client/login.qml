import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2
Pane {

    id: pane



    Column {
        spacing: 10
        anchors.centerIn:parent

        Rectangle {
            id: rect
            objectName: "rect"
            width: 200
            height: 100
            color: "#00B000"


            Text {
                anchors.centerIn: parent
                text: "Super Encrypred Chat"
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.color = "#00B000"
                onExited:  parent.color = "steelblue"
            }
        }
        TextField {
            id: login

            placeholderText: qsTr("Enter name")
        }
        TextField {
            id: password
            echoMode: TextInput.Password
            placeholderText: qsTr("Enter password")
        }
        Component {
            id: busyLoader
            BusyIndicator {
            anchors.centerIn: pageLoader
            running: true
            }
        }

        MessageDialog {
            id: messageDialog
            objectName: "messageDialogObj"
            standardButtons: StandardButton.Ok
            onAccepted: {
                console.log("And of course you could only agree.")

            }
            visible: false
            }


        Button {

            text: "Button"
            onClicked: {
                //pageLoader.sourceComponent = busyLoader
                sendCredentials([login.text,password.text])
                //pageLoader.source = "client.qml"


            }

        }

    }
}
