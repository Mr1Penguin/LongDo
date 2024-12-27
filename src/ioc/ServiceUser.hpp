#pragma once

#include <memory>

namespace long_do::ioc {

template<class TService>
class ServiceUser {
public:
  ~ServiceUser() = default;

protected:
  ServiceUser()                                  = default;
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
class ServicesUser : public ServiceUser<TServices>... {};

} // namespace long_do::ioc
