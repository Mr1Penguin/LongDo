#include "MainPageLoader.hpp"

#include <QCoreApplication>

namespace longdo {

MainPageLoader::MainPageLoader(QUrl pageUrl) : m_pageUrl(std::move(pageUrl)) {}

void MainPageLoader::load(QQmlApplicationEngine& engine) {
  m_connection = QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, this,
                                  &MainPageLoader::checkLoadedPage, Qt::QueuedConnection);
  engine.load(m_pageUrl);
}

void MainPageLoader::checkLoadedPage(QObject* obj, const QUrl& objUrl) {
  if (objUrl != m_pageUrl) {
    return;
  }

  if (obj == nullptr) {
    QCoreApplication::exit(-1);
  }

  qDebug() << "Loaded main page";
  QObject::disconnect(m_connection);
}

} // namespace longdo
