from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout


class xroostRecipe(ConanFile):
    name = "xroost"
    version = "0.7.0"

    license = "New BSD License"
    author = "Denis Pronin"
    url = "https://github.com/dpronin/xroost"
    description = "A library with common oriented algorithms"

    settings = "os", "compiler", "build_type", "arch"

    exports_sources = "CMakeLists.txt", "include/*", "LICENSE"

    generators = "CMakeToolchain", "CMakeDeps"

    def build_requirements(self):
        self.tool_requires("cmake/[^3.23]")

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
