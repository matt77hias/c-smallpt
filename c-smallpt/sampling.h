#pragma once

inline struct Vector3 uniform_sample_on_hemisphere(double u1, double u2) {
	const double r = sqrt(max(0.0, 1.0 - u1 * u1));
	const double phi = 2.0 * M_PI * u2;
	const struct Vector3 result = { r * cos(phi), r * sin(phi), u1 };
	return result;
}