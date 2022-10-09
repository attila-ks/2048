#include "../headers/tile.h"

Tile::Tile(const int value) :
  mValue(value)
{
  setColor();
}


int Tile::value() const
{
  return mValue;
}


void Tile::setValue(const int val)
{
  mValue = val;
  setColor();
}


QString Tile::color() const
{
  return mColor;
}


void Tile::setColor()
{
  switch (mValue)
  {
    case 2:
      mColor = "#eee4da";
      break;
    case 4:
      mColor = "#ede0c8";
      break;
    case 8:
      mColor = "#f2b179";
      break;
    case 16:
      mColor = "#f59563";
      break;
    case 32:
      mColor = "#f67c5f";
      break;
    case 64:
      mColor = "#f65e3b";
      break;
    case 128:
      mColor = "#edcf72";
      break;
    case 256:
      mColor = "#edcc61";
      break;
    case 512:
      mColor = "#edc850";
      break;
    case 1024:
      mColor = "#edc53f";
      break;
    case 2048:
      mColor = "#edc22e";
      break;
    default:
      mColor = "#3c3a32";
      break;
  }
}
