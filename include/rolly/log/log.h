#pragma once

#include <spdlog/spdlog.h>
#include <rolly/log/global/export.h>

namespace rolly::log
{
  namespace impl = spdlog;

  [[nodiscard]] inline impl::logger& log() {
    return *impl::default_logger();
  }

  [[nodiscard]] inline impl::logger& log(std::string_view const name) {
    return *impl::get(name.data());
  }
} // namespace rolly::log