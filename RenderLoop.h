#pragma once
#include "./IncludesAndDefines.h"
#include "ExceptionLogger.h"
#include "CommandLine.h"

#include "Menu/StartMenu.h"
#include "Multiplayer/Client.h"
#include <WindowResize.h>
#include <OpenGLWrapper/Quad.h>
#include <ScreenSize.h>
#include <Pong/PongGame.h>
#include <OpenGLWrapper/Input/Input.h>
#include "./Multiplayer/Client.h"
GLFWwindow* window;

static int Init()
{
    if (!glfwInit())
        return(-1);
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(Window_Width, Window_Height, "Pong Online", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return(-1);
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initilize glew\n" << endl;
        ASSERT(false);
    }
    Imgui_Start(window);
    return 0;
}

static void TerminateContext()
{
    Imgui_Close();
    glfwTerminate();
}

int RenderLoop(Client& client)
{
    assert(Init() != -1);

    static PongGame* pongGame = 0;
    static bool isInGame = 0,isHost = 0;
    StartMenu menu(client,isInGame,isHost);

    Input::Init(window);
    glfwSetWindowSizeCallback(window, ResizeCallBack);
    {
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);
            Imgui_NewFrame();

            if (isInGame && pongGame == nullptr)
                pongGame = new PongGame(isInGame,isHost);
            else if (isInGame)
                pongGame->Render(),pongGame->UIRender();
            else
                menu.Render();
            
            Imgui_Render();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    TerminateContext();
    return 0;
}