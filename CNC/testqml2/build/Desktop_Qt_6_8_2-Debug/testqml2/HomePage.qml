import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 6.5

Rectangle {
    anchors.fill: parent

    Image {
        id: splash
        source: "qrc:/Images/splash.png"
        anchors.fill: parent
        opacity: 0.1

        SequentialAnimation on opacity {
            running: true
            NumberAnimation { to: 1; duration: 2000; easing.type: Easing.InOutQuad }
            PauseAnimation { duration: 1000 }
            ScriptAction { script: stackView.push("ParameterSettingPage.qml") }
        }
    }

    BusyIndicator {
        id: busyIndicator
        running: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
    }

    AudioOutput {
        id: welcomeSound
    }

    MediaPlayer {
        id: media
        source: "qrc:/Images/audio.wav"
        audioOutput: welcomeSound
        autoPlay: true
    }
}
