#ifndef GAME_H
#define GAME_H

#include "board.h"
#include <QObject>

class Game : public QObject
{
  Q_OBJECT
  Q_PROPERTY(const Board* board READ board CONSTANT)
  Q_PROPERTY(int score READ score NOTIFY scoreChanged)
  Q_PROPERTY(int best READ best NOTIFY bestChanged)

public:
  enum class State
  {
    running,
    win,
    gameOver
  };

  ~Game();
  Game(const Game&) = delete;
  void operator=(const Game&) = delete;

  static Game& instance();

  const Board* board() const;
  int score() const;
  void increaseScore(int value);
  int best() const;

  Q_INVOKABLE void start();
  Q_INVOKABLE void restart();
  Q_INVOKABLE void keepGoing();

  Q_INVOKABLE void processInput(Qt::Key k);

signals:
  void scoreChanged(int value);
  void bestChanged();

  void win();
  void gameOver();

public slots:
  void onTileAnimsFinished();

private:
  explicit Game(QObject* parent = nullptr);

  void load();
  void save() const;

  void checkState();
  bool isWin() const;
  bool isGameOver() const;

private:
  Board mBoard;
  int mScore;
  int mBest;
  State mState;
};

#endif // GAME_H
