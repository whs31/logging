/**
 * @file rolly/log/global/export.h
 * @brief Export macros for the <b>rolly</b> library.
 */

#pragma once

#include <rolly/global/export.h>

// NOLINTBEGIN(*-reserved-identifier, *-identifier-naming, *-macro-usage)
#if defined(_WIN32)
# if defined(ROLLY_LOG_LIBRARY)
#   define ___rolly_log_api___ __declspec(dllexport)
# elif defined(ROLLY_LOG_STATIC_LIBRARY)
#   define ___rolly_log_api___
# else // defined(ROLLY_LOG_LIBRARY)
#   define ___rolly_log_api___ __declspec(dllimport)
# endif // defined(ROLLY_LOG_LIBRARY)
#else // defined(_WIN32)
# define ___rolly_log_api___
#endif // defined(_WIN32)

namespace rolly::log {} // namespace rolly::log
// NOLINTEND(*-reserved-identifier, *-identifier-naming, *-macro-usage)