#pragma once

#include <chrono>
#include <string>
#include <string_view>
#include <fmt/color.h>
#include <rolly/types/stdint.h>
#include <rolly/traits/pimpl.h>
#include <rolly/log/global/export.h>

namespace rolly::log
{
  class progress_bar final {
   public:
    using duration_type = std::chrono::duration<double>;

    progress_bar(
      std::string_view text,
      usize total,
      fmt::terminal_color running_color = fmt::terminal_color::cyan,
      fmt::terminal_color done_color = fmt::terminal_color::green
    );
    ~progress_bar();

    [[nodiscard]] duration_type elapsed() const;
    [[nodiscard]] f32 current_progress() const;
    [[nodiscard]] fmt::terminal_color running_color() const;
    [[nodiscard]] fmt::terminal_color done_color() const;

    void update(usize progress);
    void inc();
    void finish();
    void set_total(usize total);
    void set_running_color(fmt::terminal_color running_color);
    void set_done_color(fmt::terminal_color done_color);

   private:
    struct impl;
    pimpl<struct impl> impl_;
  };
} // namespace rolly::log