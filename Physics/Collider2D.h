#pragma once
#include "Common.h"

struct Collider2D
{
	bool wasHitBefore = false;
protected:
	bool _CollisionStayLogic(bool hasCollided);
	virtual bool Collide(const Collider2D* c2) = 0;
};

struct CircleCollider2D : public Collider2D
{
	glm::vec2 position;
	float radius;
	bool Collide(const Collider2D* c2) override;
};

struct BoxCollider2D : public Collider2D
{
	glm::vec2 position;
	glm::vec2 size;
	bool Collide( const Collider2D* c2) override;
};
