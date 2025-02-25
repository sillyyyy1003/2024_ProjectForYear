#pragma once
#include "Primitive.h"

///@brief This is only test for Geometry Shader
class Triangle:public Primitive
{
public:
	Triangle();
	~Triangle() override = default;

	void Init(const char* _fileName);
	void Init(const std::shared_ptr<Texture>& _Tex);

	void CreateMesh();


};

