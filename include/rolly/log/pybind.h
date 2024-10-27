#pragma once

#include <rolly/log/global/export.h>

#if defined(ROLLY_LOG_PYBIND)
# include <pybind11/pybind11.h>

namespace rolly::log
{
  ___rolly_log_api___ extern void register_(pybind11::module& m);
} // namespace rolly::log
#endif // ROLLY_LOG_PYBIND