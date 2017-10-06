#pragma once

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <stdint.h>

#pragma endregion

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#pragma region

#define M_PI 3.14159265358979323846

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
#pragma region

inline double clamp(double x, double low, double high) {
	return (x < high) ? ((x > low) ? x : low) : high;
}

inline uint8_t to_byte(double x, double gamma) {
	return (uint8_t)clamp(255.0 * pow(x, 1.0 / gamma), 0.0, 255.0);
}

#pragma endregion