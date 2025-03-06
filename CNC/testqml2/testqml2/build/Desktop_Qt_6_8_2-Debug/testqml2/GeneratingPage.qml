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
                    messageDialogCheck.open();
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
                    messageDialogCheck.open();
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
    MessageDialog {
        id: messageDialogCheck
        title: "🎉 Success"
        text: "Operation completed successfully."
        buttons: MessageDialog.Ok
    }
}
