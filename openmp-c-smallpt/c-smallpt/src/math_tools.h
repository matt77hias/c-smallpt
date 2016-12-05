#pragma once

#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.14159265358979323846

inline double clamp(double x, double low, double high) {
	return (x < high) ? ((x > low) ? x : low) : high;
}

inline uint8_t to_byte(double x, double gamma) {
	return (uint8_t)clamp(255.0 * pow(x, 1.0 / gamma), 0.0, 255.0);
}