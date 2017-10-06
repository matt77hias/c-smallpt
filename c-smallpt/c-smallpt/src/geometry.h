#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "vector.h"

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions: Ray
//-----------------------------------------------------------------------------
#pragma region

typedef struct Ray {
	Vector3 o, d;
	double tmin, tmax;
	int depth;
} Ray;

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions: Ray Utilities
//-----------------------------------------------------------------------------
#pragma region

inline struct Vector3 eval_r(const Ray *r, double t) {
	const struct Vector3 dt = mul_v3d(&r->d, t);
	return add_v3v3(&r->o, &dt);
}

inline void print_r(const Ray *r) {
	printf("o: [%f %f %f]\nd: [%f %f %f]\n", 
		r->o.x, r->o.y, r->o.z, r->d.x, r->d.y, r->d.z);
}

#pragma endregion