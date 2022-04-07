#pragma once

#include "precomp.h"

using namespace Tmpl8;

namespace SLL
{
	class Texture
	{
	public:
		virtual ~Texture();
		virtual Color Value(float u, float v, const Point3& p) const = 0;
	};
}