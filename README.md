# About

Crafter.WebBuild is a C++ build system specifically tailored to web development, it is an extension to [Crafter.Build](https://github.com/Catcrafts/Crafter.Build). Like Crafter.Build it uses JSON project files.

# Install

## Prerequisites:
```
clang 18>
git
Crafter.Build
WASI SDK
```

## Clone and build:
Install the [wasi-sdk](https://github.com/WebAssembly/wasi-sdk?tab=readme-ov-file#install).
```bash
#for x86_64 linux
wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-25/wasi-sdk-25.0-x86_64-linux.tar.gz
tar xvf wasi-sdk-25.0-x86_64-linux.tar.gz
```
then ``nano ~/.bashrc``
```bash
export WASI_SDK_PATH=/insert/install/path/here/wasi-sdk-25.0-x86_64-linux
```

Install Crafter.Build (If you already have Crafter.Build installed you can ignore this step)
```bash
git clone https://github.com/Catcrafts/Crafter.Build.git
cd Crafter.Build
./build.sh
```
(optionally add to path)

Install Crafter.WebBuild
```bash
git clone https://github.com/Catcrafts/Crafter.WebBuild.git
cd Crafter.WebBuild
crafter-build -c release
```
(optionally add to path)

# How to use

## Quickstart
(This can also be seen in the ``sample`` folder)
create a ``project.json`` in an empty folder, open it in your preferred text editor.
Create a basic project file, that describes your web project.
```JSON
{
    "name": "hello-world",
    "configurations": [
        {
            "name": "debug",
            "standard": "c++26",
            "source_files": ["main"],
            "module_files": [],
            "build_dir": "./build",
            "output_dir": "./bin",
            "optimization_level": "0",
            "target": "wasm32-unknown-wasi",
            "type": "executable"
        }
    ]
}

```
Save and close the file, create a ``main.cpp``
```cpp
#include <iostream>
int main() {
    std::cout << "Hello World!" << std::endl;
}
```
Save and close, then run ``crafter-webbuild serve -c debug``. Now you can open the browser at ``http://localhost:8080/`` and ``Hello World!`` will appear in the console.

## CLI commands

``watch`` Builds the project and hosts it, Automatically rebuilds upon changes.

``serve`` Builds the project and hosts it.

``build`` Only builds the project.

``help`` Displays a help message.

## CLI arguments

``-c`` The name of the configuration to build.

``-p`` The path to the project file, defaults to ``project.json``.

``-o`` Overrides the output folder.

``-P`` Sets the port of ``serve``, Defaults to: ``8080``.

``-h`` Sets the host which ``serve`` will listen to, Defualts to: ``0.0.0.0``.

## configuration properties
``name`` Name of the configuration.

``standard`` C++ standard that this configuration uses, please refer to the [relevant clang documentation](https://clang.llvm.org/cxx_status.html)

``source_files`` All source files of the project ``.cpp`` extension is assumed.

``module_files`` All C++ 20 module files of the project ``.cppm`` extension is assumed.

``build_dir`` The directory where intermediate files are stored.

``output_dir`` The directory where the output files will be placed.

``extends`` An array of configuration names that this configuration extends, later elements in the array take priority over previous ones.

``optimization_level`` Please refer to the [relevant clang documentation](https://clang.llvm.org/docs/CommandGuide/clang.html#code-generation-options).

``dependencies`` An object array of the dependencies of this project, example:
```json
"dependencies": [
    {
        "path":"/home/Crafter.Build/project.json",
        "configuration":"debug-lib"
    }
]
```
This will now link the library of Crafter.Build in the configuration you use this in.

``target`` Best to use is ``wasm32-unknown-wasi``, for advanced use please refer to the [relevant clang documentation](https://clang.llvm.org/docs/CrossCompilation.html#target-triple).
