#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main (int argc, char** argv) {
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  const QUrl mainPage {u"qrc:/qt/qml/long_do/main.qml"_qs};
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
		   &app, [&mainPage](QObject *obj, const QUrl &objUrl) {
		     if (!obj && mainPage == objUrl)
		       QCoreApplication::exit(-1);
		   }, Qt::QueuedConnection);
  engine.load(mainPage);

  return app.exec();
}
