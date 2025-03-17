import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 900
    height: 780
    title: "CNC Routing Machine 💀"

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "HomePage.qml"
    }
}
