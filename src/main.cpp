#include <QCoreApplication>
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QString>
#include <QStringLiteral>
#include <QUrl>
#include <Qt>

int main(int argc, char** argv) {
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  using namespace Qt::StringLiterals;

  const QUrl mainPage{u"qrc:/qt/qml/long_do/main.qml"_s};
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [&mainPage](QObject* obj, const QUrl& objUrl) {
        if (obj == nullptr && mainPage == objUrl) {
          QCoreApplication::exit(-1);
        }
      },
      Qt::QueuedConnection);
  engine.load(mainPage);

  return QGuiApplication::exec();
}
