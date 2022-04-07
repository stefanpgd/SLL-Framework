#pragma once

#include "precomp.h"


namespace SLL
{
    class AABB {
    public:
        AABB() {}
        AABB(const vec3& a, const vec3& b) { minimum = a; maximum = b; }

        vec3 GetMin() const;
        vec3 GetMax() const;
        bool Hit(const Ray& r, float t_min, float t_max) const;

    private:
        vec3 minimum;
        vec3 maximum;
    };

    inline AABB SurroundingBox(AABB box0, AABB box1)
    {
        vec3 small(fmin(box0.GetMin().x, box1.GetMin().x),
            fmin(box0.GetMin().y, box1.GetMin().y),
            fmin(box0.GetMin().z, box1.GetMin().z));

        vec3 big(fmax(box0.GetMax().x, box1.GetMax().x),
            fmax(box0.GetMax().y, box1.GetMax().y),
            fmax(box0.GetMax().z, box1.GetMax().z));

        return AABB(small, big);
    }
}