#pragma once

#include <concepts>
#include <functional>
#include <iterator>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace long_do::ioc {

class Container {
public:
  using InterfaceTypeIndex = std::type_index;

  template<class TInterface, std::derived_from<TInterface> TImplementation>
  void registerService(std::move_only_function<void*() const> factory) {
    const InterfaceTypeIndex index{typeid(TInterface)};
    if (m_singletons.contains(index)) {
      m_singletons.erase(index);
    }

    m_serviceConstructors.emplace(index, std::move(factory));
  }

  template<class TInterface, std::derived_from<TInterface> TImplementation>
  void registerSingleton(auto&&... args) {
    const InterfaceTypeIndex index{typeid(TInterface)};
    if (m_serviceConstructors.contains(index)) {
      m_serviceConstructors.erase(index);
    }

    std::shared_ptr instance = std::make_shared<TImplementation>(std::forward<decltype(args)>(args)...);
    m_singletons.emplace(index, std::static_pointer_cast<void>(instance));
  }

  template<class TService>
  std::shared_ptr<TService> resolveService() const noexcept {
    if (const std::input_iterator auto singletonIt = m_singletons.find({typeid(TService)});
        singletonIt != std::ranges::cend(m_singletons)) {
      return std::static_pointer_cast<TService>(singletonIt->second);
    }

    if (const std::input_iterator auto serviceConstructorIt = m_serviceConstructors.find({typeid(TService)});
        serviceConstructorIt != std::ranges::cend(m_serviceConstructors)) {
      return std::shared_ptr<TService>(static_cast<TService*>(serviceConstructorIt->second()));
    }

    return nullptr;
  }

private:
  std::unordered_map<InterfaceTypeIndex, std::move_only_function<void*() const>> m_serviceConstructors;
  std::unordered_map<InterfaceTypeIndex, std::shared_ptr<void>> m_singletons;
};

} // namespace long_do::ioc
