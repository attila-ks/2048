#ifndef SPOT_H
#define SPOT_H

#include "tile.h"
#include <optional>

class Spot
{
public:
  Spot();

  std::optional<Tile> tile() const;
  void setTile(std::optional<Tile> t);

  bool isEmpty() const;

private:
  std::optional<Tile> mTile;
};

#endif // SPOT_H
