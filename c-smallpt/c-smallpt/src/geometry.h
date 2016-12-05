#pragma once

struct Ray {
	struct Vector3 o, d;
	double tmin, tmax;
	int depth;
};

inline struct Vector3 eval_r(const struct Ray *r, double t) {
	const struct Vector3 dt = mul_v3d(&r->d, t);
	return add_v3v3(&r->o, &dt);
}

inline void print_r(const struct Ray *r) {
	printf("o: [%f %f %f]\nd: [%f %f %f]\n", r->o.x, r->o.y, r->o.z, r->d.x, r->d.y, r->d.z);
}