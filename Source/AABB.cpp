#include "precomp.h"
#include "AABB.h"
#include <algorithm>

namespace SLL
{
    vec3 AABB::GetMin() const { return minimum; }
    vec3 AABB::GetMax() const { return maximum; }

    bool AABB::Hit(const Ray& r, float tMin, float tMax) const
    {
        // Andrew Kensler AABB
        // Source: https://medium.com/@bromanz/another-view-on-the-classic-ray-aabb-intersection-algorithm-for-bvh-traversal-41125138b525
        for (int a = 0; a < 3; a++)
        {
            float invD = 1.0f / r.Direction[a];
            float t0 = (minimum[a] - r.Origin[a]) * invD;
            float t1 = (maximum[a] - r.Origin[a]) * invD;

            if (invD < 0.0f)
            {
                float temp = t1;
                t1 = t0;
                t0 = temp;
            }

            tMin = t0 > tMin ? t0 : tMin;
            tMax = t1 < tMax ? t1 : tMax;

            if (tMax <= tMin)
                return false;
        }

        return true;
    }
}