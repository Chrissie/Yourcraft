#include "Texture.h"
#include <GL/freeglut.h>

extern GLuint *textures;


Texture::Texture(char* fileName)
{
}


Texture::~Texture()
{
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, textures[1]);
}
