#pragma once
#include <IncludesAndDefines.h>
#include <Common.h>
#include <ScreenSize.h>
int Window_Width = 800;
int Window_Height = 600;
float AspectR = (float)Window_Height / Window_Width;

void ResizeCallBack(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Window_Height = height;
	Window_Width = width;
	AspectR = (float)Window_Height / Window_Width;
}