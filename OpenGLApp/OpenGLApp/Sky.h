#pragma once

#include "MiniEngine/GameObject.h"
#include "simple_shader.h"

class Sky : GameObject
{
public:
	Sky();
	~Sky() = default;
	void Draw();

};

