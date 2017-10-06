#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "erand48.h"
#include "vector.h"

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
#pragma region

inline double reflectance0(double n1, double n2) {
	const double sqrt_R0 = (n1 - n2) / (n1 + n2);
	return sqrt_R0 * sqrt_R0;
}

inline double schlick_reflectance(double n1, double n2, double c) {
	const double R0 = reflectance0(n1, n2);
	return R0 + (1 - R0) * c * c * c * c * c;
}

inline Vector3 ideal_specular_reflect(
	const Vector3 *d, const Vector3 *n) {
	
	const Vector3 _a = mul_dv3(2.0 * dot_v3v3(n, d), n);
	return sub_v3v3(d, &_a);
}

inline Vector3 ideal_specular_transmit(
	const Vector3 *d, const Vector3 *n, 
	double n_out, double n_in, double *pr, 
	unsigned short xseed[3]) {
	
	const Vector3 d_Re = ideal_specular_reflect(d, n);

	const bool out_to_in = dot_v3v3(n, d) < 0;
	const Vector3 nl = out_to_in ? *n : minus_v3(n);
	const double nn = out_to_in ? n_out / n_in : n_in / n_out;
	const double cos_theta = dot_v3v3(d, &nl);
	const double cos2_phi = 1.0 - nn * nn * (1.0 - cos_theta * cos_theta);

	// Total Internal Reflection
	if (cos2_phi < 0) {
		*pr = 1.0;
		return d_Re;
	}

	const Vector3 _a = mul_dv3(nn, d);
	const Vector3 _b = mul_v3d(&nl, (nn * cos_theta + sqrt(cos2_phi)));
	Vector3 d_Tr = sub_v3v3(&_a, &_b);
	normalize_v3(&d_Tr);
	const double c = 1 - (out_to_in ? -cos_theta : dot_v3v3(&d_Tr, n));

	const double Re = schlick_reflectance(n_out, n_in, c);
	const double p_Re = 0.25 + 0.5 * Re;
	if (erand48(xseed) < p_Re) {
		*pr = (Re / p_Re);
		return d_Re;
	}
	else {
		const double Tr = 1 - Re;
		const double p_Tr = 1 - p_Re;
		*pr = (Tr / p_Tr);
		return d_Tr;
	}
}

#pragma endregion