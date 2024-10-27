#pragma once

#include <chrono>
#include <string>
#include <string_view>
#include <fmt/color.h>
#include <rolly/types/stdint.h>
#include <rolly/traits/pimpl.h>
#include <rolly/traits/pin.h>
#include <rolly/log/global/export.h>

namespace rolly::log
{
  class ___rolly_log_api___ progress_bar final : pin { // NOLINT(*-special-member-functions)
   public:
    using duration_type = std::chrono::duration<double>;

    progress_bar(
      std::string_view text,
      usize total,
      fmt::terminal_color running_color = fmt::terminal_color::cyan,
      fmt::terminal_color done_color = fmt::terminal_color::green
    );
    progress_bar(
      std::string_view text,
      usize total,
      std::string_view running_color,
      std::string_view done_color
    );
    ~progress_bar();

    [[nodiscard]] duration_type elapsed() const;
    [[nodiscard]] f32 current_progress() const;
    [[nodiscard]] usize total() const;
    [[nodiscard]] fmt::terminal_color running_color() const;
    [[nodiscard]] std::string_view running_color_str() const;
    [[nodiscard]] fmt::terminal_color done_color() const;
    [[nodiscard]] std::string_view done_color_str() const;

    void update(usize progress);
    void inc();
    void finish();
    void set_total(usize total);
    void set_running_color(fmt::terminal_color running_color);
    void set_running_color(std::string_view running_color);
    void set_done_color(fmt::terminal_color done_color);
    void set_done_color(std::string_view done_color);

   private:
    struct impl;
    pimpl<struct impl> impl_;
  };
} // namespace rolly::log