### Simple example of cone rendering in browser using VTK and Emscripten.

#### Build using docker

Run ```docker build . -t emvtk``` to build emvtk image. Run ```docker run --rm -it -p 6931:6931 emvtk``` to start web server on 6931 port. Result should be accesible by url ```http://localhost:6931```

#### Build without docker

Before build following requirements should be met:

- Installed Emscripten
- Active Emscripten environment (i.e. ```source ./emsdk_env.sh``` for Bash)
- ```CMAKE_TOOLCHAIN_FILE``` environment variable should me set to ```<emsdk install path>/upstream/    emscripten/cmake/Modules/Platform/Emscripten.cmake```
- Compiled VTK, using Emscripten ([how to build VTK](#how-to-build-vtk-using-emscripten))
- ```VTK_BUILD_DIR``` environment variable should me set to VTK build directory

Use ```make build-local``` command to build and run example. Result should be accesible by url ```http://localhost:6931```.

### How to build VTK using Emscripten

Build requires following branch of VTK [```https://github.com/dicehub/VTK/tree/emscripten```](https://github.com/dicehub/VTK/tree/emscripten).

First of all, freetype port should be enabled for Emscripten compiler. It can be done by setting CXXFLAGS environment variable, for example:

        export CXXFLAGS='-s USE_FREETYPE=1'

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

And build it:

        ninja -j "$(nproc)"

In command above, freetype library faked using cmake options ```FREETYPE_INCLUDE_DIRS``` and ```FREETYPE_LIBRARY```. Also, ```getpwuid``` is disabled because currently it missing in Emscripten. Full example can be found in ```Dockerfile```.

