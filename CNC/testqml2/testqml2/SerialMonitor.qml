import QtQuick 2.15

Window {
    width: 800
    height: 700
    visible: true
    title: "Serial Monitor 🍵"
    x: 50
    y: 50
    Rectangle {
        anchors.fill: parent
        color: "black"
        Image {
            id: background
            source: "qrc:/Images/serial.jpg"
            anchors.fill: parent
        }
        Flickable {
            id: flickable
            anchors.fill: parent
            contentHeight: textEdit.paintedHeight
            interactive: true

            TextEdit {
                id: textEdit
                width: flickable.width
                height: flickable.contentHeight
                color: "white"
                font.pixelSize: 18
                wrapMode: TextEdit.Wrap
                readOnly: true
                text: serial.SendToSerialMonitor();
                font.bold: true
                font.italic: true
                Component.onCompleted: flickable.contentY = flickable.contentHeight
                onTextChanged: flickable.contentY = flickable.contentHeight
            }
        }
    }
}

