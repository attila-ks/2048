import QtQuick 2.12

Item {
    id: tile
    width: 106
    height: 106
    property int value
    property string color
    property bool merged: false
    Component.onCompleted: {
        rect.value = tile.value;
        rect.color = color;
        creationAnim.start();
    }

    Rectangle {
        id: rect
        radius: 3
        property alias value: value.text
        anchors.fill: parent
        Text {
            id: value
            font.family: "Arial"
            font.bold: true
            color: text === "2" || text === "4" ? "#776e65" : "#f9f6f2";
            anchors.centerIn: parent
            onTextChanged: {
                font.pointSize = calculateValuePointSize(text);
            }
        }
    }

    ScaleAnimator {
        id: creationAnim
        target: tile
        from: 0.0
        to: 1.0
        duration: 100
        onStarted: {
            ++board.runningTileAnimCounter;
        }
        onFinished: {
            --board.runningTileAnimCounter;
        }
    }

    Behavior on x {
        NumberAnimation {
            duration: 80
            onRunningChanged: {
                if (running) {
                    ++board.runningTileAnimCounter;
                }
                else {
                    if (tile.merged) {
                        mergingAnim.start();
                    }
                    else {
                        --board.runningTileAnimCounter;
                    }
                }
            }
        }
    }
    Behavior on y {
        NumberAnimation {
            duration: 80
            onRunningChanged: {
                if (running) {
                    ++board.runningTileAnimCounter;
                }
                else {
                    if (tile.merged) {
                        mergingAnim.start();
                    }
                    else {
                        --board.runningTileAnimCounter;
                    }
                }
            }
        }
    }

    SequentialAnimation {
        id: mergingAnim
        onRunningChanged: {
            if (running) {
                ++board.runningTileMergingAnimCounter;
                rect.value = tile.value;
                rect.color = tile.color;
            }
            else {
                --board.runningTileAnimCounter;
                --board.runningTileMergingAnimCounter;
                tile.merged = false;
            }
        }
        ScaleAnimator {
            target: tile
            from: 1.0
            to: 1.1
            duration: 100
        }
        ScaleAnimator {
            target: tile
            from: 1.1
            to: 1.0
            duration: 100
        }
    }

    function calculateValuePointSize(value) {
        var pointSize;

        switch (value.length) {
        case 1:
        case 2:
            pointSize = 40;
            break;
        case 3:
            pointSize = 35;
            break;
        case 4:
            pointSize = 30;
            break;
        case 5:
            pointSize = 25;
            break;
        }

        return pointSize;
    }
}
