
#include "emvtkOpenGLRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkObjectFactory.h"
#include "vtkOpenGLFramebufferObject.h"
#include <GLES3/gl3.h>

vtkStandardNewMacro(emvtkOpenGLRenderWindow);

emvtkOpenGLRenderWindow::emvtkOpenGLRenderWindow()
    : Window(nullptr)
    , Context(nullptr)
{
    this->SetStencilCapable(1);
}

emvtkOpenGLRenderWindow::~emvtkOpenGLRenderWindow()
{
    if (this->Window)
    {
        SDL_DestroyWindow(this->Window);
        this->Window = nullptr;
    }
}

void emvtkOpenGLRenderWindow::SwapBuffersOff()
{
    // OpenGL ES not using double buffering,
    // so vtkHardwareSelector should use offscreen buffer
    this->Superclass::SwapBuffersOff();
    this->SetUseOffScreenBuffers(1);
    this->Start();
    auto size = this->GetSize();
    this->GetOffScreenFramebuffer()->Resize(size[0], size[1]);
}

void emvtkOpenGLRenderWindow::Render()
{
    this->SetUseOffScreenBuffers(this->SwapBuffers ? 0 : 1);
    this->Superclass::Render();
    if (this->SwapBuffers)
    {
        SDL_GL_SwapWindow(this->Window);
    }
}

void emvtkOpenGLRenderWindow::SetSize(int x, int y)
{
    if ((this->Size[0] != x) || (this->Size[1] != y))
    {
        this->Superclass::SetSize(x, y);

        if (this->Interactor)
        {
            this->Interactor->SetSize(x, y);
        }
        if (this->Window)
        {
            SDL_SetWindowSize(this->Window, x, y);
        }
    }
}

void emvtkOpenGLRenderWindow::SetWindowName(const char *title)
{
    this->Superclass::SetWindowName(title);
    if (this->Window)
    {
        SDL_SetWindowTitle(this->Window, title);
    }
}

void emvtkOpenGLRenderWindow::Initialize()
{
    if (!this->Window)
    {
        int x = ((this->Position[0] >= 0) ? this->Position[0] : SDL_WINDOWPOS_CENTERED);
        int y = ((this->Position[1] >= 0) ? this->Position[1] : SDL_WINDOWPOS_CENTERED);
        int height = ((this->Size[1] > 0) ? this->Size[1] : 300);
        int width = ((this->Size[0] > 0) ? this->Size[0] : 300);
        this->SetSize(width, height);

        SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");

        int res = SDL_Init(SDL_INIT_VIDEO);

        if (res)
        {
            fprintf(stderr, "Error: %s\n", SDL_GetError());
        }

        this->Window = SDL_CreateWindow(this->WindowName,
                                        x, y, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        SDL_SetWindowResizable(this->Window, SDL_TRUE);

    }
    if (!this->Context)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        this->Context = SDL_GL_CreateContext(this->Window);
    }
    this->OpenGLInit();
}
