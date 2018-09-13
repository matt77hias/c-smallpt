#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "vector.h"

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
#pragma region

inline const Vector3 uniform_sample_on_hemisphere(double u1, double u2) {
	const double sin_theta = sqrt(fmax(0.0, 1.0 - u1 * u1));
	const double phi = 2.0 * PI * u2;
	const Vector3 result = { 
		cos(phi) * sin_theta, 
		sin(phi) * sin_theta, 
		u1 
	};
	return result;
}

inline const Vector3 cosine_weighted_sample_on_hemisphere(double u1, double u2) {
	const double cos_theta = sqrt(1.0 - u1);
	const double sin_theta = sqrt(u1);
	const double phi = 2.0 * PI * u2;
	const Vector3 result = { 
		cos(phi) * sin_theta, 
		sin(phi) * sin_theta, 
		cos_theta 
	};
	return result;
}

#pragma endregion