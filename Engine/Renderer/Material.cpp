#include "Material.h"

Material::Material()
{
	myAlbedo.LoadTexture(&Colors::MissingTextureColor, 1, 1);
	myRoughnessMap.LoadTexture(&Colors::UnhandledTextureColor, 1, 1);
	myMetalnessMap.LoadTexture(&Colors::UnhandledTextureColor, 1, 1);
	myNormalMap.LoadTexture(&Colors::UnhandledTextureColor, 1, 1);
}

void Material::SetAlbedo(const char* aFileName)
{
	myAlbedo.LoadTexture(aFileName);
}

void Material::SetRoughness(const char* aFileName)
{
	myRoughnessMap.LoadTexture(aFileName);
}

void Material::SetMetalness(const char* aFileName)
{
	myMetalnessMap.LoadTexture(aFileName);
}

void Material::SetNormal(const char* aFileName)
{
	myNormalMap.LoadTexture(aFileName);
}
