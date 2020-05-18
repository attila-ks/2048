#include "game.h"
#include <fstream>

using namespace std;

static bool sgInputProcEnabled = false;
static bool sgGameRestarted = false;
static bool sgPlayerHasAlreadyWon = false;

Game::~Game()
{
  save();
}


Game& Game::instance()
{
  static Game sGame;
  return sGame;
}


const Board* Game::board() const
{
  return &mBoard;
}


int Game::score() const
{
  return mScore;
}


void Game::increaseScore(const int value)
{
  mScore += value;
  emit scoreChanged(value);

  if (mScore > mBest)
  {
    mBest = mScore;
    emit bestChanged();
  }
}


int Game::best() const
{
  return mBest;
}


void Game::start()
{
  const optional<bool> tilesLoaded = mBoard.init(true);

  if (!(*tilesLoaded))
  {
    mScore = 0;
    sgPlayerHasAlreadyWon = false;

    emit scoreChanged(0);
  }
}


void Game::restart()
{
  mBoard.init(false);
  mScore = 0;
  mState = Game::State::running;

  sgInputProcEnabled = false;
  sgGameRestarted = true;
  sgPlayerHasAlreadyWon = false;

  emit scoreChanged(0);
}


void Game::keepGoing()
{
  if (mState == Game::State::win)
  {
    mState = Game::State::running;
    sgPlayerHasAlreadyWon = true;

    mBoard.addRandomTile();
  }
}


void Game::processInput(const Qt::Key k)
{
  if (!sgInputProcEnabled || mState != Game::State::running)
  {
    return;
  }

  switch (k)
  {
    case Qt::Key_W:
    case Qt::Key_Up:
      sgInputProcEnabled = !mBoard.moveTilesUp();
      break;
    case Qt::Key_S:
    case Qt::Key_Down:
      sgInputProcEnabled = !mBoard.moveTilesDown();
      break;
    case Qt::Key_A:
    case Qt::Key_Left:
      sgInputProcEnabled = !mBoard.moveTilesLeft();
      break;
    case Qt::Key_D:
    case Qt::Key_Right:
      sgInputProcEnabled = !mBoard.moveTilesRight();
      break;
  }
}


void Game::onTileAnimsFinished()
{
  checkState();

  static int sCounter = 0;

  if (sgGameRestarted)
  {
    sCounter = 0;
    sgGameRestarted = false;
  }

  ++sCounter;

  if (sCounter % 2 == 0)
  {
    if (mState == Game::State::running)
    {
      mBoard.addRandomTile();
      sgInputProcEnabled = false;
    }
  }
  else
  {
    sgInputProcEnabled = true;
  }
}


Game::Game(QObject* parent) :
  QObject(parent),
  mBoard(),
  mScore(0),
  mBest(0),
  mState(Game::State::running)
{
  load();
}


void Game::load()
{
  ifstream f("state.dat");

  if (f.is_open())
  {
    f >> mScore;
    f >> mBest;
    f >> sgPlayerHasAlreadyWon;
    f.close();
  }
}


void Game::save() const
{
  ofstream f("state.dat");

  if (f.is_open())
  {
    f << mScore << '\n';
    f << mBest << '\n';
    f << sgPlayerHasAlreadyWon;
    f.close();
  }
}


void Game::checkState()
{
  if (!sgPlayerHasAlreadyWon && isWin())
  {
    mState = Game::State::win;
    emit win();
  }
  else if (isGameOver())
  {
    mState = Game::State::gameOver;
    emit gameOver();
  }
}


bool Game::isWin() const
{
  const auto& spots = mBoard.spots();

  for (const auto& row : spots)
  {
    for (const auto& spot : row)
    {
      if (!spot.isEmpty() && spot.tile()->value() == 2048)
      {
        return true;
      }
    }
  }

  return false;
}


bool Game::isGameOver() const
{
  const auto& spots = mBoard.spots();

  for (size_t i = 0; i < spots.size(); ++i)
  {
    for (size_t j = 0; j < spots[i].size(); ++j)
    {
      if (spots[i][j].isEmpty())
      {
        return false;
      }
      if (j + 1 < spots[i].size() && !spots[i][j + 1].isEmpty() &&
          spots[i][j].tile()->value() == spots[i][j + 1].tile()->value())
      {
        return false;
      }
      if (i + 1 < spots.size() && !spots[i + 1][j].isEmpty() &&
          spots[i][j].tile()->value() == spots[i + 1][j].tile()->value())
      {
        return false;
      }
    }
  }

  return true;
}
