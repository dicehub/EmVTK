#include "emvtkRenderWindowInteractor.h"
#include "vtkObjectFactory.h"
#include <cmath>
#include <emscripten.h>
#include <vector>

vtkStandardNewMacro(emvtkRenderWindowInteractor)

void emvtkRenderWindowInteractor::Tick(void *arg)
{
    emvtkRenderWindowInteractor *interactor = static_cast<emvtkRenderWindowInteractor *>(arg);
    SDL_Event event;
    std::vector<SDL_Event> events;

    // SDL generates continuous sequences of mouse motion events per frame,
    // let use only last event of each sequence

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_MOUSEMOTION && !events.empty() && events.back().type == event.type)
        {
            events.back() = event;
        }
        else
        {
            events.push_back(event);
        }
    }

    for (SDL_Event &ev : events)
    {
        if (interactor->ProcessEvent(ev))
        {
            break;
        }
    }
}

emvtkRenderWindowInteractor::emvtkRenderWindowInteractor()
{
}

emvtkRenderWindowInteractor::~emvtkRenderWindowInteractor()
{
}

int emvtkRenderWindowInteractor::ProcessEvent(const SDL_Event &event)
{
    SDL_Keymod modstates = SDL_GetModState();

    int alt = modstates & (KMOD_LALT | KMOD_RALT) ? 1 : 0;
    int shift = modstates & (KMOD_LSHIFT | KMOD_RSHIFT) ? 1 : 0;
    int ctrl = modstates & (KMOD_LCTRL | KMOD_RCTRL) ? 1 : 0;

    switch (event.type)
    {
    case SDL_QUIT:
    {
        return 1;
    }
    break;
    case SDL_MOUSEMOTION:
    {
        this->MouseX = event.motion.x;
        this->MouseY = event.motion.y;
        this->SetEventInformationFlipY(event.motion.x, event.motion.y, ctrl, shift);
        this->SetAltKey(alt);
        this->InvokeEvent(vtkCommand::MouseMoveEvent, nullptr);
    }
    break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
        this->SetEventInformationFlipY(event.button.x, event.button.y, ctrl, shift);
        this->SetAltKey(alt);

        int ev;

        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            ev = event.button.state == SDL_PRESSED ? vtkCommand::LeftButtonPressEvent
                 : vtkCommand::LeftButtonReleaseEvent;
            break;
        case SDL_BUTTON_MIDDLE:
            ev = event.button.state == SDL_PRESSED ? vtkCommand::MiddleButtonPressEvent
                 : vtkCommand::MiddleButtonReleaseEvent;
            break;
        case SDL_BUTTON_RIGHT:
            ev = event.button.state == SDL_PRESSED ? vtkCommand::RightButtonPressEvent
                 : vtkCommand::RightButtonReleaseEvent;
            break;
        }
        this->InvokeEvent(ev, nullptr);
    }
    break;
    case SDL_MOUSEWHEEL:
    {
        this->SetEventInformationFlipY(this->MouseX, this->MouseY, ctrl, shift);
        this->SetAltKey(alt);
        int ev = event.wheel.y > 0 ? vtkCommand::MouseWheelForwardEvent
                 : vtkCommand::MouseWheelBackwardEvent;
        this->InvokeEvent(ev, nullptr);
    }
    break;
    case SDL_WINDOWEVENT:
    {
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
        {
            this->UpdateSize(event.window.data1, event.window.data2);
            this->Render();
        }
        break;
        }
    }
    break;
    }
    return 0;
}

void emvtkRenderWindowInteractor::StartEventLoop()
{
    emscripten_set_main_loop_arg(emvtkRenderWindowInteractor::Tick, (void *)this, 0, 1);
}


void emvtkRenderWindowInteractor::Enable()
{
    if (this->Enabled)
    {
        return;
    }

    this->Enabled = 1;
    this->Modified();
}


//----------------------------------------------------------------------------
void emvtkRenderWindowInteractor::Disable()
{
    if (!this->Enabled)
    {
        return;
    }

    this->Enabled = 0;
    this->Modified();
}
