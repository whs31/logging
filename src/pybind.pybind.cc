#include <rolly/log/pybind.h>

#if defined(ROLLY_LOG_PYBIND)
# include <rolly/log/progress_bar.h>

namespace py = pybind11;
namespace rolly::log
{
  void register_(py::module& m) {
    py::class_<progress_bar>(m, "ProgressBar")
      .def(
        py::init<std::string_view, usize>(),
        py::arg("running_color") = "cyan",
        py::arg("done_color") = "green"
      )
      .def_property_readonly("elapsed", [&](progress_bar const& self) { return self.elapsed().count(); })
      .def_property_readonly("current_progress", [&](progress_bar const& self) { return self.current_progress(); })
      .def_property("running_color",
        [&](progress_bar const& self) { return self.running_color_str(); },
        [&](progress_bar& self, std::string_view const color) { self.set_running_color(color); }
      )
      .def_property("done_color",
        [&](progress_bar const& self) { return self.done_color_str(); },
        [&](progress_bar& self, std::string_view const color) { self.set_done_color(color); }
      )
      .def_property("total", &progress_bar::total, &progress_bar::set_total)
      .def("update", &progress_bar::update)
      .def("inc", &progress_bar::inc)
      .def("finish", &progress_bar::finish);
  }
} // namespace rolly::log
#endif // ROLLY_LOG_PYBIND