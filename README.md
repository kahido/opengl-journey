# OpenGL with GLAD and GLFW - Journey through Gmae Programming

## About

This is a C++ simple project to learn something about OpenGL programming. After some success, we should be able to create own game based on achieved knowlodge.

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
