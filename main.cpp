#include "game.h"
#include <QGuiApplication>
#include <QMetaType>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  qRegisterMetaType<Board*>("const Board*");

  Game& game = Game::instance();
  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty(QStringLiteral("game"), &game);
  const QUrl url(QStringLiteral("qrc:/main.qml"));

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject* obj, const QUrl& objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  engine.load(url);

  auto board =
      engine.rootObjects().first()->findChild<const QObject* const>("board");
  QObject::connect(board, SIGNAL(tileAnimsFinished()), &game,
                   SLOT(onTileAnimsFinished()));

  return app.exec();
}
