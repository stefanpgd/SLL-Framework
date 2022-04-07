#pragma once

namespace SLL
{
	class Texture;
	struct HitRecord;

	class Material
	{
	public:
		Material(const Color& color);
		Material(Texture* texture);
		~Material();

		void SetDiffuse(float diff, float spec);
		void SetDiffuse(float diff, float spec, float shine);
		void SetReflectiveness(float reflec);
		void SetTransparency(float trans, float refracIndex, float dense);

		Color GetColor(const HitRecord& record) const;
		
	public:
		bool IsEmmisive = false;
		
		// Phong
		float Diffuse = 0.0f;
		float Specular = 0.0f;
		float Shininess = 15.0f;
		
		// Reflection: Mirrors/Metal
		float Reflectiveness = 0.0f;

		// Refraction: Tansparency/Dielectrics 
		float Transparency = 0.0f;
		float RefractionIndex = 1.0f;
		float Density = 0.03f;

	private:
		Texture* pTexture;
	};
}