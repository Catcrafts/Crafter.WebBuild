/*
 * Crafter.WebBuild
 * Copyright (C) 2024 Catcrafts
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
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
    void Serve(const Project& project, std::string configuration, std::string host, std::uint16_t port, fs::path outputPath);
    void Watch(const Project& project, std::string configuration, std::string host, std::uint16_t port, fs::path watchpath);
    void Watch(const Project& project, std::string configuration, std::string host, std::uint16_t port, fs::path watchpath, fs::path outputPath);
}
