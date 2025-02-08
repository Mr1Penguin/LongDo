#include <QCoreApplication>
#include <QGuiApplication>
#include <QObject>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QString>
#include <QStringLiteral>
#include <QUrl>
#include <Qt>

#include <klocalizedcontext.h>
#include <klocalizedstring.h>

int main(int argc, char** argv) {
  using namespace Qt::StringLiterals;

  QGuiApplication app(argc, argv);
  KLocalizedString::setApplicationDomain("long-do"_ba);
  QQmlApplicationEngine engine;

  const QUrl mainPage{u"qrc:/qt/qml/long_do/main.qml"_s};
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [&mainPage](QObject* obj, const QUrl& objUrl) {
        if (obj == nullptr && mainPage == objUrl) {
          QCoreApplication::exit(-1);
        }
      },
      Qt::QueuedConnection);

  //NOLINTNEXTLINE
  engine.rootContext()->setContextObject(new KLocalizedContext(&engine));

  engine.load(mainPage);

  return QGuiApplication::exec();
}
