#pragma once

#include "precomp.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"
#include "Lambertian.h"
#include "Metal.h"
#include "MovingSphere.h"
#include "Dielectric.h"
#include "DiffuseLight.h"
#include "BVH.h"
#include "TestHittable.h"
#include "Ellipsoid.h"

namespace SLL
{
	class Scene
	{
	public:
		Scene() {};

		void LoadScene(HittableList& world, int sceneIndex, Color& background, Point3& lookFrom,
			Point3& lookAt, float& vFov, float& aperture, float& aspectRatio);
	private:
		void RandomWorld(HittableList& world);
		void TwoSpheres(HittableList& world);
		void PerlinSpheres(HittableList& world);
		void Earth(HittableList& world);
		void BVH(HittableList& world);
		void SLL(HittableList& world);
		void RayScene(HittableList& world);
	};

	void Scene::LoadScene(HittableList& world, int sceneIndex, Color& background, Point3& lookFrom,
		Point3& lookAt, float& vFov, float& aperture, float& aspectRatio)
	{
		switch (sceneIndex)
		{
		case 1:
			RandomWorld(world);
			lookFrom = Point3(13.0f, 2.0f, 3.0f);
			lookAt = Point3(0.0f, 0.0f, 0.0f);
			vFov = 20.0f;
			aperture = 0.0f;
			background = Color(0.7f, 0.8f, 1.0f);
			break;

		case 2:
			TwoSpheres(world);
			lookFrom = Point3(13.0f, 2.0f, 3.0f);
			lookAt = Point3(0.0f, 0.0f, 0.0f);
			vFov = 20.0f;
			aperture = 0.0f;
			background = Color(0.7f, 0.8f, 1.0f);
			break;

		case 3:
			PerlinSpheres(world);
			lookFrom = Point3(5.0f, 20.0f, 5.0f);
			lookAt = Point3(-1.5f, 0.5f, 0.0f);
			vFov = 35.0f;
			aperture = 0.1f;
			background = Color(0.7f, 0.8f, 1.0f);
			break;

		case 4:
			Earth(world);
			lookFrom = Point3(13.0f, 2.0f, 3.0f);
			lookAt = Point3(0.0f, 0.0f, 0.0f);
			vFov = 20.0f;
			aperture = 0.00f;
			background = Color(0.7f, 0.8f, 1.0f);
			break;

		case 7:
			BVH(world);
			lookFrom = Point3(5.0f, 4.0f, 5.0f);
			lookAt = Point3(0.0f, 0.0f, 0.0f);
			vFov = 85.0f;
			aperture = 0.0f;
			background = Color(0.7f, 0.8f, 1.0f);
			break;

		case 8:
			SLL(world);
			lookFrom = Point3(-2.0f, 2.0f, 10.0f);
			lookAt = Point3(4.0f, 1.0f, -1.0f);
			vFov = 60.0f;
			aperture = 0.0f;
			background = Color(0.0f, 0.0f, 0.0f);
			break;

		case 9:
			RayScene(world);
			lookFrom = Point3(0.0f, 0.0f, 10.0f);
			lookAt = Point3(0.0f, 0.0f, 0.0f);
			vFov = 45.0f;
			aperture = 0.0f;
			background = Color(0.0f, 0.0f, 0.0f);
			break;
		}
	}

	void Scene::RandomWorld(HittableList& world)
	{
		HittableList objects;
		auto checker = new CheckerTexture(Color(0.05f, 0.05f, 0.05f), Color(0.85f, 0.85f, 0.85f));
		objects.Add(new Sphere(Point3(0, -1000.0f, 0.0f), 1000.0f, new Lambertian(checker)));

		int sphereCount = 11;
		for (int a = -sphereCount; a < sphereCount; a++)
		{
			for (int b = -sphereCount; b < sphereCount; b++)
			{
				auto chooseMat = Random01();
				Point3 center(a + 0.9f * Random01(), 0.2f, b + 0.9f * Random01());

				if ((center - Point3(4.0f, 0.2f, 0.0f)).Magnitude() > 0.9f)
				{
					Material* sphereMat;

					if (chooseMat < 0.8f)
					{
						// Diffuse/Lambertian
						auto albedo = Color::Random() * Color::Random();
						sphereMat = new Lambertian(albedo);
						Point3 center2 = center + Vector3(0.0f, RandomInRange(0.0f, 0.25f), 0.0f);
						objects.Add(new MovingSphere(center, center2, 0.0f, 1.0f, 0.2f, sphereMat));
					}
					else if (chooseMat < 0.95f)
					{
						// Metal
						auto albedo = Color::Random(0.5f, 1.0f);
						auto fuzz = RandomInRange(0.0f, 0.5f);
						sphereMat = new Metal(albedo, fuzz);
						objects.Add(new Sphere(center, 0.2f, sphereMat));
					}
					else
					{
						// Glass
						sphereMat = new Dielectric(1.5f);
						objects.Add(new Sphere(center, 0.2f, sphereMat));
					}
				}
			}
		}

		Material* material1 = new Dielectric(1.5f);
		objects.Add(new Sphere(Point3(0.0f, 1.0f, 0.0f), 1.0f, material1));

		Material* material2 = new Lambertian(Color(0.4f, 0.2f, 0.1f));
		objects.Add(new Sphere(Point3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

		Material* material3 = new Metal(Color(0.7f, 0.6f, 0.5f), 0.0f);
		objects.Add(new Sphere(Point3(4.0f, 1.0f, 0.0f), 1.0f, material3));

		world.Add(new BVHNode(objects, 0.0f, 0.0f));
	}

	void Scene::TwoSpheres(HittableList& world)
	{
		auto checker = new CheckerTexture(Color(1.0f, 1.0f, 1.0f), Color(0.05f, 0.05f, 0.05f));
		world.Add(new Sphere(Point3(0.0f, 10.0f, 0.0f), 10.0f, new Lambertian(checker)));
		world.Add(new Sphere(Point3(0.0f, -10.0f, 0.0f), 10.0f, new Lambertian(checker)));
	}

	void Scene::PerlinSpheres(HittableList& world)
	{
		auto perlinTexture = new NoiseTexture(4.0f);
		auto color1PerlinTexture = new PerlinNoiseTexture(Color(1.0f, 0.5f, 0.5f), 102.0f);
		auto color2PerlinTexture = new PerlinNoiseTexture(Color(0.5f, 0.5f, 1.0f), 2.0f);
		auto color3PerlinTexture = new PerlinNoiseTexture(Color(0.5f, 1.0f, 0.5f), 10.0f);

		world.Add(new Sphere(Point3(0.0f, -1000.0f, 0.0f), 1000.0f, new Lambertian(perlinTexture)));
		world.Add(new Sphere(Point3(0.0f, 2.0f, 4.5f), 2.0f, new Lambertian(color1PerlinTexture)));
		world.Add(new Sphere(Point3(0.0f, 2.0f, -4.5f), 2.0f, new Lambertian(color2PerlinTexture)));
		world.Add(new Sphere(Point3(0.0f, 2.0f, 0.0f), 2.0f, new Lambertian(color3PerlinTexture)));
	}

	Tmpl8::Sprite earthImg{ new Surface("assets/earthmap.png"), 1 };
	void Scene::Earth(HittableList& world)
	{
		auto earth_texture = new SpriteTexture(&earthImg);
		auto earth_surface = new Lambertian(earth_texture);
		world.Add(new Sphere(Point3(0.0f, 0.0f, 0.0f), 2.0f, earth_surface));
	}

	void Scene::BVH(HittableList& world)
	{
		HittableList objects;

		auto checker = new CheckerTexture(Color(0.05f, 0.05f, 0.05f), Color(0.85f, 0.85f, 0.85f));
		objects.Add(new Sphere(Point3(0, -1000.0f, 0.0f), 1000.0f, new Lambertian(checker)));

		int sphereCount = 64;
		for (int a = -sphereCount; a < sphereCount; a++)
		{
			for (int b = -sphereCount; b < sphereCount; b++)
			{
				auto chooseMat = Random01();
				Point3 center(a + 0.9f * Random01(), 0.2f + RandomInRange(0.0f, 4.5f), b + 0.9f * Random01());

				if ((center - Point3(4.0f, 0.2f, 0.0f)).Magnitude() > 0.9f)
				{
					Material* sphereMat;

					if (chooseMat < 0.8f)
					{
						// Diffuse/Lambertian
						auto albedo = Color::Random() * Color::Random();
						sphereMat = new Lambertian(albedo);
						Point3 center2 = center + Vector3(0.0f, RandomInRange(0.0f, 0.25f), 0.0f);
						objects.Add(new MovingSphere(center, center2, 0.0f, 1.0f, 0.2f, sphereMat));
					}
					else if (chooseMat < 0.95f)
					{
						// Metal
						auto albedo = Color::Random(0.5f, 1.0f);
						auto fuzz = RandomInRange(0.0f, 0.5f);
						sphereMat = new Metal(albedo, fuzz);
						objects.Add(new Sphere(center, 0.2f, sphereMat));
					}
					else
					{
						// Glass
						sphereMat = new Dielectric(1.5f);
						objects.Add(new Sphere(center, 0.2f, sphereMat));
					}
				}
			}
		}

		std::cout << objects.objects.size() << std::endl;
		world.Add(new BVHNode(objects, 0.0f, 0.0f));
	}

	void Scene::SLL(HittableList& world)
	{
		int map[5][14]
		{
			{1,1,1,1,0,2,0,0,0,0,3,0,0,0},
			{1,0,0,0,0,2,0,0,0,0,3,0,0,0},
			{1,1,1,1,0,2,0,0,0,0,3,0,0,0},
			{0,0,0,1,0,2,0,0,0,0,3,0,0,0},
			{1,1,1,1,0,2,2,2,2,0,3,3,3,3},
		};

		HittableList objects;
		auto floor = new CheckerTexture(Color(0.01f, 0.01f, 0.01f), Color(0.8f, 0.8f, 0.8f));
		auto floorMat = new Lambertian(floor);
		objects.Add(new Sphere(Point3(0.0f, -2500.5f, 0.0f), 2500.0f, floorMat));

		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 14; x++)
			{
				Material* material;
				if (map[y][x] == 1)
				{
					material = new DiffuseLight(Color(1, 0.811, 0.874));
					objects.Add(new Sphere(Point3(0.0f + float(x) / 1.25f, 5.0f - float(y) / 1.25f, -2.0f), 0.5f, material));
				}
				else if (map[y][x] == 2)
				{
					material = new DiffuseLight(Color(0.996, 0.992, 0.792));
					objects.Add(new Sphere(Point3(0.0f + float(x) / 1.25f, 5.0f - float(y) / 1.25f, -1.25f), 0.5f, material));
				}
				else if (map[y][x] == 3)
				{
					material = new DiffuseLight(Color(0.647, 0.870, 0.898));
					objects.Add(new Sphere(Point3(0.0f + float(x) / 1.25f, 5.0f - float(y) / 1.25f, -1.25f), 0.5f, material));
				}
			}
		}

		for (int i = 0; i < 10000; i++)
		{
			int roll = RandomInRange(0, 100);
			Point3 center = Point3(RandomInRange(-25.0f, 25.0f), 0.075f, RandomInRange(-25.0f, 25.0f));

			if (roll < 55)
			{
				auto lambertian = new Lambertian(Color::Random());
				objects.Add(new Sphere(center, 0.15f, lambertian));
			}
			else if (roll < 85)
			{
				auto metal = new Metal(Color::Random(), 0.1f);
				objects.Add(new Sphere(center, 0.15f, metal));
			}
			else
			{
				auto glass = new Dielectric(1.51f);
				objects.Add(new Sphere(center, 0.15f, glass));
			}
		}

		std::cout << objects.objects.size() << std::endl;
		world.Add(new BVHNode(objects, 0.0f, 0.0f));
	}
	
	void Scene::RayScene(HittableList& world)
	{
		DiffuseLight* Whitelight = new DiffuseLight(Color(0.75f, 0.1f, 0.25f));
		Dielectric* Otherlight = new Dielectric(1.15f);
		world.Add(new Ellipsoid(Point3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 0.75f, 1.0f), 1.0f, Whitelight));
		world.Add(new Ellipsoid(Point3(0.0f, 0.0f, 0.0f), Vector3(0.45f, 1.0f, 1.0f), 1.0f, Otherlight));
	}
}