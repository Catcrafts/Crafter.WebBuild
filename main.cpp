/*
Crafter.Build
Copyright (C) 2024 Catcrafts

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
USA
*/

#include <filesystem>
#include <iostream>
#include <string>
#include <format>
#include <print>
#include "httplib.h"
import Crafter.Build;
using namespace Crafter::Build;
namespace fs = std::filesystem;
using namespace httplib;

int main(int argc, char* argv[]) {
    std::string command = std::string(argv[1]);
    fs::path path = "project.json";
    std::string configuration;
    std::string host = "0.0.0.0";
    std::uint16_t port = 8080;
    std::string outputDir;
    for (std::int_fast32_t i = 1; i < argc; i++) {
        std::string arg = std::string(argv[i]);
        if(arg == "-c"){
            configuration = argv[++i];
        } else if(arg == "-p"){
            path = fs::path(argv[++i]);
        } else if(arg == "-h"){
            host = std::string(argv[++i]);
        } else if(arg == "-o"){
            outputDir = argv[++i];
        } else if(arg == "-P"){
            port = std::stoi(argv[++i]);
        } else if(arg[0] == '-'){
            std::println("Unkown argument: {}", argv[i]);
            return 1;
        }
    }

    fs::path projectPath;
    if(path.is_relative()){
        projectPath = fs::current_path()/path;
    }else{
        projectPath = path;
    }

    fs::path configOutputDir;
    Project project = Project::LoadFromJSON(projectPath);
    if(outputDir.empty()){
        project.Build(configuration);
    } else{
        configOutputDir = fs::path(outputDir);
        project.Build(configuration, configOutputDir);
    }

    for (const Configuration& config : project.configurations) {
        if(config.name == configuration){
            configOutputDir = config.outputDir;
            break;
        }
    }

    fs::copy(fs::canonical("/proc/self/exe").parent_path().parent_path()/"runtime.js", configOutputDir);

    if(command == "serve"){
        httplib::Server svr;
        svr.Get("/", [&](const Request& req, Response& res) {
            res.set_content(std::format("<!DOCTYPE html><html><head><title>Crafter.Webbuild development page</title><script src=\"runtime.js\" id=\"runtime-script\" type=\"module\" wasmFile=\"{}.wasm\"></script></head></html>", project.name).c_str(), "text/html");
        });

        svr.set_mount_point("/", configOutputDir);
        std::cout << std::format("Server listening on port: {}\n", port);
        svr.listen(host, port);
    } else if(command != "build"){
        std::println("Invalid command, run help to get help");
    }
}
