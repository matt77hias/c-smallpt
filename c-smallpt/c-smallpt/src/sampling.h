#pragma once

inline struct Vector3 uniform_sample_on_hemisphere(double u1, double u2) {
	const double sin_theta = sqrt(max(0.0, 1.0 - u1 * u1));
	const double phi = 2.0 * M_PI * u2;
	const struct Vector3 result = { cos(phi) * sin_theta, sin(phi) * sin_theta, u1 };
	return result;
}

inline struct Vector3 cosine_weighted_sample_on_hemisphere(double u1, double u2) {
	const double cos_theta = sqrt(1.0 - u1);
	const double sin_theta = sqrt(u1);
	const double phi = 2.0 * M_PI * u2;
	const struct Vector3 result = { cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta };
	return result;
}