#pragma once

#include "ServiceUser.hpp"

namespace long_do::ioc {

namespace detail {
template<class T>
concept HasDependencies = requires(T obj) { typename T::DepenndencyContainer; };
} // namespace detail

template<class TInjector>
class Ioc {
private:
public:
  Ioc(TInjector injector) : m_injector(std::move(injector)) {}

  template<detail::HasDependencies T>
  [[nodiscard]] auto make() const noexcept -> T {
    return makeWithDependencies<T>(static_cast<T*>(nullptr));
  }

  template<class T>
  [[nodiscard]] auto make() const noexcept -> T {
    return m_injector.template create<T>();
  }

private:
  template<detail::HasDependencies T, class... TDependencies>
    requires(std::is_constructible_v<T, typename ServicesUser<TDependencies...>::DependencyContainer>)
  auto makeWithDependencies(ServicesUser<TDependencies...>* /*deductor*/) const noexcept -> T {
    using DependencyContainer = typename ServicesUser<TDependencies...>::DependencyContainer;
    return T{m_injector.template create<DependencyContainer>};
  }

  TInjector m_injector;
};

template<class TInjector>
Ioc(TInjector) -> Ioc<TInjector>;

} // namespace long_do::ioc
