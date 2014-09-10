#pragma once
#include "../../Graphics/mesh.h"
#include "BaseComponent.h"
#include "../GameEntity.h"

class RenderComponent: public BaseComponent
{

public:
	RenderComponent(GameEntity* paren, Mesh * _mesh, GLuint _shader, GLuint _texture);
	~RenderComponent();

	void Update();
	void toString();
	void Cleanup();

	void calculateLight();

	bool flipCullFace = false;


private:
	Mesh * mesh;
	GLuint shader_ID;
	GLuint texture_ID;
	float lightRotation = 0;
	glm::vec3 lightDirection;
};

