module;
#include <string>
#include <filesystem>
#include <cstdint>
export module Crafter.WebBuild;
import Crafter.Build;
using namespace Crafter::Build;
namespace fs = std::filesystem;

export namespace Crafter::WebBuild {
    void Build(const Project& project, std::string configuration);
    void Build(const Project& project, std::string configuration, fs::path outputPath);
    void Serve(const Project& project, std::string configuration, std::string host, std::uint16_t port);
}
