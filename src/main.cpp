#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QString>
#include <QStringLiteral>
#include <QUrl>

#include <klocalizedcontext.h>
#include <klocalizedstring.h>

#include "MainPageLoader.hpp"

int main(int argc, char** argv) {
  using namespace Qt::StringLiterals;

  const QGuiApplication app(argc, argv);
  KLocalizedString::setApplicationDomain("long-do"_ba);
  QQmlApplicationEngine engine;

  auto locContext = KLocalizedContext{&engine};

  engine.rootContext()->setContextObject(&locContext);

  auto pageLoader = longdo::MainPageLoader{QUrl{u"qrc:/qt/qml/long_do/main.qml"_s}};
  pageLoader.load(engine);

  return QGuiApplication::exec();
}
