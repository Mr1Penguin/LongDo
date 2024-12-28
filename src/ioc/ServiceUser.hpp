#pragma once

#include <memory>

namespace long_do::ioc {

namespace detail {

template<class TService>
class ServiceUser {
public:
  ~ServiceUser() = default;

protected:
  ServiceUser() = default;
  ServiceUser(std::shared_ptr<TService> service) : m_service(std::move(service)) {}
  ServiceUser(const ServiceUser&)                = default;
  ServiceUser(ServiceUser&&) noexcept            = default;
  ServiceUser& operator=(const ServiceUser&)     = default;
  ServiceUser& operator=(ServiceUser&&) noexcept = default;
  TService* service() const noexcept { return m_service.get(); }

private:
  std::shared_ptr<TService> m_service;
};

} // namespace detail

template<class... TServices>
  requires(sizeof...(TServices) > 0)
class ServicesUser : private detail::ServiceUser<TServices>... {
public:
  using ServicesLoader = ServicesUser<TServices...>;
  class DependencyContainer {
  public:
    explicit DependencyContainer(std::shared_ptr<TServices>... services)
        : m_dependencies(std::make_tuple(std::move(services)...)) {}

    DependencyContainer(const DependencyContainer&)            = delete;
    DependencyContainer(DependencyContainer&&)                 = default;
    DependencyContainer& operator=(const DependencyContainer&) = delete;
    DependencyContainer& operator=(DependencyContainer&&)      = default;
    ~DependencyContainer()                                     = default;

    friend class ServicesUser;

  private:
    std::tuple<std::shared_ptr<TServices>...> m_dependencies;
  };

protected:
  ServicesUser(DependencyContainer&& container)
      : detail::ServiceUser<TServices>(std::get<std::shared_ptr<TServices>>(std::move(container).m_dependencies))... {}

  template<class T>
  T* service() const noexcept {
    return detail::ServiceUser<T>::service();
  }
};

template<class T>
concept HasDependencies = requires(T obj) { typename T::ServicesLoader; };

} // namespace long_do::ioc
