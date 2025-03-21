import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
import QtQuick.Dialogs 6.5

Rectangle {
    anchors.fill: parent
    Image {
        id: background
        source: "qrc:/Images/final.jpg"
        anchors.fill: parent
        opacity: 0.5

        SequentialAnimation on opacity {
            running: true
            NumberAnimation { to: 1; duration: 2000; easing.type: Easing.InOutQuad }
        }
    }

    Column {
        spacing: 30
        anchors.centerIn: parent

        Rectangle {
            id: generatesvg
            width: 280
            height: 70
            radius: 15
            color: "#2196F3"
            border.color: "#444"
            border.width: 2

            Text {
                anchors.centerIn: parent
                text: "🖼️ Generate SVG"
                font.pixelSize: 22
                font.bold: true
                color: "black"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("🖼️ Generate SVG button pressed!");
                    image_processing.processImage();
                    customMessageDialog.open();
                }
                onPressed: parent.opacity = 0.3
                onReleased: parent.opacity = 1.0
            }

            DropShadow {
                anchors.fill: parent
                source: parent
                verticalOffset: 5
                horizontalOffset: 5
                samples: 16
                color: "#80000000"
            }
        }

        Rectangle {
            id: generategcode
            width: 280
            height: 70
            radius: 15
            color: "#4CAF50"
            border.color: "#444"
            border.width: 2

            Text {
                anchors.centerIn: parent
                text: "⚙️ Generate G-Code"
                font.pixelSize: 22
                font.bold: true
                color: "black"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("🖼️ Generate gcode button pressed!");
                    gcode.invokeGeneration();
                    customMessageDialog.open();
                }
                onPressed: parent.opacity = 0.3
                onReleased: parent.opacity = 1.0
            }

            DropShadow {
                anchors.fill: parent
                source: parent
                verticalOffset: 5
                horizontalOffset: 5
                samples: 16
                color: "#80000000"
            }
        }

        Rectangle {
            id: homing
            width: 280
            height: 70
            radius: 15
            color: "gray"
            border.color: "#444"
            border.width: 2

            Text {
                anchors.centerIn: parent
                text: "📜 Homing"
                font.pixelSize: 22
                font.bold: true
                color: "black"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("🖼️ Homing button pressed!");

                    stackView.push("AdjustHomePosition.qml")
                }
                onPressed: parent.opacity = 0.3
                onReleased: parent.opacity = 1.0
            }

            DropShadow {
                anchors.fill: parent
                source: parent
                verticalOffset: 5
                horizontalOffset: 5
                samples: 16
                color: "#80000000"
            }
        }

        Rectangle {
            id: serialize
            width: 280
            height: 70
            radius: 15
            color: "#FF9800"
            border.color: "#444"
            border.width: 2

            Text {
                anchors.centerIn: parent
                text: "📜 Serialize"
                font.pixelSize: 22
                font.bold: true
                color: "black"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("🖼️ Serialization button pressed!");
                    serial.StartProcess();
                    stackView.push("SerialMonitor.qml")
                }
                onPressed: parent.opacity = 0.3
                onReleased: parent.opacity = 1.0
            }

            DropShadow {
                anchors.fill: parent
                source: parent
                verticalOffset: 5
                horizontalOffset: 5
                samples: 16
                color: "#80000000"
            }
        }
    }

    Popup {
        id: customMessageDialog
        width: 350
        height: 180
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        opacity: 0

        background: Item {
            Rectangle {
                id: bg
                width: parent.width
                height: parent.height
                color: "#FFFFFF"
                radius: 15
            }

            DropShadow {
                anchors.fill: bg
                source: bg
                radius: 10
                samples: 20
                color: "#80000000"
            }
        }

        Column {
            anchors.centerIn: parent
            spacing: 15
            width: parent.width * 0.85

            Text {
                text: "웃 Success"
                font.bold: true
                font.pixelSize: 20
                color: "#2C3E50"
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
            }

            Text {
                text: "Operation completed successfully."
                font.pixelSize: 14
                color: "#555"
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                width: parent.width
            }

            Button {
                text: "OK"
                width: 70
                height: 40
                font.pixelSize: 14
                background: Rectangle {
                    color: "#3498db"
                    radius: 20
                }
                contentItem: Text {
                    text: parent.text
                    font.pixelSize: 14
                    color: "white"
                    anchors.centerIn: parent
                }
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: customMessageDialog.close()
            }
        }

        onOpened: opacityAnim.start()

        NumberAnimation {
            id: opacityAnim
            target: customMessageDialog
            property: "opacity"
            from: 0
            to: 1
            duration: 250
            easing.type: Easing.OutQuad
        }
    }
}
