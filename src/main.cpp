#include <vtkConeSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include "emvtkRenderWindowInteractor.h"
#include "emvtkOpenGLRenderWindow.h"

int main(int, char *[])
{
    //Create a cone
    vtkSmartPointer<vtkConeSource> coneSource =
        vtkSmartPointer<vtkConeSource>::New();
    coneSource->Update();

    //Create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(coneSource->GetOutputPort());

    vtkSmartPointer<vtkActor> actor =
        vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    //Create a renderer, render window, and interactor
    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<emvtkOpenGLRenderWindow> renderWindow =
        vtkSmartPointer<emvtkOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    renderWindow->SetSize(800, 600);
    vtkSmartPointer<emvtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<emvtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
        vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    renderWindowInteractor->SetInteractorStyle(style);
    style->SetDefaultRenderer(renderer);


    //Add the actors to the scene
    renderer->AddActor(actor);
    renderer->SetBackground(.3, .2, .1); // Background color dark red

    //Render and interact
    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}