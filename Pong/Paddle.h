#pragma once
#include "Physics/Collider2D.h"
#include <Common.h>
#include <Quad.h>
#include "GameObject.h"
class Paddle : public GameObject
{
public:
	float speed = 0;
	Quad quad;
	BoxCollider2D collider;
	glm::vec2& Size() { return collider.size; }
	glm::vec2& Pos() { return collider.position; }
	Paddle() {}
	bool isAffectedByInput = false;
	void Update() override;
	void RenderPosUpdate() override;
	void SetDefaultTransform(bool isLeft) override;
	void Render() override;
};