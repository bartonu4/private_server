import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0


ApplicationWindow {
    id: window
    width: 640
    height: 840
    visible: true
    title: "Qt Quick Controls 2"
    signal sendCredentials(string msg)

    Loader {
        objectName: "loader"
        id: pageLoader
        anchors.fill: parent
        source: "login.qml"
        onLoaded: {
            console.log("login loaded")

        }
        //property alias loaderSource: loader.source
        function setClient()
        {
            source = "client.qml"
            console.log("set client qml")
        }
    }






}
