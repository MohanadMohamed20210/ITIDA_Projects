import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    width: 400
    height: 450
    visible: true
    title: "CNC Controller 🛠️"
    x: 50
    y: 50

    background: Rectangle {
        color: "#1E1E1E"
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 30

        Label {
            text: "CNC Motor Controller 🏗️"
            font.pixelSize: 27
            font.bold: true
            color: "white"
            Layout.alignment: Qt.AlignHCenter
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 20

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 20

                Button {
                    text: "⬆️ Y+"
                    font.pixelSize: 28
                    width: 180
                    height: 90
                    background: Rectangle { color: "#3498db"; radius: 20 }
                    contentItem: Text { text: parent.text; color: "white"; font.pixelSize: 28; anchors.centerIn: parent }
                    onClicked: serial.sendCommand("G91 G1 Y1 F1000")
                }

                Button {
                    text: "⬇️ Y-"
                    font.pixelSize: 28
                    width: 180
                    height: 90
                    background: Rectangle { color: "#3498db"; radius: 20 }
                    contentItem: Text { text: parent.text; color: "white"; font.pixelSize: 28; anchors.centerIn: parent }
                    onClicked: serial.sendCommand("G91 G1 Y-1 F1000")
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 20

                Button {
                    text: "⬅️ X-"
                    font.pixelSize: 28
                    width: 180
                    height: 90
                    background: Rectangle { color: "#3498db"; radius: 20 }
                    contentItem: Text { text: parent.text; color: "white"; font.pixelSize: 28; anchors.centerIn: parent }
                    onClicked: serial.sendCommand("G91 G1 X-1 F1000")
                }

                Button {
                    text: "➡️ X+"
                    font.pixelSize: 28
                    width: 180
                    height: 90
                    background: Rectangle { color: "#3498db"; radius: 20 }
                    contentItem: Text { text: parent.text; color: "white"; font.pixelSize: 28; anchors.centerIn: parent }
                    onClicked: serial.sendCommand("G91 G1 X1 F1000")
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 20

                Button {
                    text: "⬆️ Z+"
                    font.pixelSize: 28
                    width: 180
                    height: 90
                    background: Rectangle { color: "#3498db"; radius: 20 }
                    contentItem: Text { text: parent.text; color: "white"; font.pixelSize: 28; anchors.centerIn: parent }
                    onClicked: serial.sendCommand("G91 G1 Z-1 F1000")
                }

                Button {
                    text: "⬇️ Z-"
                    font.pixelSize: 28
                    width: 180
                    height: 90
                    background: Rectangle { color: "#3498db"; radius: 20 }
                    contentItem: Text { text: parent.text; color: "white"; font.pixelSize: 28; anchors.centerIn: parent }
                    onClicked: serial.sendCommand("G91 G1 Z1 F1000")
                }
            }
            Button {
                text: "🏠 Set Home"
                font.pixelSize: 30
                Layout.alignment: Qt.AlignHCenter
                width: 300
                height: 100
                background: Rectangle { color: "#27ae60"; radius: 25 }
                contentItem: Text { text: parent.text; color: "white"; font.pixelSize: 30; anchors.centerIn: parent }
                onClicked: serial.sendCommand("G10 L20 P1 X0 Y0 Z0")
            }
        }
    }
}
