/*
 * Crafter.WebBuild
 * Copyright (C) 2024 Catcrafts
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

module;
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <print>
#include <chrono>
#include "httplib.h"
#include <sys/types.h>
#include <sys/inotify.h>
#include <sys/select.h>
module Crafter.WebBuild;
using namespace Crafter::Build;
using namespace Crafter::WebBuild;
namespace fs = std::filesystem;

constexpr int EVENT_SIZE  = sizeof(inotify_event);
constexpr int BUF_LEN =  1024 * ( EVENT_SIZE + 16 );

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
    Build(project, configuration, outputPath);
}

void Crafter::WebBuild::Serve(const Project& project, std::string configuration, std::string host, std::uint16_t port) {
    fs::path outputPath;
    for (const Configuration& config : project.configurations) {
        if(config.name == configuration){
            outputPath = config.outputDir;
            break;
        }
    }

    Serve(project,configuration,host,port,outputPath);
}

void Crafter::WebBuild::Serve(const Project& project, std::string configuration, std::string host, std::uint16_t port, fs::path outputPath) {
    Crafter::WebBuild::Build(project, configuration, outputPath);

    httplib::Server svr;
    svr.Get("/", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_content(std::format("<!DOCTYPE html><html><head><title>Crafter.Webbuild development page</title><script src=\"runtime.js\" id=\"runtime-script\" type=\"module\" wasmFile=\"{}.wasm\"></script></head></html>", project.name).c_str(), "text/html");
    });

    svr.set_mount_point("/", outputPath);
    std::cout << std::format("Server listening on port: {}\n", port);
    svr.listen(host, port);
}

void Crafter::WebBuild::Watch(const Project& project, std::string configuration, std::string host, std::uint16_t port, fs::path watchpath) {
    fs::path outputPath;
    for (const Configuration& config : project.configurations) {
        if(config.name == configuration){
            outputPath = config.outputDir;
            break;
        }
    }

    Watch(project,configuration,host,port,watchpath, outputPath);
}

void Crafter::WebBuild::Watch(const Project& project, std::string configuration, std::string host, std::uint16_t port, fs::path watchpath, fs::path outputPath) {
    std::thread serveThread = std::thread([&project, &configuration, host, port, outputPath](){
       Serve(project, configuration, host, port, outputPath);
    });

    char buffer[BUF_LEN];
    std::println("Watching for changes...");
    int fd = inotify_init();
    int wd = inotify_add_watch(fd, watchpath.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE );
    bool reloadRequested = false;
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        timeval time = {0,0};
        if (select(fd+1, &fds, 0, 0 , &time) != 1 && reloadRequested){
            std::println("Building");
            Build(project, configuration, outputPath);
            std::println("Building complete");
            reloadRequested = false;
        }
        int length = read( fd, buffer, sizeof(inotify_event)*100 );
        int i = 0;
        while ( i < length ) {
            inotify_event* event = reinterpret_cast<inotify_event*>(&buffer[i]);
            if ( event->len && !std::string(event->name).ends_with(".kate-swp")) {
                reloadRequested = true;
            }
            i += EVENT_SIZE + event->len;
        }
        skip:;
    }
    inotify_rm_watch( fd, wd );
    close( fd );
}
