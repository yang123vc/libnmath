/*

    This file is part of the nemesis math library.

    sphere.cc
    Sphere

    Copyright (C) 2008, 2010
    Papadopoulos Nikolaos

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General
    Public License along with this library; if not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301 USA

*/

#include <math.h>

#include "sphere.h"

#include "defs.h"
#include "vector.h"
#include "intinfo.h"

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#ifdef __cplusplus
}

Sphere::Sphere()
    : Geometry(GEOMETRY_SPHERE), radius(NMATH_SPHERE_DEFAULT_RADIUS)
{}

Sphere::Sphere(const Vector3 &org, real_t rad)
    : Geometry(GEOMETRY_SPHERE), origin(org), radius(rad > 0 ? rad : NMATH_SPHERE_DEFAULT_RADIUS)
{}

bool Sphere::intersection(const Ray &ray, IntInfo* i_info) const
{

#ifdef NMATH_USE_BBOX_INTERSECTION
	if(!aabb.intersection(ray))
	{
		return false;
	}
#endif

	// We use the algebraic solution
	real_t a = dot(ray.direction, ray.direction);

	real_t b = 2 * ray.direction.x * (ray.origin.x - origin.x) +
		2 * ray.direction.y * (ray.origin.y - origin.y) +
		2 * ray.direction.z * (ray.origin.z - origin.z);

	real_t c = dot(origin, origin) + dot(ray.origin, ray.origin) + 
		2 * dot(-origin, ray.origin) - radius * radius;
	
	real_t discr = (b * b - 4 * a * c);

	if (discr < 0.0) 
	{
		return false;
	}

	real_t sqrt_discr = sqrt(discr);
	real_t t1 = (-b + sqrt_discr) / (2.0 * a);
	real_t t2 = (-b - sqrt_discr) / (2.0 * a);
	
	if (t1 < EPSILON) t1 = t2;
	if (t2 < EPSILON) t2 = t1;
	
	real_t t = t1 < t2 ? t1 : t2;

	if (t < EPSILON )
	{
		return false;
	}

	if (i_info)
	{
		i_info->t = t;
		i_info->point = ray.origin + ray.direction * t;
		i_info->normal = (i_info->point - origin) / radius;
		i_info->geometry = this;
	}
	
	if (dot(i_info->normal, ray.direction) > 0) i_info->normal = -i_info->normal;

	return true;
}

void Sphere::calc_aabb()
{
	aabb.max = origin + Vector3(radius, radius, radius);
	aabb.min = origin - Vector3(radius, radius, radius);
}

#endif	/* __cplusplus */
