#include <QObject>
#include <QTest>
#include <qtestcase.h>

#include "Ioc.hpp"
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

class Implementation : public Interface {};
class Implementation2 : public Interface2 {};

class User : public long_do::ioc::ServicesUser<Interface> {
public:
  User(DependencyContainer && cont) : ServicesLoader(std::move(cont)) {}

  template<class T>
  T* service() const {
    return ServicesLoader::service<T>();
  }
};

class User2 : public long_do::ioc::ServicesUser<Interface, Interface2> {
public:
  User2(DependencyContainer && cont) : ServicesLoader(std::move(cont)) {}

  template<class T>
  T* service() const {
    return ServicesLoader::service<T>();
  }
};

} // namespace

class IocTest : public QObject {
  Q_OBJECT

private slots:
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void shouldCreateWithDependency() {
    long_do::ioc::Ioc ioc;
    ioc.registerSingleton<Interface, Implementation>();

    auto user       = ioc.make<User>();
    auto* interface = user.service<Interface>();
    QVERIFY(interface);
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void shouldCreateWithDependencies() {
    long_do::ioc::Ioc ioc;
    ioc.registerSingleton<Interface, Implementation>();
    ioc.registerSingleton<Interface2, Implementation2>();

    auto user       = ioc.make<User2>();
    auto* interface = user.service<Interface>();
    QVERIFY(interface);
    auto* interface2 = user.service<Interface2>();
    QVERIFY(interface2);
  }
};

QTEST_MAIN(IocTest)
#include "ut_ioc.moc"
