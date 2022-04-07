#include "precomp.h"
#include "Material.h"
#include "Texture.h"
#include "SolidColor.h"
#include "Primitive.h"

namespace SLL
{
	Material::Material(const Color& color) : pTexture(new SolidColor(color)) {}
	Material::Material(Texture* texture) : pTexture(texture) {}

	Material::~Material() 
	{
		delete pTexture;
		pTexture = nullptr;
	}

	void Material::SetDiffuse(float diff, float spec)
	{
		Diffuse = diff;
		Specular = spec;
	}

	void Material::SetDiffuse(float diff, float spec, float shine)
	{
		Diffuse = diff;
		Specular = spec;
		Shininess = shine;
	}

	void Material::SetReflectiveness(float reflec)
	{
		Reflectiveness = reflec;
	}

	void Material::SetTransparency(float trans, float refracIndex, float dense)
	{
		Transparency = trans;
		RefractionIndex = refracIndex;
		Density = dense;
	}


	Color Material::GetColor(const HitRecord& record) const
	{
		return pTexture->Value(record.U, record.V, record.Point);
	}
}