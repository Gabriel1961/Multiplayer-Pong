#include "Collider2D.h"
using namespace glm;
namespace Collider2DAlg
{
	/// Return vector perpendicular to v1
	vec2 GetNormal(const vec2& v1)
	{
		return -normalize(vec2(-v1.y,v1.x));
	}
	bool CircleWBox(const CircleCollider2D* c1, const BoxCollider2D* c2)
	{
		vec2 halfsize = c2->size * 0.5f;
		vec2 a = c2->position + vec2(-halfsize.x, -halfsize.y);
		vec2 b = c2->position + vec2(halfsize.x, -halfsize.y);
		vec2 c = c2->position + vec2(halfsize.x, halfsize.y);
		vec2 d = c2->position + vec2(-halfsize.x, halfsize.y);
		vec2 normal;
		int ans = 0;
		normal = GetNormal(b - a);
		if (dot(c1->position - b - normal * c1->radius, normal) < 0)
			ans ++;
		normal = GetNormal(c - b);
		if (dot(c1->position - c - normal * c1->radius, normal) < 0)
			ans++;
		normal = GetNormal(d - c);
		if (dot(c1->position - d  - normal * c1->radius, normal) < 0)
			ans++;
		normal = GetNormal(a - d);
		if (dot(c1->position - a - normal * c1->radius, normal) < 0)
			ans++;
		return ans == 4;
	}
};
bool CircleCollider2D::Collide( const Collider2D* c2)
{
	if (dynamic_cast<const BoxCollider2D*>(c2))
	{
		return Collider2DAlg::CircleWBox(this, (const BoxCollider2D*)c2);
	}
}

bool BoxCollider2D::Collide( const Collider2D* c2)
{
	if (dynamic_cast<const CircleCollider2D*>(c2))
	{
		return Collider2DAlg::CircleWBox((const CircleCollider2D*)c2,this);
	}
}
