#include <QObject>
#include <QTest>
#include <memory>
#include <qtestcase.h>

#include "Ioc.hpp"

class Interface {
public:
  virtual ~Interface() = default;  
};
class Implementation : public Interface {};

class IocTest : public QObject {
  Q_OBJECT

private slots:
  void shouldReturnNullptrForUnregistered() {
    long_do::ioc::Ioc ioc;
    const std::shared_ptr<Interface> serviceInterface = ioc.resolveService<Interface>();
    QVERIFY(!serviceInterface);
  }
  
  void shouldResolveRegisteredServiceByInterace() {
    long_do::ioc::Ioc ioc;
    ioc.registerService<Interface, Implementation>();
    const std::shared_ptr<Interface> serviceInterface = ioc.resolveService<Interface>();
    QVERIFY(serviceInterface);
    const std::shared_ptr<Implementation> serviceImplementation = std::dynamic_pointer_cast<Implementation>(serviceInterface);
    QVERIFY(serviceImplementation);
  }
};

QTEST_MAIN(IocTest);
#include "ut_ioc.moc"
