import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    width: 590
    height: 715
    minimumWidth: width
    minimumHeight: height
    maximumWidth: width
    maximumHeight: height
    title: " " // TODO: Is this the only way?
    color: "#faf8ef"
    visible: true
    Component.onCompleted: {
        game.start();
    }

    Item {
        anchors.fill: parent
        focus: true
        Keys.onPressed: {
            game.processInput(event.key);
        }

        Text {
            text: "2048"
            font.family: "Arial"
            font.pointSize: rectScore.height
            font.bold: true
            color: "#776e65"
            x: board.x
            anchors.verticalCenter: rectScore.verticalCenter
        }

        Rectangle {
            id: rectScore
            width: score.width + 50
            height: 55
            color: "#bbada0"
            radius: 3
            x: rectBest.x - width - 5
            y: 24
            Text {
                text: "SCORE"
                font.family: "Arial"
                font.pointSize: 10
                font.bold: true
                color: "#eee4da"
                anchors.horizontalCenter: parent.horizontalCenter
                y: 7
            }
            Text {
                id: score
                text: game.score
                font.family: "Arial"
                font.pointSize: 20
                font.bold: true
                color: "#ffffff"
                anchors.horizontalCenter: parent.horizontalCenter
                y: 22
            }
            Text {
                id: addedValue
                text: ""
                font.family: "Arial"
                font.pointSize: 20
                font.bold: true
                color: "#776e65"
                anchors.horizontalCenter: parent.horizontalCenter
                y: 22
                Connections {
                    target: game
                    onScoreChanged: {
                        if (value > 0) {
                            addedValue.text = "+" + value;
                            scoreAnim.restart();
                        }
                    }
                }
                ParallelAnimation {
                    id: scoreAnim
                    NumberAnimation {
                        target: addedValue
                        property: "y"
                        from: 22
                        to: -40
                        duration: 500
                    }
                    NumberAnimation {
                        target: addedValue
                        property: "opacity"
                        from: 1.0
                        to: 0.0
                        duration: 500
                    }
                }
            }
        }

        Rectangle {
            id: rectBest
            width: best.width + 50
            height: 55
            color: "#bbada0"
            radius: 3
            anchors.right: board.right
            y: 24
            Text {
                text: "BEST"
                font.family: "Arial"
                font.pointSize: 10
                font.bold: true
                color: "#eee4da"
                anchors.horizontalCenter: parent.horizontalCenter
                y: 7
            }
            Text {
                id: best
                text: game.best
                font.family: "Arial"
                font.pointSize: 20
                font.bold: true
                color: "#ffffff"
                anchors.horizontalCenter: parent.horizontalCenter
                y: 22
            }
        }

        Item {
            id: item
            width: 500
            height: 80
            anchors.top: rectScore.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                text: "Join the numbers and get to the <b>2048 tile!</b>"
                font.family: "Arial"
                font.pointSize: 13
                color: "#776e65"
                anchors.verticalCenter: buttonNewGame.verticalCenter
            }
            CustomButton {
                id: buttonNewGame
                text: "New Game"
                radius: 3
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                onClicked: {
                    board.tiles.children = []
                    board.runningTileAnimCounter = 0;
                    winDialog.visible = false;
                    gameOverDialog.visible = false;
                    game.restart();
                }
            }
        }

        Board {
            id: board
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: item.bottom
        }

        Connections {
            target: game
            onGameOver: {
                gameOverDialog.visible = true;
            }
            onWin: {
                winDialog.visible = true;
            }
        }
        Item {
            id: gameOverDialog
            visible: false
            anchors.fill: board
            Rectangle {
                anchors.fill: parent
                color: "#faf8ef"
                opacity: 0.4
            }
            Text {
                text: "Game over!"
                font.family: "Arial"
                font.pointSize: 43
                font.bold: true
                color: "#776e65"
                anchors.centerIn: parent
            }
        }
        Item {
            id: winDialog
            visible: false
            anchors.fill: board
            Rectangle {
                anchors.fill: parent
                color: "#edc53f"
                opacity: 0.6
            }
            Text {
                id: textWin
                text: "You win!"
                font.family: "Arial"
                font.pointSize: 43
                font.bold: true
                color: "#f9f6f2"
                anchors.centerIn: parent
            }
            CustomButton {
                text: "Keep going"
                radius: 3
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: textWin.bottom
                anchors.topMargin: 25
                onClicked: {
                    winDialog.visible = false;
                    game.keepGoing();
                }
            }
        }
    }
}
