#include "../headers/board.h"
#include "../headers/game.h"
#include <cstdlib>
#include <ctime>

using namespace std;

#define ROWS 4
#define COLS 4

Board::Board(QObject* parent) :
  QObject(parent),
  mSpots(ROWS, vector<Spot>(COLS))
{
  srand(time(nullptr));
}


Board::~Board()
{
  saveTiles();
}


const vector<vector<Spot>>& Board::spots() const
{
  return mSpots;
}


optional<bool> Board::init(const bool restoreLastState)
{
  removeTiles();

  optional<bool> tilesLoaded;

  if (restoreLastState)
  {
    tilesLoaded = loadTiles();
  }
  if (!tilesLoaded || !(*tilesLoaded))
  {
    addRandomTile();
    addRandomTile();
  }

  return tilesLoaded;
}


int Board::rows() const
{
  return ROWS;
}


int Board::cols() const
{
  return COLS;
}


void Board::addRandomTile()
{
  int row;
  int col;

  do
  {
    row = rand() % ROWS;
    col = rand() % COLS;
  } while (!mSpots[row][col].isEmpty());

  int value = rand() % 10;
  value = value < 9 ? 2 : 4;

  mSpots[row][col].setTile(Tile(value));
  emit tileAdded(value, mSpots[row][col].tile()->color(), row, col);
}


bool Board::moveTilesUp()
{
  int sum = 0;
  bool success = false;

  for (int i = 0; i < COLS; ++i)
  {
    optional<pair<int, int>> emptySpotPos;
    bool mergingEnabled = false;
    for (int j = 0; j < ROWS; ++j)
    {
      if (mSpots[j][i].isEmpty())
      {
        if (!emptySpotPos)
        {
          emptySpotPos = make_pair(j, i);
        }
      }
      else
      {
        bool mergingPerformed = false;
        if (mergingEnabled)
        {
          pair<int, int> destSpotPos;
          if (emptySpotPos)
          {
            destSpotPos =
                make_pair(emptySpotPos->first - 1, emptySpotPos->second);
          }
          else
          {
            destSpotPos = make_pair(j - 1, i);
          }
          if (mSpots[destSpotPos.first][destSpotPos.second].tile()->value() ==
              mSpots[j][i].tile()->value())
          {
            sum += mergeTiles(j, i, destSpotPos.first, destSpotPos.second);
            mergingPerformed = !(mergingEnabled = false);
            emptySpotPos = make_pair(destSpotPos.first + 1, destSpotPos.second);
            success = true;
          }
        }
        if (emptySpotPos && !mergingPerformed)
        {
          moveTile(j, i, emptySpotPos->first, emptySpotPos->second);
          ++(emptySpotPos->first);
          success = true;
        }
        if (!mergingPerformed)
        {
          mergingEnabled = true;
        }
      }
    }
  }

  if (sum > 0)
  {
    Game::instance().increaseScore(sum);
  }

  return success;
}


bool Board::moveTilesDown()
{
  int sum = 0;
  bool success = false;

  for (int i = 0; i < COLS; ++i)
  {
    optional<pair<int, int>> emptySpotPos;
    bool mergingEnabled = false;
    for (int j = ROWS - 1; j >= 0; --j)
    {
      if (mSpots[j][i].isEmpty())
      {
        if (!emptySpotPos)
        {
          emptySpotPos = make_pair(j, i);
        }
      }
      else
      {
        bool mergingPerformed = false;
        if (mergingEnabled)
        {
          pair<int, int> destSpotPos;
          if (emptySpotPos)
          {
            destSpotPos =
                make_pair(emptySpotPos->first + 1, emptySpotPos->second);
          }
          else
          {
            destSpotPos = make_pair(j + 1, i);
          }
          if (mSpots[destSpotPos.first][destSpotPos.second].tile()->value() ==
              mSpots[j][i].tile()->value())
          {
            sum += mergeTiles(j, i, destSpotPos.first, destSpotPos.second);
            mergingPerformed = !(mergingEnabled = false);
            emptySpotPos = make_pair(destSpotPos.first - 1, destSpotPos.second);
            success = true;
          }
        }
        if (emptySpotPos && !mergingPerformed)
        {
          moveTile(j, i, emptySpotPos->first, emptySpotPos->second);
          --(emptySpotPos->first);
          success = true;
        }
        if (!mergingPerformed)
        {
          mergingEnabled = true;
        }
      }
    }
  }

  if (sum > 0)
  {
    Game::instance().increaseScore(sum);
  }

  return success;
}


bool Board::moveTilesLeft()
{
  int sum = 0;
  bool success = false;

  for (int i = 0; i < ROWS; ++i)
  {
    optional<pair<int, int>> emptySpotPos;
    bool mergingEnabled = false;
    for (int j = 0; j < COLS; ++j)
    {
      if (mSpots[i][j].isEmpty())
      {
        if (!emptySpotPos)
        {
          emptySpotPos = make_pair(i, j);
        }
      }
      else
      {
        bool mergingPerformed = false;
        if (mergingEnabled)
        {
          pair<int, int> destSpotPos;
          if (emptySpotPos)
          {
            destSpotPos =
                make_pair(emptySpotPos->first, emptySpotPos->second - 1);
          }
          else
          {
            destSpotPos = make_pair(i, j - 1);
          }
          if (mSpots[destSpotPos.first][destSpotPos.second].tile()->value() ==
              mSpots[i][j].tile()->value())
          {
            sum += mergeTiles(i, j, destSpotPos.first, destSpotPos.second);
            mergingPerformed = !(mergingEnabled = false);
            emptySpotPos = make_pair(destSpotPos.first, destSpotPos.second + 1);
            success = true;
          }
        }
        if (emptySpotPos && !mergingPerformed)
        {
          moveTile(i, j, emptySpotPos->first, emptySpotPos->second);
          ++(emptySpotPos->second);
          success = true;
        }
        if (!mergingPerformed)
        {
          mergingEnabled = true;
        }
      }
    }
  }

  if (sum > 0)
  {
    Game::instance().increaseScore(sum);
  }

  return success;
}


bool Board::moveTilesRight()
{
  int sum = 0;
  bool success = false;

  for (int i = 0; i < ROWS; ++i)
  {
    optional<pair<int, int>> emptySpotPos;
    bool mergingEnabled = false;
    for (int j = COLS - 1; j >= 0; --j)
    {
      if (mSpots[i][j].isEmpty())
      {
        if (!emptySpotPos)
        {
          emptySpotPos = make_pair(i, j);
        }
      }
      else
      {
        bool mergingPerformed = false;
        if (mergingEnabled)
        {
          pair<int, int> destSpotPos;
          if (emptySpotPos)
          {
            destSpotPos =
                make_pair(emptySpotPos->first, emptySpotPos->second + 1);
          }
          else
          {
            destSpotPos = make_pair(i, j + 1);
          }
          if (mSpots[destSpotPos.first][destSpotPos.second].tile()->value() ==
              mSpots[i][j].tile()->value())
          {
            sum += mergeTiles(i, j, destSpotPos.first, destSpotPos.second);
            mergingPerformed = !(mergingEnabled = false);
            emptySpotPos = make_pair(destSpotPos.first, destSpotPos.second - 1);
            success = true;
          }
        }
        if (emptySpotPos && !mergingPerformed)
        {
          moveTile(i, j, emptySpotPos->first, emptySpotPos->second);
          --(emptySpotPos->second);
          success = true;
        }
        if (!mergingPerformed)
        {
          mergingEnabled = true;
        }
      }
    }
  }

  if (sum > 0)
  {
    Game::instance().increaseScore(sum);
  }

  return success;
}


void Board::removeTiles()
{
  for (int i = 0; i < ROWS; ++i)
  {
    for (int j = 0; j < COLS; ++j)
    {
      if (mSpots[i][j].tile())
      {
        mSpots[i][j].setTile(nullopt);
      }
    }
  }
}


void Board::moveTile(const int srcRow, const int srcCol, const int destRow,
                     const int destCol)
{
  mSpots[destRow][destCol].setTile(mSpots[srcRow][srcCol].tile());
  mSpots[srcRow][srcCol].setTile(nullopt);
  emit tileMoved(srcRow, srcCol, destRow, destCol);
}


int Board::mergeTiles(const int srcRow, const int srcCol, const int destRow,
                      const int destCol)
{
  const Tile srcTile(*mSpots[srcRow][srcCol].tile());
  mSpots[srcRow][srcCol].setTile(nullopt);
  Tile destTile(*mSpots[destRow][destCol].tile());
  destTile.setValue(destTile.value() + srcTile.value());
  mSpots[destRow][destCol].setTile(destTile);
  emit tilesMerged(destTile.value(), destTile.color(), srcRow, srcCol, destRow,
                   destCol);

  return destTile.value();
}


bool Board::loadTiles()
{
  bool success = false;

  if (FILE* file = fopen("tiles.dat", "r"))
  {
    int value;
    int row;
    int col;

    while (fscanf(file, "%d,%d,%d", &value, &row, &col) == 3)
    {
      mSpots[row][col].setTile(Tile(value));
      emit tileAdded(value, mSpots[row][col].tile()->color(), row, col);
      success = true;
    }

    fclose(file);
  }

  return success;
}


void Board::saveTiles() const
{
  if (FILE* file = fopen("tiles.dat", "w"))
  {
    for (int i = 0; i < ROWS; ++i)
    {
      for (int j = 0; j < COLS; ++j)
      {
        if (mSpots[i][j].tile())
        {
          fprintf(file, "%d,%d,%d\n", mSpots[i][j].tile()->value(), i, j);
        }
      }
    }

    fclose(file);
  }
}
