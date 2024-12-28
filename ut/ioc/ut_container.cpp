#include <QObject>
#include <QTest>
#include <memory>
#include <qtestcase.h>

#include "Container.hpp"
#include "ServiceUser.hpp"

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

class Interface2 {
public:
  Interface2()                             = default;
  Interface2(const Interface2&)            = default;
  Interface2(Interface2&&)                 = delete;
  Interface2& operator=(const Interface2&) = default;
  Interface2& operator=(Interface2&&)      = delete;
  virtual ~Interface2()                    = default;
};

class Implementation : public Interface {
public:
  static Implementation* create() { return new Implementation(); }
};

class User : public long_do::ioc::ServicesUser<Interface, Interface2> {
public:
  User(std::shared_ptr<Interface> s1, std::shared_ptr<Interface2> s2) : ServicesLoader(std::move(s1), std::move(s2)) {}

  template<class T>
  T* service() const {
    return ServiceUser<T>::service().get();
  }
};

} // namespace

class ContainerTest : public QObject {
  Q_OBJECT

private slots:
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void shouldReturnNullptrForUnregistered() {
    const long_do::ioc::Container container;
    const std::shared_ptr<Interface> serviceInterface = container.resolveService<Interface>();
    QVERIFY(!serviceInterface);
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void shouldResolveRegisteredServiceByInterace() {
    long_do::ioc::Container container;
    container.registerService<Interface, Implementation>(Implementation::create);
    const std::shared_ptr<Interface> serviceInterface = container.resolveService<Interface>();
    QVERIFY(serviceInterface);
    const std::shared_ptr<Implementation> serviceImplementation =
        std::dynamic_pointer_cast<Implementation>(serviceInterface);
    QVERIFY(serviceImplementation);
    QCOMPARE(serviceImplementation.use_count(), 2);
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void shouldResolveNewServiceOnCall() {
    long_do::ioc::Container container;
    container.registerService<Interface, Implementation>(Implementation::create);
    const std::shared_ptr<Interface> serviceInterface = container.resolveService<Interface>();
    QVERIFY(serviceInterface);
    const std::shared_ptr<Interface> serviceInterface2 = container.resolveService<Interface>();
    QCOMPARE_NE(serviceInterface, serviceInterface2);
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void shouldResolveSingletonByInterface() {
    long_do::ioc::Container container;
    container.registerSingleton<Interface, Implementation>();
    std::shared_ptr<Interface> interface = container.resolveService<Interface>();
    QVERIFY(interface);
    std::shared_ptr<Implementation> implementation = std::dynamic_pointer_cast<Implementation>(interface);
    QVERIFY(implementation);
    QCOMPARE(implementation.use_count(), 3);
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void shouldResolveSameSingleton() {
    long_do::ioc::Container container;
    container.registerSingleton<Interface, Implementation>();
    std::shared_ptr<Interface> interface  = container.resolveService<Interface>();
    std::shared_ptr<Interface> interface2 = container.resolveService<Interface>();
    QVERIFY(interface);
    QCOMPARE(interface, interface2);
  }
};

QTEST_MAIN(ContainerTest)
#include "ut_container.moc"
