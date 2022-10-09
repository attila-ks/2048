import QtQuick 2.12

Rectangle {
    id: root
    width: 128
    height: 40
    color: "#8f7a66"
    property alias text: txt.text
    signal clicked

    Text {
        id: txt
        font.family: "Arial"
        font.pointSize: 13
        font.bold: true
        color: "#f9f6f2"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.centerIn: parent
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.clicked()
        }
    }
}
