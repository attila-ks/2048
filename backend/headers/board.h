#ifndef BOARD_H
#define BOARD_H

#include "spot.h"
#include <QObject>
#include <vector>

class Board : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int rows READ rows CONSTANT)
  Q_PROPERTY(int cols READ cols CONSTANT)

public:
  explicit Board(QObject* parent = nullptr);
  ~Board();

  const std::vector<std::vector<Spot>>& spots() const;

  // If the param is true, the method tries to load the last state of the board.
  // If it was successfully loaded, the method returns true, otherwise returns
  // false. If the param is false, the method returns 'nullopt'.
  std::optional<bool> init(bool restoreLastState);

  int rows() const;
  int cols() const;

  void addRandomTile();

  bool moveTilesUp();
  bool moveTilesDown();
  bool moveTilesLeft();
  bool moveTilesRight();

signals:
  void tileAdded(int value, QString color, int row, int col);
  void tileMoved(int srcRow, int srcCol, int destRow, int destCol);
  void tilesMerged(int value, QString color, int srcRow, int srcCol,
                   int destRow, int destCol);

private:
  void removeTiles();

  void moveTile(int srcRow, int srcCol, int destRow, int destCol);
  // Returns with the sum of the merged Tiles' value.
  int mergeTiles(int srcRow, int srcCol, int destRow, int destCol);

  bool loadTiles();
  void saveTiles() const;

private:
  std::vector<std::vector<Spot>> mSpots;
};

#endif // BOARD_H
