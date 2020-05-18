import QtQuick 2.12
import "tile-creator.js" as TileCreator
import "tile-destroyer.js" as TileDestroyer

Rectangle {
    id: board
    objectName: "board"
    width: 500
    height: 500
    color: "#bbada0"
    radius: 6
    property int runningTileAnimCounter: 0
    property int runningTileMergingAnimCounter: 0
    property alias tiles: tiles
    signal tileAnimsFinished
    onRunningTileAnimCounterChanged: {
        if (runningTileAnimCounter === 0) {
            tileAnimsFinished();
        }
    }
    onRunningTileMergingAnimCounterChanged: {
        if (runningTileMergingAnimCounter === 0) {
            TileDestroyer.destroyCoveredTiles(tiles.children);
        }
    }

    Grid {
        id: grid
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15
        Repeater {
            id: repeater
            model: game.board.rows * game.board.cols
            delegate: Spot {}
        }
    }

    Item {
        id: tiles
        width: grid.childrenRect.width
        height: grid.childrenRect.height
        x: 15
        y: 15
        Connections {
            target: game.board
            onTileAdded: {
                var index = row * game.board.rows + col;
                var destSpot = repeater.itemAt(index);
                TileCreator.createTileObject(value, color,
                                             destSpot.x, destSpot.y);
            }
            onTileMoved: {
                var index = srcRow * game.board.rows + srcCol;
                var srcSpot = repeater.itemAt(index);
                var tile = tiles.childAt(srcSpot.x, srcSpot.y);
                index = destRow * game.board.rows + destCol;
                var destSpot = repeater.itemAt(index);
                tile.x = destSpot.x;
                tile.y = destSpot.y;
            }
            onTilesMerged: {
                var index = srcRow * game.board.rows + srcCol;
                var srcSpot = repeater.itemAt(index);
                var tile = tiles.childAt(srcSpot.x, srcSpot.y);
                index = destRow * game.board.rows + destCol;
                var destSpot = repeater.itemAt(index);
                tile.color = color;
                tile.value = value;
                tile.merged = true;
                tile.x = destSpot.x;
                tile.y = destSpot.y;
                tile.z = 1;
            }
        }
    }
}
