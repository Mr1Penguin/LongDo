#include <boost/di.hpp>

#include <memory>

namespace {
class Interface {
public:
  virtual ~Interface()   = default;
  virtual void purpose() = 0;
};

} // namespace

void test() {
  const auto injector = boost::di::make_injector();
  injector.create<std::shared_ptr<Interface>>() == nullptr;
}
