import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps
from conan.tools.build import check_min_cppstd
from conan.tools.files import rmdir


class RLogRecipe(ConanFile):
    name = "rolly-log"
    version = "2.0.0"
    description = "Small logging facade library based on fmt and spdlog"
    author = "whs31 <whs31@github.io>"
    topics = ("coreutils", "utility", "logging")
    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "test": [True, False],
        "pybind": [True, False],
        "compat": [True, False]
    }
    default_options = {
        "shared": True,
        "test": False,
        "pybind": False,
        "compat": False
    }

    exports_sources = "*"

    @property
    def _min_cppstd(self):
        return "17" if self.options.compat else "20"

    def requirements(self):
        self.requires("spdlog/[>=1.14.0]", transitive_headers=True, transitive_libs=True)
        self.requires("rolly/[>=2.0.1]", transitive_headers=True, transitive_libs=True)
        if self.options.pybind:
            self.requires("pybind11/[>=2.6.1]")

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            check_min_cppstd(self, self._min_cppstd)

    def configure(self):
        self.options["spdlog/*"].shared = self.options.shared
        self.options["rolly/*"].shared = self.options.shared
        self.options["rolly/*"].compat = self.options.compat

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.cache_variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.cache_variables["ROLLY_LOG_TEST"] = self.options.test
        tc.cache_variables["ROLLY_LOG_PYBIND"] = self.options.pybind
        tc.cache_variables["ROLLY_LOG_COMPAT"] = self.options.compat
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        rmdir(self, os.path.join(self.package_folder, "res"))
        rmdir(self, os.path.join(self.package_folder, "share"))

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "log")
        self.cpp_info.set_property("cmake_target_name", "rolly::log")
        self.cpp_info.libs = ["log"]
        self.cpp_info.requires = ["spdlog::spdlog", "rolly::rolly"]
        if self.options.pybind:
            self.cpp_info.requires.append("pybind11::pybind11")
