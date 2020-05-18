var component;

function createTileObject(value, color, x, y) {
    component = Qt.createComponent("Tile.qml");

    if (component.status === Component.Ready ||
        component.status === Component.Error) {
        finishCreation(value, color, x, y);
    }
    else {
        component.statusChanged.connect(finishCreation);
    }
}


function finishCreation(value, color, x, y) {
    if (component.status === Component.Ready) {
        var tile = component.createObject(tiles, {
            "value": value,
            "color": color,
            "x": x,
            "y": y
        });

        if (tile === null) {
            console.log("Error creating tile");
        }
    }
    else if (component.status === Component.Error) {
        console.log("Error loading component:", component.errorString());
    }
}
