### Description

Simple example of a cone rendering in the browser using VTK and Emscripten.

#### Build using docker

Execute to build the emvtk image:

```shell
docker build . -t emvtk
```

Run to start web server on port 6931:

```shell
docker run --rm -it -p 6931:6931 emvtk
```

Result should now be accesible at:

```
http://localhost:6931
```

#### Build without docker

Before the build following requirements should be met:

- Install [Emscripten](https://emscripten.org/)
- Activate Emscripten environment (i.e. ```source ./emsdk_env.sh``` for Bash)
- ```CMAKE_TOOLCHAIN_FILE``` environment variable should be set to ```<emsdk install path>/upstream/    emscripten/cmake/Modules/Platform/Emscripten.cmake```
- Compil VTK, using Emscripten ([how to build VTK](#how-to-build-vtk-using-emscripten))
- ```VTK_BUILD_DIR``` environment variable should be set to VTK build directory

Use ```make build-local``` command to build and run the example. Result should be accesible at: ```http://localhost:6931```.

### How to build VTK using Emscripten

Build requires the following branch of VTK [```https://github.com/dicehub/VTK/tree/emscripten```](https://github.com/dicehub/VTK/tree/emscripten).

First of all, freetype port should be enabled for Emscripten compiler. It can be done by setting the CXXFLAGS environment variable, for example:

```shell
export CXXFLAGS='-s USE_FREETYPE=1'
```

Then, ninja build files should be generated, using ```CMake```:

        cmake -G Ninja \
        -DCMAKE_BUILD_TYPE:STRING=Release \
        -DBUILD_SHARED_LIBS:BOOL=OFF \
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} \
        -DCMAKE_INSTALL_PREFIX:PATH=./install \
        -DBUILD_EXAMPLES:BOOL=OFF \
        -DBUILD_TESTING:BOOL=OFF \
        -DVTK_NO_PLATFORM_SOCKETS:BOOL=ON \
        -DVTK_OPENGL_USE_GLES:BOOL=ON \
        -DModule_vtkRenderingContextOpenGL2:BOOL=OFF \
        -DVTK_USE_SYSTEM_FREETYPE:BOOL=ON \
        -DH5_HAVE_GETPWUID:BOOL=OFF \
        -DFREETYPE_INCLUDE_DIRS:PATH='include' \
        -DFREETYPE_LIBRARY:STRING='freetype' \
        ../VTK

And build it with:

        ninja -j "$(nproc)"

In the command above, freetype library is faked using cmake options ```FREETYPE_INCLUDE_DIRS``` and ```FREETYPE_LIBRARY```. Also, ```getpwuid``` is disabled because currently it is missing in Emscripten. 
Full example can be found in the ```Dockerfile```.

