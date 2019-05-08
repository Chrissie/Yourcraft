#pragma once
#include <string>

class Texture
{
public:
	Texture(char* fileName);
	~Texture();

	void bind();
};

