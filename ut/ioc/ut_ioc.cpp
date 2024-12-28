#include <QObject>
#include <QTest>
#include <memory>
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

class User : public long_do::ioc::ServicesUser<Interface, Interface2> {
public:
  User(std::shared_ptr<Interface> s1, std::shared_ptr<Interface2> s2) : ServicesLoader(std::move(s1), std::move(s2)) {}

  template<class T>
  T* service() const {
    return ServiceUser<T>::service().get();
  }
};

} // namespace

class IocTest : public QObject {
  Q_OBJECT

private slots:
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void shouldCreateWithDependencies() {
    long_do::ioc::Ioc ioc;
    ioc.registerSingleton<Interface, Implementation>();

    static_assert(long_do::ioc::HasDependencies<User>);

    auto user = ioc.make<User>();
  }
};

QTEST_MAIN(IocTest)
#include "ut_ioc.moc"
