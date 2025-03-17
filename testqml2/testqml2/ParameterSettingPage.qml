import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
import QtQuick.Dialogs 6.5

Rectangle {
    anchors.fill: parent
    Image {
        id: bg
        source: "qrc:/Images/bg2.jpg"
        anchors.fill: parent
        opacity: 0.5

        SequentialAnimation on opacity {
            running: true
            NumberAnimation { to: 1; duration: 2000; easing.type: Easing.InOutQuad }
        }
    }
    Rectangle {
        id: header
        width: parent.width - 20
        height: 50
        color: "#999999"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        radius: 10
        anchors.margins: 10

        Text {
            anchors.centerIn: parent
            text: "➕ Add G-Code Parameters"
            font.pixelSize: 30
            font.bold: true
            font.italic: true
            font.family: "Arial"
            color: "#333"
        }
    }

    DropShadow {
        anchors.fill: header
        source: header
        verticalOffset: 5
        horizontalOffset: 5
        samples: 24
        spread: 0.2
        color: "#80000000"
    }

    Grid {
        id: fileSelectionGrid
        columns: 2
        spacing: 10
        anchors.top: header.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter

        Label {
            text: "Select File:"
            font.pixelSize: 18
            font.bold: true
            color: "#333"
        }
        Row {
            spacing: 10
            TextField {
                id: filePathField
                width: 250
                height: 40
                placeholderText: "No file selected"
                readOnly: true
            }
            Rectangle {
                id: browse
                width: 100
                height: 40
                color: "#ffcc66"
                radius: 8
                Text {
                    anchors.centerIn: parent
                    text: "📂 Browse"
                    font.bold: true
                    color: "black"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: fileDialog.open()
                }
            }
        }

        Label { text: "Canny Threshold 1:"; font.pixelSize: 18; font.bold: true; color: "#333" }
        SpinBox { id: cannyThreshold1; from: 0; to: 255; value: 100; width: 120; height: 40 }

        Label { text: "Canny Threshold 2:"; font.pixelSize: 18; font.bold: true; color: "#333" }
        SpinBox { id: cannyThreshold2; from: 0; to: 255; value: 200; width: 120; height: 40 }

        Label { text: "Contour Approximation Epsilon:"; font.pixelSize: 18; font.bold: true; color: "#333" }
        TextField { id: contourApproximationEpsilon; width: 120; height: 40; placeholderText: "0.01"; validator: DoubleValidator { bottom: 0.00001; top: 0.1; decimals: 5 } }

        Label {
            text: "Select Input Type";
            font.pixelSize: 18;
            font.bold: true;
            color: "#333"
        }

        ComboBox {
            id: inputTypeComboBox
            model: ["Gerber", "Image"]
            currentIndex: 1
            anchors.margins: 10
            onCurrentIndexChanged:
            {
                console.log("Selected input type:", inputTypeComboBox.currentText);
                if (currentText === "Gerber")
                {
                    console.log("Processing Gerber file...");
                }
                else
                {
                    console.log("Processing Image file...");
                }
            }
        }

        Label { text: "Workspace Width:"; font.pixelSize: 18; font.bold: true; color: "#333" }
        TextField { id: workspaceWidth; width: 120; height: 40; placeholderText: "mm" }

        Label { text: "Workspace Height:"; font.pixelSize: 18; font.bold: true; color: "#333" }
        TextField { id: workspaceHeight; width: 120; height: 40; placeholderText: "mm" }

        Label { text: "Serial Port:"; font.pixelSize: 18; font.bold: true; color: "#333" }
        TextField { id: serialportid; width: 120; height: 40; placeholderText: "COM" }

    }

    Row {
        id: dialRow
        spacing: 40
        anchors.top: fileSelectionGrid.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        Column {
            spacing: 5
            Label {
                text: "Spindle Speed (RPM)"
                font.pixelSize: 18
                font.bold: true
                color: "red"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Dial {
                id: spindleSpeedDial
                width: 80
                height: 80
                from: 1000
                to: 24000
                value: 12000
                stepSize: 100
                onValueChanged: spindleSpeedLabel.text = "Value: " + value
            }
            Label {
                id: spindleSpeedLabel
                text: "Value: " + spindleSpeedDial.value
                font.pixelSize: 16
                color: "black"
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Column {
            spacing: 5
            Label {
                text: "Feed Rate (mm/min)"
                font.pixelSize: 18
                font.bold: true
                color: "red"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Dial {
                id: feedRateDial
                width: 80
                height: 80
                from: 50
                to: 5000
                value: 1000
                stepSize: 50
                onValueChanged: feedRateLabel.text = "Value: " + value
            }
            Label {
                id: feedRateLabel
                text: "Value: " + feedRateDial.value
                font.pixelSize: 16
                color: "black"
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Column {
            spacing: 5
            Label {
                text: "Cut Depth (mm)"
                font.pixelSize: 18
                font.bold: true
                color: "red"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Dial {
                id: cutDepthDial
                width: 80
                height: 80
                from: 0.1
                to: 20
                value: 1
                stepSize: 0.1
                onValueChanged: cutDepthLabel.text = "Value: " + value.toFixed(1) // Shows one decimal place
            }
            Label {
                id: cutDepthLabel
                text: "Value: " + cutDepthDial.value.toFixed(1) // Shows one decimal place
                font.pixelSize: 16
                color: "black"
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Select a File"
        nameFilters: ["All Files (*.*)", "Images (*.png *.jpg *.jpeg *.gbl *.gbs *.gbrjob)", "G-Code Files (*.nc *.gcode)"]
        fileMode: FileDialog.OpenFile
        onAccepted: { filePathField.text = selectedFile; console.log("Selected File:", selectedFile) }
    }

    DelayButton {
        id: delayButton
        text: "⏳ Hold to Confirm"
        font.pixelSize: 18
        font.bold: true
        delay: 2000
        width: parent.width - 20
        height: 70
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10

        onActivated: {
            console.log("========= Sending Parameters to C++ =========");
            console.log(filePathField.text);
            console.log(inputTypeComboBox.currentText);
            console.log(serialportid.text);

            dataHandler.receiveParameters(
                filePathField.text,
                cannyThreshold1.value,
                cannyThreshold2.value,
                parseFloat(contourApproximationEpsilon.text),
                workspaceWidth.text,
                workspaceHeight.text,
                spindleSpeedDial.value,
                feedRateDial.value,
                cutDepthDial.value,
                serialportid.text,
                inputTypeComboBox.currentText
            );

            console.log("========= Data Sent Successfully =========");

            stackView.push("GeneratingPage.qml");
        }
    }
}
