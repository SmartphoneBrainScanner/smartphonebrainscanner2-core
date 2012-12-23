import QtQuick 1.0

Rectangle {
    width: 360
    height: 360
    Text {
	text: qsTr("Test App")
        anchors.centerIn: parent
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }
}
