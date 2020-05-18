#ifndef TILE_H
#define TILE_H

#include <QString>

class Tile
{
public:
  Tile(int value);

  int value() const;
  void setValue(int val);
  QString color() const;

private:
  void setColor();

private:
  int mValue;
  QString mColor;
};

#endif // TILE_H
