#include <QObject>
#include <QTest>
#include <memory>
#include <qtestcase.h>

#include "Ioc.hpp"

namespace {

class Interface {
public:
  Interface()                            = default;
  Interface(const Interface&)            = default;
  Interface(Interface&&)                 = delete;
  Interface& operator=(const Interface&) = default;
  Interface& operator=(Interface&&)      = delete;
  virtual ~Interface()                   = default;
};

class Implementation : public Interface {};

} // namespace

class IocTest : public QObject {
  Q_OBJECT

private slots:
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void shouldReturnNullptrForUnregistered() {
    const long_do::ioc::Ioc ioc;
    const std::shared_ptr<Interface> serviceInterface = ioc.resolveService<Interface>();
    QVERIFY(!serviceInterface);
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void shouldResolveRegisteredServiceByInterace() {
    long_do::ioc::Ioc ioc;
    ioc.registerService<Interface, Implementation>();
    const std::shared_ptr<Interface> serviceInterface = ioc.resolveService<Interface>();
    QVERIFY(serviceInterface);
    const std::shared_ptr<Implementation> serviceImplementation =
        std::dynamic_pointer_cast<Implementation>(serviceInterface);
    QVERIFY(serviceImplementation);
  }
};

QTEST_MAIN(IocTest)
#include "ut_ioc.moc"
