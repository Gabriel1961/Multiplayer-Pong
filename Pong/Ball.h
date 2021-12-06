#pragma once
#include <Circle.h>
#include "Physics/Collider2D.h"
#include "GameObject.h"
class Ball : public GameObject
{
public:
	Circle circle;
	CircleCollider2D collider;
	glm::vec2& Pos() { return collider.position; }
	float& Radius() { return collider.radius; }
	glm::vec2 speed;
	Ball();
	Ball(Circle&& circle);
	bool IsOnLeftEdge();
	bool IsOnRightEdge();
	void Update() override;
	void SetDefaultTransform(bool isLeft) override;
	void RenderPosUpdate() override;
	void Render() override;
};

