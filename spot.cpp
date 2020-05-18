#include "spot.h"

using namespace std;

Spot::Spot() :
  mTile(std::nullopt) {}


optional<Tile> Spot::tile() const
{
  return mTile;
}


void Spot::setTile(optional<Tile> t)
{
  mTile.swap(t);
}


bool Spot::isEmpty() const
{
  return !mTile.has_value();
}
