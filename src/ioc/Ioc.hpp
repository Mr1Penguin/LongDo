#pragma once

#include <concepts>
#include <memory>

#include "Container.hpp"
#include "ServiceUser.hpp"

namespace long_do::ioc {

class Ioc {
public:
  template<class TInterface, std::derived_from<TInterface> TImplementation>
  void registerService() {
    m_container.registerService<TInterface, TImplementation>(std::bind_front(&Ioc::make<TImplementation>, this));
  }

  template<class TInterface, std::derived_from<TInterface> TImplementation>
  void registerSingleton(auto&&... args) {
    m_container.registerSingleton<TInterface, TImplementation>(std::forward<decltype(args)>(args)...);
  }

  template<HasDependencies T>
  [[nodiscard]] auto make() const noexcept -> T {
    return makeWithDependencies<T>(static_cast<T*>(nullptr));
  }

private:
  template<class TImplementation>
  [[nodiscard]] TImplementation* make() const noexcept {
    return new TImplementation();
  }

  template<HasDependencies T, class... TDependencies>
    requires(std::is_constructible_v<T, std::shared_ptr<TDependencies>...>)
  auto makeWithDependencies(ServicesUser<TDependencies...>* /*deductor>*/) const noexcept -> T {
    return T{m_container.resolveService<TDependencies>()...};
  }

  Container m_container;
};

} // namespace long_do::ioc
