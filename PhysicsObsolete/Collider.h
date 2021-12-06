#pragma once
#include <Common.h>
#include <array>
using glm::vec2;
using glm::vec3;
struct Collider
{
	virtual glm::vec3 FindFurthestPoint(const glm::vec3& dir)const;
};

class MeshCollider2D : public Collider
{
public:
	std::vector<glm::vec2> vertices;
	glm::vec3 FindFurthestPoint(const glm::vec3& dir) const override;
};

struct BoxCollider2D : public Collider
{
	glm::vec2 position;
	glm::vec2 size;
	glm::vec3 FindFurthestPoint(const glm::vec3& dir) const override;
};
struct CircleCollider : public Collider
{
	glm::vec2 position;
	float radius;
	glm::vec3 FindFurthestPoint(const glm::vec3& dir) const override;
};

glm::vec3 Support(const Collider* c1, const Collider* c2, const glm::vec3& dir);

struct Simplex {
private:
	std::array<vec3, 4> m_points;
	unsigned m_size;

public:
	Simplex()
		: m_points()
		, m_size(0)
	{}

	Simplex& operator=(std::initializer_list<vec3> list) {
		for (auto v = list.begin(); v != list.end(); v++) {
			m_points[std::distance(list.begin(), v)] = *v;
		}
		m_size = list.size();

		return *this;
	}

	void push_front(vec3 point) {
		m_points = { point, m_points[0], m_points[1], m_points[2] };
		m_size = std::min(m_size + 1, 4u);
	}

	vec3& operator[](unsigned i) { return m_points[i]; }
	unsigned size() const { return m_size; }

	auto begin() const { return m_points.begin(); }
	auto end()   const { return m_points.end() - (4 - m_size); }
};

bool SameDir(const vec3& dir, const vec3& ao);

bool LineSimplex(Simplex& points, vec3& dir);

bool TriangleSimplex(Simplex& points, vec3& dir);

bool TetrahedronSimplex(Simplex& points, vec3& dir);

bool NextSimplex(Simplex& points, vec3& dir);

bool GJK(const Collider* c1, const Collider* c2);