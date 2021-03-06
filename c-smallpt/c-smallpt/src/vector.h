#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "math_tools.h"

#pragma endregion

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions: Vector3
//-----------------------------------------------------------------------------
#pragma region

#pragma warning( push )
#pragma warning( disable : 4201 ) // Nameless struct/union.

typedef struct Vector3 {
	union {
		struct {
			double x, y, z;
		};
		double raw[3];
	};
} Vector3;

#pragma warning( pop )

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions: Vector3 Utilities
//-----------------------------------------------------------------------------
#pragma region

inline bool hasNaNs_v3(const Vector3* v) {
	return isnan(v->x) || isnan(v->y) || isnan(v->z);
}

inline Vector3* copy_v3v3(Vector3* v1, const Vector3* v2) {
	v1->x = v2->x;
	v1->y = v2->y;
	v1->z = v2->z;
	return v1;
}

inline const Vector3 minus_v3(const Vector3* v) {
	const Vector3 result = { -v->x, -v->y, -v->z };
	return result;
}
inline const Vector3 add_v3v3(const Vector3* v1, const Vector3* v2) {
	const Vector3 result = { v1->x + v2->x, v1->y + v2->y, v1->z + v2->z };
	return result;
}
inline const Vector3 sub_v3v3(const Vector3* v1, const Vector3* v2) {
	const Vector3 result = { v1->x - v2->x, v1->y - v2->y, v1->z - v2->z };
	return result;
}
inline const Vector3 mul_v3v3(const Vector3* v1, const Vector3* v2) {
	const Vector3 result = { v1->x * v2->x, v1->y * v2->y, v1->z * v2->z };
	return result;
}
inline const Vector3 div_v3v3(const Vector3* v1, const Vector3* v2) {
	const Vector3 result = { v1->x / v2->x, v1->y / v2->y, v1->z / v2->z };
	return result;
}

inline const Vector3 add_v3d(const Vector3* v, double a) {
	const Vector3 result = { v->x + a, v->y + a, v->z + a };
	return result;
}
inline const Vector3 sub_v3d(const Vector3* v, double a) {
	const Vector3 result = { v->x - a, v->y - a, v->z - a };
	return result;
}
inline const Vector3 mul_v3d(const Vector3* v, double a) {
	const Vector3 result = { v->x * a, v->y * a, v->z * a };
	return result;
}
inline const Vector3 div_v3d(const Vector3* v, double a) {
	const double inv_a = 1.0 / a;
	const Vector3 result = { v->x * inv_a, v->y * inv_a, v->z * inv_a };
	return result;
}

inline const Vector3 add_dv3(double a, const Vector3* v) {
	const Vector3 result = { a + v->x, a + v->y, a + v->z };
	return result;
}
inline const Vector3 sub_dv3(double a, const Vector3* v) {
	const Vector3 result = { a - v->x, a - v->y, a - v->z };
	return result;
}
inline const Vector3 mul_dv3(double a, const Vector3* v) {
	const Vector3 result = { a * v->x, a * v->y, a * v->z };
	return result;
}
inline const Vector3 div_dv3(double a, const Vector3* v) {
	const Vector3 result = { a / v->x, a / v->y, a / v->z };
	return result;
}

inline double dot_v3v3(const Vector3* v1, const Vector3* v2) {
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}
inline const Vector3 cross_v3v3(const Vector3* v1, const Vector3* v2) {
	const Vector3 result = { 
		v1->y * v2->z - v1->z * v2->y,                 
		v1->z * v2->x - v1->x * v2->z,                  
		v1->x * v2->y - v1->y * v2->x 
	};
	return result;
}

inline bool eq_v3v3(const Vector3* v1, const Vector3* v2) {
	return v1->x == v2->x && v1->y == v2->y && v1->z == v2->z;
}
inline bool ne_v3v3(const Vector3* v1, const Vector3* v2) {
	return v1->x != v2->x || v1->y != v2->y || v1->z != v2->z;
}
inline bool lt_v3v3(const Vector3* v1, const Vector3* v2) {
	return v1->x < v2->x && v1->y < v2->y && v1->z < v2->z;
}
inline bool le_v3v3(const Vector3* v1, const Vector3* v2) {
	return v1->x <= v2->x && v1->y <= v2->y && v1->z <= v2->z;
}
inline bool gt_v3v3(const Vector3* v1, const Vector3* v2) {
	return v1->x > v2->x && v1->y > v2->y && v1->z > v2->z;
}
inline bool ge_v3v3(const Vector3* v1, const Vector3* v2) {
	return v1->x >= v2->x && v1->y >= v2->y && v1->z >= v2->z;
}

inline const Vector3 sqrt_v3(const Vector3* v) {
	const Vector3 result = { 
		sqrt(v->x),                  
		sqrt(v->y),                   
		sqrt(v->z) 
	};
	return result;
}
inline const Vector3 pow_v3(const Vector3* v, double a) {
	const Vector3 result = { 
		pow(v->x, a),                   
		pow(v->y, a),                   
		pow(v->z, a) 
	};
	return result;
}
inline const Vector3 abs_v3(const Vector3* v) {
	const Vector3 result = { 
		fabs(v->x),                   
		fabs(v->y),                  
		fabs(v->z) 
	};
	return result;
}
inline const Vector3 min_v3v3(const Vector3* v1, const Vector3* v2) {
	const Vector3 result = { 
		fmin(v1->x, v2->x), 				 
		fmin(v1->y, v2->y),				 
		fmin(v1->z, v2->z) 
	};
	return result;
}
inline const Vector3 max_v3v3(const Vector3* v1, const Vector3* v2) {
	const Vector3 result = { 
		fmax(v1->x, v2->x),                 
		fmax(v1->y, v2->y),             
		fmax(v1->z, v2->z) 
	};
	return result;
}
inline const Vector3 round_v3(const Vector3* v) {
	const Vector3 result = { 
		round(v->x),            
		round(v->y),             
		round(v->z) 
	};
	return result;
}
inline const Vector3 floor_v3(const Vector3* v) {
	const Vector3 result = { 
		floor(v->x),                  
		floor(v->y),                  
		floor(v->z) 
	};
	return result;
}
inline const Vector3 ceil_v3(const Vector3* v) {
	const Vector3 result = { 
		ceil(v->x),                      
		ceil(v->y),                 
		ceil(v->z) 
	};
	return result;
}
inline const Vector3 trunc_v3(const Vector3* v) {
	const Vector3 result = { 
		trunc(v->x),          
		trunc(v->y),                 
		trunc(v->z) 
	};
	return result;
}
inline const Vector3 clamp_v3(const Vector3* v, double low, double high) {
	const Vector3 result = { 
		clamp(v->x, low, high),                   
		clamp(v->y, low, high),            
		clamp(v->z, low, high) 
	};
	return result;
}
inline const Vector3 lerp_v3v3(double a, const Vector3* v1, const Vector3* v2) {
	const Vector3 result = { 
		v1->x + a * (v2->x - v1->x),                
		v1->y + a * (v2->y - v1->y),                
		v1->z + a * (v2->z - v1->z) 
	};
	return result;
}
inline const Vector3 permute_v3(const Vector3* v, int x, int y, int z) {
	const Vector3 result = { 
		v->raw[x],                   
		v->raw[y],                 
		v->raw[z] 
	};
	return result;
}

inline int mind_v3(const Vector3* v) {
	return (v->x < v->y && v->x < v->z) ? 0 : ((v->y < v->z) ? 1 : 2);
}
inline int maxd_v3(const Vector3* v) {
	return (v->x > v->y && v->x > v->z) ? 0 : ((v->y > v->z) ? 1 : 2);
}
inline double min_v3(const Vector3* v) {
	return (v->x < v->y && v->x < v->z) ? v->x : ((v->y < v->z) ? v->y : v->z);
}
inline double max_v3(const Vector3* v) {
	return (v->x > v->y && v->x > v->z) ? v->x : ((v->y > v->z) ? v->y : v->z);
}

inline double norm2s_v3(const Vector3* v) {
	return v->x * v->x + v->y * v->y + v->z * v->z;
}
inline double norm2_v3(const Vector3* v) {
	return sqrt(norm2s_v3(v));
}
inline Vector3* normalize_v3(Vector3* v) {
	const double a = 1 / norm2_v3(v);
	v->x *= a;
	v->y *= a;
	v->z *= a;
	return v;
}

inline void print_v3(const Vector3* v) {
	printf("[%f %f, %f]", v->x, v->y, v->z);
}

#pragma endregion