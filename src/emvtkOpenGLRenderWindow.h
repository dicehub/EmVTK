#pragma once

#include "vtkGenericOpenGLRenderWindow.h"

#include <SDL2/SDL.h>
#include "EGL/egl.h"

#if __EMSCRIPTEN__
#   define GLFW_INCLUDE_ES3
#   include <GLFW/glfw3.h>
#   include <GLES2/gl2.h>
#endif


class emvtkOpenGLRenderWindow
    : public vtkGenericOpenGLRenderWindow
{
private:
    SDL_Window *Window;
#if EMSCRIPTEN
    SDL_GLContext Context;
#else
    EGLDisplay Display;
    EGLContext Context;
    EGLSurface Surface;
    void ResizeWindow(int x, int y);
#endif

protected:
    emvtkOpenGLRenderWindow();
    ~emvtkOpenGLRenderWindow() override;
public:
    vtkTypeMacro(emvtkOpenGLRenderWindow, vtkGenericOpenGLRenderWindow);
    static emvtkOpenGLRenderWindow *New();
    void Initialize(void) override;
    void SetWindowName(const char *) override;
    void SetSize(int, int) override;
    void Render() override;
    void SwapBuffersOff() override;
};