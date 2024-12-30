/*
Crafter.WebBuild
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
import Crafter.WebBuild;
import Crafter.Build;
using namespace Crafter::WebBuild;
using namespace Crafter::Build;
namespace fs = std::filesystem;

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
            std::println("Unkown argument: {}, Run crafter-webbuild help for help", argv[i]);
            return 1;
        }
    }

    fs::path projectPath;
    if(path.is_relative()){
        projectPath = fs::current_path()/path;
    }else{
        projectPath = path;
    }

    Project project = Project::LoadFromJSON(projectPath);

    if(command == "serve"){
        Serve(project, configuration, host, port);
    } else if(command == "build"){
        Build(project, configuration);
    } else if(command == "watch"){
        Watch(project, configuration, host, port, projectPath.remove_filename());
    } else if(command == "help"){
        std::println("<watch> Builds the project and hosts it, Automatically rebuilds upon changes.\n<serve> Builds the project and hosts it.\n<build> Only builds the project.\n<help> Displays a help message.\n\n-c The name of the configuration to build.\n-p The path to the project file, defaults to project.json.\n-o Overrides the output folder.\n-P Sets the port of ``serve``, Defaults to: 8080.\n-h Sets the host which ``serve`` will listen to, Defualts to: 0.0.0.0.");
    } else {
         std::println("Invalid command, run help to get help");
    }
}
