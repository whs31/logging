#include <rolly/log/progress_bar.h>

#include <iostream>
#include <fmt/format.h>
#include <fmt/color.h>

namespace
{
  [[nodiscard]] fmt::terminal_color from_str(std::string_view str) {
    if(str == "black") return fmt::terminal_color::black;
    if(str == "red") return fmt::terminal_color::red;
    if(str == "green") return fmt::terminal_color::green;
    if(str == "yellow") return fmt::terminal_color::yellow;
    if(str == "blue") return fmt::terminal_color::blue;
    if(str == "magenta") return fmt::terminal_color::magenta;
    if(str == "cyan") return fmt::terminal_color::cyan;
    if(str == "white") return fmt::terminal_color::white;
    if(str == "bright_black") return fmt::terminal_color::bright_black;
    if(str == "bright_red") return fmt::terminal_color::bright_red;
    if(str == "bright_green") return fmt::terminal_color::bright_green;
    if(str == "bright_yellow") return fmt::terminal_color::bright_yellow;
    if(str == "bright_blue") return fmt::terminal_color::bright_blue;
    if(str == "bright_magenta") return fmt::terminal_color::bright_magenta;
    if(str == "bright_cyan") return fmt::terminal_color::bright_cyan;
    return fmt::terminal_color::red;
  }

  [[nodiscard]] std::string_view as_str(fmt::terminal_color color) {
    switch(color) {
      case fmt::terminal_color::black: return "black";
      case fmt::terminal_color::red: return "red";
      case fmt::terminal_color::green: return "green";
      case fmt::terminal_color::yellow: return "yellow";
      case fmt::terminal_color::blue: return "blue";
      case fmt::terminal_color::magenta: return "magenta";
      case fmt::terminal_color::cyan: return "cyan";
      case fmt::terminal_color::white: return "white";
      case fmt::terminal_color::bright_black: return "bright_black";
      case fmt::terminal_color::bright_red: return "bright_red";
      case fmt::terminal_color::bright_green: return "bright_green";
      case fmt::terminal_color::bright_yellow: return "bright_yellow";
      case fmt::terminal_color::bright_blue: return "bright_blue";
      case fmt::terminal_color::bright_magenta: return "bright_magenta";
      case fmt::terminal_color::bright_cyan: return "bright_cyan";
      default: return "unknown";
    }
  }
} // namespace

namespace rolly::log
{
  struct progress_bar::impl
  {
    std::chrono::steady_clock::time_point start_time_;
    mutable bool start_time_printed_;
    std::string text_;
    usize total_;
    usize progress_;
    fmt::terminal_color running_color_;
    fmt::terminal_color done_color_;

    impl(
      std::string_view text,
      usize total,
      fmt::terminal_color running_color,
      fmt::terminal_color done_color
    )
      : start_time_(std::chrono::steady_clock::now())
      , start_time_printed_(false)
      , text_(text)
      , total_(total)
      , progress_(0)
      , running_color_(running_color)
      , done_color_(done_color) {
      this->update(0);
    }

    ~impl() {
      if(not this->start_time_printed_)
        this->finish();
    }

    void update(usize const progress) {
      this->progress_ = progress;
      this->print();
    }

    void inc() { this->update(++this->progress_); }
    void set_total(usize const total) { this->total_ = total; }

    void finish() {
      this->start_time_printed_ = true;
      auto const fmt = fmt::format("           \t{: <41}[{:<17}] OK     ({:.3f} c)",
        this->text_,
        fmt::format("{:.2f} %", this->progress()),
        this->elapsed().count()
      );
      fmt::println(stdout, "\r{}", fmt::styled(fmt, fg(this->done_color_)));
      std::fflush(stdout);
    }

    [[nodiscard]] std::chrono::duration<double> elapsed() const {
      return std::chrono::steady_clock::now() - this->start_time_;
    }

    [[nodiscard]] f32 progress() const {
      return static_cast<f32>(this->progress_) / static_cast<f32>(this->total_) * 100.f;
    }

    void print() const {
      auto const fmt = fmt::format("           \t{: <41}[{:<17}]", this->text_, fmt::format("{:.2f} %", this->progress()));
      fmt::print(stdout, "\r{}", fmt::styled(fmt, fg(this->running_color_)));
      std::fflush(stdout);
    }
  };

  progress_bar::progress_bar(
    std::string_view text,
    usize total,
    fmt::terminal_color running_color,
    fmt::terminal_color done_color
  )
    : impl_(make_box<impl>(text, total, running_color, done_color))
  {}

  progress_bar::progress_bar(
    std::string_view text,
    usize total,
    std::string_view running_color,
    std::string_view done_color
  )
    : impl_(make_box<impl>(text, total, ::from_str(running_color), ::from_str(done_color)))
  {}

  progress_bar::~progress_bar() = default;

  std::chrono::duration<double> progress_bar::elapsed() const { return this->impl_->elapsed(); }
  f32 progress_bar::current_progress() const { return this->impl_->progress(); }
  void progress_bar::update(usize const progress) { this->impl_->update(progress); }
  void progress_bar::inc() { this->impl_->inc(); }
  usize progress_bar::total() const { return this->impl_->total_; }
  void progress_bar::set_total(usize const total) { this->impl_->set_total(total); }
  void progress_bar::finish() { this->impl_->finish(); }

  fmt::terminal_color progress_bar::running_color() const { return this->impl_->running_color_; }
  std::string_view progress_bar::running_color_str() const { return ::as_str(this->impl_->running_color_); }
  fmt::terminal_color progress_bar::done_color() const { return this->impl_->done_color_; }
  std::string_view progress_bar::done_color_str() const { return ::as_str(this->impl_->done_color_); }
  void progress_bar::set_running_color(fmt::terminal_color running_color) { this->impl_->running_color_ = running_color; }
  void progress_bar::set_running_color(std::string_view running_color) { this->impl_->running_color_ = ::from_str(running_color); }
  void progress_bar::set_done_color(fmt::terminal_color done_color) { this->impl_->done_color_ = done_color; }
  void progress_bar::set_done_color(std::string_view done_color) { this->impl_->done_color_ = ::from_str(done_color); }
} // namespace rolly::log