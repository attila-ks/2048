function destroyCoveredTiles(tiles) {
    for (var i = 0; i < tiles.length; ++i) {
        for (var j = 0; j < tiles.length; ++j) {
            if (tiles[i].x === tiles[j].x &&
                tiles[i].y === tiles[j].y &&
                i !== j) {
                if (tiles[i].z === 0 && tiles[j].z === 1) {
                    tiles[i].destroy();
                    tiles[j].z = 0;
                    break;
                }
                else if (tiles[i].z === 1 && tiles[j].z === 0) {
                    tiles[j].destroy();
                    tiles[i].z = 0;
                    break;
                }
            }
        }
    }
}
