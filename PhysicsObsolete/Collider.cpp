#include "Collider.h"
#include "..\PhysicsObsolete\Collider.h"

glm::vec3 MeshCollider2D::FindFurthestPoint(const glm::vec3& dir) const
{
	vec2 maxPoint;
	float maxDistance = -FLT_MAX;

	for (vec2 vertex : vertices) {
		float distance = dot(vertex,vec2(dir));
		if (distance > maxDistance) {
			maxDistance = distance;
			maxPoint = vertex;
		}
	}
	return vec3(maxPoint,0);
}

glm::vec3 BoxCollider2D::FindFurthestPoint(const glm::vec3& dir) const
{
	float height_o2 = size.y / 2;
	float width_o2 = size.y / 2;

	vec2 vertices[] = {
		{ position + vec2(0,height_o2) },
		{ position + vec2(0,-height_o2) },
		{ position + vec2(width_o2,0) },
		{ position + vec2(-width_o2,0) },
	};
	float mxDot = -FLT_MAX; float d;
	int mx;
	for (int i = 0; i<4; i++)
		if (mxDot < (d = dot(dir, vec3(vertices[i],0))))	
			mxDot = d, mx = i;
	return vec3(vertices[mx],0);
}
glm::vec3 CircleCollider::FindFurthestPoint(const glm::vec3& dir) const
{
	return normalize(dir) * radius;
}

vec3 Support(const Collider* c1, const Collider* c2, const glm::vec3& dir)
{
	return c1->FindFurthestPoint(dir)
		- c2->FindFurthestPoint(-dir);
}

bool SameDir(const vec3& dir, const vec3& ao)
{
	return dot(dir, ao) > 0;
}

bool LineSimplex(Simplex& points, vec3& dir)
{
	vec3 a = points[0];
	vec3 b = points[1];

	vec3 ab = b - a;
	vec3 ao = -a;

	if (SameDir(ab, ao)) {
		dir =cross( cross(ab, ao),ab);
	}

	else {
		points = { a };
		dir = ao;
	}

	return false;
}

bool TriangleSimplex(Simplex& points, vec3& dir)
{
	vec3 a = points[0];
	vec3 b = points[1];
	vec3 c = points[2];

	vec3 ab = b - a;
	vec3 ac = c - a;
	vec3 ao = -a;

	vec3 abc = cross(ab,ac);

	if (SameDir(cross(abc,ac), ao)) {
		if (SameDir(ac, ao)) {
			points = { a, c };
			dir = cross(cross(ac,ao),ac);
		}
		else {
			return LineSimplex(points = { a, b }, dir);
		}
	}
	else {
		if (SameDir(cross(ab,abc), ao)) {
			return LineSimplex(points = { a, b }, dir);
		}

		return true;
	}

	return false;
}

bool TetrahedronSimplex(Simplex& points, vec3& dir)
{
	vec3 a = points[0];
	vec3 b = points[1];
	vec3 c = points[2];
	vec3 d = points[3];

	vec3 ab = b - a;
	vec3 ac = c - a;
	vec3 ad = d - a;
	vec3 ao = -a;

	vec3 abc = cross(ab,ac);
	vec3 acd = cross(ac,ad);
	vec3 adb = cross(ad,ab);

	if (SameDir(abc, ao)) {
		return TriangleSimplex(points = { a, b, c }, dir);
	}

	if (SameDir(acd, ao)) {
		return TriangleSimplex(points = { a, c, d }, dir);
	}

	if (SameDir(adb, ao)) {
		return TriangleSimplex(points = { a, d, b }, dir);
	}

	return true;
}

bool NextSimplex(Simplex& points, vec3& dir)
{
	switch (points.size())
	{
	case 2:	return LineSimplex(points, dir);
	case 3: return TriangleSimplex(points, dir);
	case 4: return TetrahedronSimplex(points, dir);
	}
	return false;
}

bool GJK(const Collider* c1, const Collider* c2)
{
	using namespace glm;
	vec3 support = Support(c1, c2, vec3(1.0f, 0.0f,0));
	Simplex points;
	points.push_front(support);
	vec3 direction = -support;
	while (true)
	{
		support = Support(c1, c2, direction);
		if (dot(support, direction) <= 0)
			return false;
		points.push_front(support);
		if (NextSimplex(points, direction))
			return true;
	}
}


glm::vec3 Collider::FindFurthestPoint(const glm::vec3& dir) const { return { 0,0,0 }; }
