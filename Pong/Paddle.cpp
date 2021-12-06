#include "Paddle.h"
#include "./Input/Input.h"
#include <OpenGLWrapper/Transform.h>
glm::vec2 initialPaddlePosition;
void Paddle::Update()
{
	if (isAffectedByInput == false)
		return;
	const float accel = 0.002;
	const float highAccel = 0.007;
	const float maxSpeed = 0.1;

	if (Input::Keyboard::GetKey(GLFW_KEY_W) == 1)
	{
		speed += speed < 0 ? highAccel : accel;
	}
	else if (Input::Keyboard::GetKey(GLFW_KEY_S) == 1)
	{
		speed -= speed > 0 ? highAccel : accel;
	}
	else
		speed = speed *= 0.9;
	Pos().y += std::min(speed, maxSpeed);

	if (Pos().y > 1 - Size().y)
		Pos().y = 1 - Size().y, speed = -speed * 0.5;
	if (Pos().y < -1 + Size().y)
		Pos().y = -1 + Size().y, speed = -speed * 0.5;
}

void Paddle::RenderPosUpdate()
{
	quad.transform.Transforms()[0].GetMatrix()[0][3] = Pos().x;
	quad.transform.Transforms()[0].GetMatrix()[1][3] = Pos().y;
	quad.transform.Transforms()[0].GetMatrix()[0][0] = Size().x;
	quad.transform.Transforms()[0].GetMatrix()[1][1] = Size().y;
}

void Paddle::SetDefaultTransform(bool isLeft)
{
	initialPaddlePosition = { isLeft ? -0.9 : 0.9, 0 };
	const glm::vec2 size = { 0.01,0.1 };
	Size() = size;
	Pos() = initialPaddlePosition;
	quad.transform.Transforms().clear();
	quad.transform.Transforms().push_back(MatrixTransform(glm::mat4(
		size.x, 0, 0, initialPaddlePosition.x,
		0, size.y, 0, initialPaddlePosition.y,
		0, 0, 1, 0,
		0, 0, 0, 1
	)));
}

void Paddle::Render()
{
	Update();
	RenderPosUpdate();
	quad.Render();
}

