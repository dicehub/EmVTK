#pragma once

#include <vtkRenderWindowInteractor.h>
#include "SDL2/SDL.h"

class emvtkRenderWindowInteractor
    : public vtkRenderWindowInteractor
{
private:
    int MouseX = 0;
    int MouseY = 0;

    static void Tick(void *);
    int ProcessEvent(const SDL_Event &);

protected:
    emvtkRenderWindowInteractor();
    ~emvtkRenderWindowInteractor() override;
    void StartEventLoop() override;
public:
    vtkTypeMacro(emvtkRenderWindowInteractor, vtkRenderWindowInteractor);
    static emvtkRenderWindowInteractor *New();
    void Enable() override;
    void Disable() override;
};