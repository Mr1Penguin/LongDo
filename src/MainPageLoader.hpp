#include <QObject>
#include <QQmlApplicationEngine>
#include <QUrl>

#pragma once

namespace longdo {

class MainPageLoader : public QObject {
  Q_OBJECT
public:
  explicit MainPageLoader(QUrl pageUrl);

  void load(QQmlApplicationEngine& engine);

private Q_SLOTS:
  void checkLoadedPage(QObject* obj, const QUrl& objUrl);

private:
  QUrl m_pageUrl;
  QMetaObject::Connection m_connection;
};
} // namespace longdo
