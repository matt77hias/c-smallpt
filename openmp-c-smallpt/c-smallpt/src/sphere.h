#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "geometry.h"

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions: Defines
//-----------------------------------------------------------------------------
#pragma region

#define EPSILON_SPHERE 1e-4

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions: Reflection_t
//-----------------------------------------------------------------------------
#pragma region

typedef enum Reflection_t { 
	DIFFUSE, 
	SPECULAR, 
	REFRACTIVE 
} Reflection_t;

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions: Sphere
//-----------------------------------------------------------------------------
#pragma region

typedef struct Sphere {
	double r;
	Vector3 p, e, f; // position, emission, reflection
	Reflection_t reflection_t;
} Sphere;

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions: Sphere Utilities
//-----------------------------------------------------------------------------
#pragma region

bool intersect_sphere(const Sphere *sphere, Ray *ray) {
	// (o + t*d - p) . (o + t*d - p) - r*r = 0
	// <=> (d . d) * t^2 + 2 * d . (o - p) * t + (o - p) . (o - p) - r*r = 0
	// 
	// Discriminant check
	// (2 * d . (o - p))^2 - 4 * (d . d) * ((o - p) . (o - p) - r*r) <? 0
	// <=> (d . (o - p))^2 - (d . d) * ((o - p) . (o - p) - r*r) <? 0
	// <=> (d . op)^2 - 1 * (op . op - r*r) <? 0
	// <=> b^2 - (op . op) + r*r <? 0
	// <=> D <? 0
	//
	// Solutions
	// t = (- 2 * d . (o - p) +- 2 * sqrt(D)) / (2 * (d . d))
	// <=> t = dop +- sqrt(D)

	const Vector3 op = sub_v3v3(&sphere->p, &ray->o);
	const double dop = dot_v3v3(&ray->d, &op);
	const double D = dop * dop - dot_v3v3(&op, &op) + sphere->r * sphere->r;

	if (D < 0) {
		return false;
	}

	const double sqrtD = sqrt(D);

	const double tmin = dop - sqrtD;
	if (ray->tmin < tmin && tmin < ray->tmax) {
		ray->tmax = tmin;
		return true;
	}

	const double tmax = dop + sqrtD;
	if (ray->tmin < tmax && tmax < ray->tmax) {
		ray->tmax = tmax;
		return true;
	}

	return false;
}

#pragma endregion