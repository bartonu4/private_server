import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ClientsModel 1.0
import MessageModel 1.0

RowLayout {
    id: row
    spacing:0
    //width: Math.min(window.width, window.height) / 3 * 2


    signal addButton()

    ClientsModel {
        id: clientsModel
        objectName: "clientsModel"


    }
    MessageModel {
        signal sendMessage(string recepient, string message)
        id: messageModel
        objectName: "messageModel"

    }
    //    Binding {
    //        function getValue(text){
    //        console.log("from binding"+text)
    //            return text;
    //        }
    //        target: messageModel
    //        property: "recipient"
    //        when:listClients.currentIndex >= 0
    //        value:getValue(listClients.delegate.text);
    //    }

    Rectangle {
        color: "#eeeeee"

        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.minimumWidth: 0.3*parent.width
        Layout.preferredWidth: 0.3*parent.width
        Layout.maximumWidth: 0.3*parent.width




        ListView {

            anchors.fill: parent
            id: listClients
            objectName: "listClients"


            signal chooseClient(string msg)
            currentIndex: -1

            //width: 180; height: 200

            delegate:     ItemDelegate {
                id: control
                text: clientsModel.data[model.index]
                width: listClients.width


                contentItem: Text {
                    rightPadding: control.spacing
                    text: control.text
                    font: control.font
                    color: control.enabled ? (control.down ? "#17a81a" : "#21be2b") : "#bdbebf"
                    elide: Text.ElideRight
                    visible: true
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                background:
                    Rectangle {

                    implicitWidth: 100
                    implicitHeight: 50
                    opacity: enabled ? 1 : 0.3
                    color: control.down ? "#dddedf" : "#eeeeee"

                    Rectangle {
                        width: parent.width
                        height: 1
                        color: control.down ? "#17a81a" : "#21be2b"
                        anchors.bottom: parent.bottom
                    }
                }
                onClicked: {

                    listClients.currentIndex = model.index
                    messageModel.recipient = text;
                    console.log(listClients.currentIndex)
                    listClients.chooseClient(messageModel.recipient)
                    listMessage.visible = true
                    //console.log(messageModel.recipient)
                    //clientsModel.add("nu4")
                }



            }


            highlight: Rectangle {
                width: parent.width
                color: "red"
            }
            highlightFollowsCurrentItem: false
            model: clientsModel.data

            ScrollIndicator.vertical: ScrollIndicator { }

        }
    }

    Rectangle {

        id: rectList

        Layout.fillWidth: true
        Layout.fillHeight: true
        //            Layout.minimumWidth: 800
        //            Layout.preferredWidth: 800
        //            Layout.maximumWidth: 800


        color: "grey"
        ListView {

            id: listMessage
            width: parent.width
            height: parent.height*0.7
            verticalLayoutDirection: ListView.BottomToTop
            visible: false

            add: Transition {
                NumberAnimation { properties: "x,y"; from: 100; duration: 1000 }
            }
            currentIndex: -1

            //width: 180; height: 200

            delegate:     ItemDelegate {
                function calculateSize(width, contentWidth)
                {
                    //console.log(width)
                    console.log(contentWidth)
                    return width*0.3
                }
                id: messageId
                text: model.message
                property bool senderMe: model.sender === messageModel.myName
                anchors.right:senderMe ? parent.right : undefined
                anchors.margins: 10
                width: calculateSize(parent.width, contentMessage.contentWidth)



                highlighted: ListView.isCurrentItem
                contentItem: Text {
                    id: contentMessage
                    rightPadding: messageId.spacing
                    text: messageId.text
                    font: messageId.font
                    color: "black"
                    elide: Text.ElideRight
                    visible: true
                    horizontalAlignment:Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap

                }

                background:
                    Rectangle {

                    //implicitHeight: 50
                    width: parent.width
                    radius: 15
                    opacity: enabled ? 1 : 0.3
                    color:  senderMe ? "#D4F0C9" : "#DC8FF2"

                    Rectangle {
                        //                        function getVisibility(model)
                        //                        {
                        //                            console.log(model.sender)

                        //                            if(model.at(index).sender === model.sender )
                        //                            {
                        //                                return false;
                        //                            }
                        //                            else
                        //                            {
                        //                                return true;
                        //                            }
                        //                        }
                        width: parent.width
                        //visible: getVisibility(model)
                        color:  "#586547"
                        anchors.top: parent.top
                        Text
                        {   leftPadding: 5
                            color:"#3D738A"
                            text:senderMe ? "" :model.sender
                        }
                    }
                }
                onClicked: {
                    listClients.currentIndex = index
                    console.log(messageId.text)
                    console.log(listMessage.model.sender)
                }



            }


            highlight: Rectangle {
                width: parent.width
                color: "red"
            }
            model: messageModel

            ScrollIndicator.vertical: ScrollIndicator { }

        }

        Rectangle {
            id: rectMessage
            anchors.top: listMessage.bottom
            height:parent.height*0.3
            width:parent.width
            color: "white"

            TextArea {
                id: textAreaMessage
                anchors.fill: parent
                Keys.onReturnPressed: {
                    if(text.length > 0)
                    {
                        messageModel.add(messageModel.myName, messageModel.recipient, text)
                        messageModel.sendMessage(messageModel.recipient, text)
                        console.log(text)

                        //textAreaMessage.clear()
                    }
                }

            }


        }

    }




}





//Pane {

//



//    Column {

//        spacing: 10
//        anchors.centerIn:parent

//        Label {
//            text: "Clients"
//            font.pixelSize: 22
//            font.italic: true
//            color: "steelblue"
//        }



//}
