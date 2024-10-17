#include <thread>
#include <rolly/rolly.h>
#include <rolly/log/progress_bar.h>

int main(int /*argc*/, char** /*argv*/) {
  auto pb = rolly::log::progress_bar{"Downloading...", 100, fmt::terminal_color::bright_yellow};
  for (auto i = 0; i < 100; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    pb.inc();
  }
  pb.finish();
  return 0;
}