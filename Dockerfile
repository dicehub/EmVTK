FROM dockcross/web-wasm:20200119-1c10fb2

RUN export CXXFLAGS="-s USE_FREETYPE=1" && \
    git clone https://github.com/dicehub/VTK.git && \
    cd VTK && \
    git checkout emscripten && \
    cd ../ && \
    mkdir VTK-build && \
    cd VTK-build && \
    cmake \
        -G Ninja \
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
        -DFREETYPE_INCLUDE_DIRS:PATH=include \
        -DFREETYPE_LIBRARY:STRING=freetype \
        -DH5_HAVE_GETPWUID:BOOL=OFF \
        ../VTK && \
        ninja -j "$(nproc)"

COPY ./ ./emvtk

RUN export VTK_BUILD_DIR="../VTK-build" && \
    cd ./emvtk && \
    make build

CMD cd ./emvtk/.build && emrun --hostname 0.0.0.0 --port 6931 --verbose --no_browser index.html