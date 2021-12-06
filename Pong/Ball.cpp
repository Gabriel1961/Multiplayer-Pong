#include "Ball.h"
using namespace glm;
extern vec2 initialPaddlePosition;
float RandF()
{
	return rand() / RAND_MAX;
}

Ball::Ball()
{
	SetDefaultTransform(false);
}

Ball::Ball(Circle&& circle) 
{
	this->circle = std::move(circle);
	SetDefaultTransform(false);
}

bool Ball::IsOnLeftEdge()
{
	return Pos().x == -initialPaddlePosition.x - 2 * Radius();
}

bool Ball::IsOnRightEdge()
{
	return Pos().x == initialPaddlePosition.x + 2 * Radius();
}

void Ball::Update()
{
	Pos() += speed;
	if (Pos().y < -1 + Radius())
		Pos().y = -1 + Radius(), speed.y = -speed.y;
	else if (Pos().y > 1 - Radius())
		Pos().y = 1 - Radius(), speed.y = -speed.y;
	else if (Pos().x < -initialPaddlePosition.x - 2 * Radius())
		Pos().x = -initialPaddlePosition.x - 2* Radius(), speed = { 0,0 };
	else if (Pos().x > initialPaddlePosition.x + 2* Radius())
		Pos().x = initialPaddlePosition.x + 2* Radius(), speed = { 0,0 };

}

void Ball::SetDefaultTransform(bool isLeft)
{
	const float startSpeed = 0.005;
	Radius() = 0.1;
	Pos() = isLeft ? vec2(-0.5, 0) : vec2(0.5, 0);
	speed = isLeft ? vec2(+startSpeed, 0) : vec2(-startSpeed, 0);
	speed.y = startSpeed * (2 * RandF() - 1);
	circle.transform.Transforms().clear();
	circle.transform.Transforms().push_back(MatrixTransform(glm::mat4(
		Radius(), 0, 0, Pos().x,
		0, Radius(), 0, Pos().y,
		0, 0, 1, 0,
		0, 0, 0, 1
	)));
}

void Ball::RenderPosUpdate()
{
	circle.transform.Transforms()[0].GetMatrix()[0][3] = Pos().x;
	circle.transform.Transforms()[0].GetMatrix()[1][3] = Pos().y;
}

void Ball::Render()
{
	Update();
	RenderPosUpdate();
	circle.Render();
}

