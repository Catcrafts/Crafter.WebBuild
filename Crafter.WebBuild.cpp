module;
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "httplib.h"
module Crafter.WebBuild;
using namespace Crafter::Build;
using namespace Crafter::WebBuild;
namespace fs = std::filesystem;

void Crafter::WebBuild::Build(const Project& project, std::string configuration, fs::path outputPath){
    project.Build(configuration, outputPath);
    fs::copy(fs::canonical("/proc/self/exe").parent_path().parent_path()/"runtime.js", outputPath);
}

void Crafter::WebBuild::Build(const Project& project, std::string configuration) {
    fs::path outputPath;
    for (const Configuration& config : project.configurations) {
        if(config.name == configuration){
            outputPath = config.outputDir;
            break;
        }
    }
    Crafter::WebBuild::Build(project, configuration, outputPath);
}

void Crafter::WebBuild::Serve(const Project& project, std::string configuration, std::string host, std::uint16_t port) {
    fs::path outputPath;
    for (const Configuration& config : project.configurations) {
        if(config.name == configuration){
            outputPath = config.outputDir;
            break;
        }
    }

    Crafter::WebBuild::Build(project, configuration, outputPath);

    httplib::Server svr;
    svr.Get("/", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_content(std::format("<!DOCTYPE html><html><head><title>Crafter.Webbuild development page</title><script src=\"runtime.js\" id=\"runtime-script\" type=\"module\" wasmFile=\"{}.wasm\"></script></head></html>", project.name).c_str(), "text/html");
    });

    svr.set_mount_point("/", outputPath);
    std::cout << std::format("Server listening on port: {}\n", port);
    svr.listen(host, port);
}
