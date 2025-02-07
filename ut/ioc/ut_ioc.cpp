#include <QObject>
#include <QTest>

#include <boost/di.hpp>

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
  virtual ~Interface()                   = 0;
};

Interface::~Interface() = default;

class Interface2 {
public:
  Interface2()                             = default;
  Interface2(const Interface2&)            = default;
  Interface2(Interface2&&)                 = delete;
  Interface2& operator=(const Interface2&) = default;
  Interface2& operator=(Interface2&&)      = delete;
  virtual ~Interface2()                    = 0;
};

Interface2::~Interface2() = default;

class Implementation : public Interface {
};
class Implementation2 : public Interface2 {
};

class User : public long_do::ioc::ServicesUser<Interface> {
public:
  User(DependencyContainer&& cont) : ServicesUser(std::move(cont)) {}

  template<class T>
  const T* service() const {
    return &ServicesUser::service<T>();
  }
};

class User2 : public long_do::ioc::ServicesUser<Interface, Interface2> {
public:
  User2(DependencyContainer&& cont) : ServicesUser(std::move(cont)) {}

  template<class T>
  const T* service() const {
    return &ServicesUser::service<T>();
  }
};

} // namespace

class IocTest : public QObject {
  Q_OBJECT

private slots:
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void shouldCreateWithDependency() {

    auto injector = boost::di::make_injector(boost::di::bind<Interface>().to<Implementation>());

    long_do::ioc::Ioc ioc(std::move(injector));

    auto user       = ioc.make<User>();
    auto* interface = user.service<Interface>();
    QVERIFY(interface);
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void shouldCreateWithDependencies() {
    auto injector = boost::di::make_injector(boost::di::bind<Interface>().to<Implementation>(),
                                             boost::di::bind<Interface2>().to<Implementation2>());
    long_do::ioc::Ioc ioc(std::move(injector));

    auto user       = ioc.make<User2>();
    auto* interface = user.service<Interface>();
    QVERIFY(interface);
    auto* interface2 = user.service<Interface2>();
    QVERIFY(interface2);
  }
};

QTEST_MAIN(IocTest)
#include "ut_ioc.moc"
