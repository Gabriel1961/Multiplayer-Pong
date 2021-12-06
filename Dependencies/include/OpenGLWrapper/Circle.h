#pragma once
#include <Common.h>
#include <Renderer.h>
#include <Transform.h>
#include <string>
static const char* CIRCLE_DEFAULT_SHADER = "Dependencies/include/OpenGLWrapper/Quad.shader";
class Circle
{
protected:

	void _Render()
	{
		if (useTex)
		{
			shader->SetUniform1i("samp", texSlot);
			shader->SetUniform1i("useTex", useTex);
		}
		this->shader->SetUniform4f("color", color);
		this->shader->SetUniformMat4f("transform", scale(transform.GetMatrix(), {AspectR,1,1}));
		Renderer::Draw(*va, *ib, *shader, GL_TRIANGLES);
	}
	void _GenerateVbIbBuffers(int subDivs)
	{
		using namespace glm;
		std::vector<vec2> verts((subDivs+1)*2);
		verts[2*subDivs] = { 0,0 };
		verts[2*subDivs+1] = { 0.5,0.5 };
		float angle = 0;
		float deltaAngle = (2 * Pi) / subDivs;
		for (int i = 0; i < subDivs; i++)
		{
			verts[2*i] = { cos(angle),sin(angle) };
			verts[2 * i + 1] = { (verts[2 * i].x + 1) / 2,(verts[2 * i].y+1) / 2 };
			angle += deltaAngle;
		}
		std::vector<uint> idxes((subDivs+1)*3);
		int i=subDivs-1;
		for (i = 0; i < subDivs-1; i++)
		{
			idxes[3 * i] = subDivs;
			idxes[3*i+1] = i;
			idxes[3 * i + 2] = i + 1;
		}
		idxes[3 * i] = subDivs;
		idxes[3 * i + 1] = subDivs-1;
		idxes[3 * i + 2] = 0;
		vb = new VertexBuffer(verts.size() * 2 * 4, &verts[0]);
		ib = new IndexBuffer(idxes.size(), &idxes[0], GL_UNSIGNED_INT);
	}
public:
	VertexArray* va;
	IndexBuffer* ib;
	VertexBuffer* vb;
	Shader* shader;
	TransformGroup transform;
	glm::vec4 color = glm::vec4(1);
	int texSlot = 0;
	bool useTex = false;


	/// Texture constructor
	Circle(int subDivs,bool useTex, int texSlot, std::string shader = CIRCLE_DEFAULT_SHADER) : useTex(useTex), texSlot(texSlot)
	{
		_GenerateVbIbBuffers(subDivs);
		
		VertexBufferLayout vbl;
		vbl.Push<float>(2);
		vbl.Push<float>(2);
		va = new VertexArray();
		va->AddLayout(*vb, vbl);
		this->shader = new Shader(shader);
	}

	/// <summary>
	/// No texture constructor
	/// </summary>
	/// <param name="color"></param>
	/// <param name="shader">Custom shader can be made 
	/// Req Vertex Shader: uniform mat4 transform; and applying it on vertexCoords
	/// Req Fragment Shader: uniform vec4 color; and applying it to outColor
	/// </param>
	Circle(int subDivs=10,glm::vec4 color = glm::vec4(1), std::string shader = CIRCLE_DEFAULT_SHADER)
		:color(color)
	{
		_GenerateVbIbBuffers(subDivs);

		VertexBufferLayout vbl;
		vbl.Push<float>(2);
		vbl.Push<float>(2);
		va = new VertexArray();
		va->AddLayout(*vb, vbl);
		this->shader = new Shader(shader);
	}

	Circle(Circle&& o) noexcept
	{
		va = o.va;
		ib = o.ib;
		vb = o.vb;
		shader = o.shader;
		transform = o.transform;
		color = o.color;
		texSlot = o.texSlot;
		useTex = o.useTex;
		o.va = 0;
		o.ib = 0;
		o.vb = 0;
		o.shader = 0;
	}

	Circle& operator=(Circle&& o)noexcept
	{
		va = o.va;
		ib = o.ib;
		vb = o.vb;
		shader = o.shader;
		transform = o.transform;
		color = o.color;
		texSlot = o.texSlot;
		useTex = o.useTex;
		o.va = 0;
		o.ib = 0;
		o.vb = 0;
		o.shader = 0;
		return *this;
	}

	virtual void Render()
	{
		_Render();
	}

	~Circle()
	{
		delete va;
		delete ib;
		delete vb;
		delete shader;
	}
};
