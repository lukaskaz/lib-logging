#pragma once

#include "log/factory.hpp"

#include <memory>
#include <string>

namespace logging::storage
{

class Log : public LogIf
{
  public:
    ~Log();

    void log(type, const std::string&) override;
    void setlevel(type);
    std::string info() const override;

  private:
    friend class logging::LogFactory;
    explicit Log(type);
    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace logging::storage
