#pragma once

#include <memory>

namespace long_do::ioc {

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
  TService* service() noexcept { return m_service.get(); }
  const TService* service() const noexcept { return m_service.get(); }

private:
  std::shared_ptr<TService> m_service;
};

template<class... TServices>
  requires(sizeof...(TServices) > 0)
class ServicesUser : public ServiceUser<TServices>... {
public:
  using ServicesLoader = ServicesUser<TServices...>;

protected:
  ServicesUser(std::shared_ptr<TServices>... services) : ServiceUser<TServices>(std::move(services))... {}
};

template<class T>
concept HasDependencies = requires(T obj) { typename T::ServicesLoader; };

} // namespace long_do::ioc
