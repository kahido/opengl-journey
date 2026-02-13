# Simple C++ Project Template

## About

This is a C++ GitHub template for getting up and running projects with C++ quickly. It is based on originaly designed and published by leftcus [C++ Best Practices GitHub template](https://github.com/cpp-best-practices/gui_starter_template).

By default (collectively known as `ENABLE_DEVELOPER_MODE`)

 * Address Sanitizer and Undefined Behavior Sanitizer enabled where possible
 * Warnings as errors
 * clang-tidy and cppcheck static analysis

## Getting Started

### Use the Github template

First, click the green `Use this template` button near the top of this page. This will take you to Github's [Generate Repository](https://github.com/kahido/simple-project-structure/generate) page. Fill in a repository name and short description, and click 'Create repository from template'. This will allow you to create a new repository in your Github account, prepopulated with the contents of this project.

After creating the project please wait until the cleanup workflow has finished setting up your project and commited the changes.

## Build Instructions

### (1) Specify the compiler using environment variables

By default (if you don't set environment variables `CC` and `CXX`), the system default compiler will be used.

Conan and CMake use the environment variables CC and CXX to decide which compiler to use. So to avoid the conflict issues only specify the compilers using these variables.

CMake will detect which compiler was used to build each of the Conan targets. If you build all of your Conan targets with one compiler, and then build your CMake targets with a different compiler, the project may fail to build.

### (2) Configure your build

To configure the project, you could use `cmake` or `ccmake`. Each of them are explained in the following:

#### (2.a) Configuring via cmake:
With Cmake directly:

    cmake -S ${SOURCE_DIRECTORY} -B ${BUILD_DIRECTORY}

#### (2.b) Configuring via ccmake:

With the Cmake Curses Dialog Command Line tool:

    ccmake -S ${SOURCE_DIRECTORY} -B ${BUILD_DIRECTORY}

Once `ccmake` has finished setting up, press 'c' to configure the project, press 'g' to generate, and 'q' to quit.

### (3) Build the project

Once you have selected all the options you would like to use, you can build the project (all targets):

    cmake --build ${BUILD_DIRECTORY}

For Visual Studio, give the build configuration (Release, RelWithDeb, Debug, etc) like the following:

    cmake --build ${BUILD_DIRECTORY} -- /p:configuration=Release
