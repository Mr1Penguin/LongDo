#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace long_do::ioc {

class Ioc {
private:
  using InterfaceTypeIndex = std::type_index;

public:
  template<class TInterface, std::derived_from<TInterface> TImplementation>
  void registerService() noexcept {
    addServiceConstructor<TImplementation>(typeid(TInterface));
  }

  template<class TService>
  std::shared_ptr<TService> resolveService() const noexcept {
    if (const auto serviceConstructorIt = m_serviceConstructors.find({typeid(TService)});
        serviceConstructorIt != std::ranges::cend(m_serviceConstructors)) {
      return std::shared_ptr<TService>(static_cast<TService*>(serviceConstructorIt->second()));
    }

    return nullptr;
  }

private:
  template<class TImplementation>
  [[nodiscard]] void* make() const noexcept {
    return new TImplementation();
  }

  template<class TImplementation>
  void addServiceConstructor(InterfaceTypeIndex interfaceIndex) {
    auto constructor = std::bind_front(&Ioc::make<TImplementation>, this);
    if (const auto serviceConstructorIt = m_serviceConstructors.find(interfaceIndex);
        serviceConstructorIt != std::ranges::cend(m_serviceConstructors)) {
      serviceConstructorIt->second = std::move(constructor);
    } else {
      m_serviceConstructors.emplace(interfaceIndex, std::move(constructor));
    }
  }

  std::unordered_map<InterfaceTypeIndex, std::move_only_function<void*() const>> m_serviceConstructors;
};

} // namespace long_do::ioc
