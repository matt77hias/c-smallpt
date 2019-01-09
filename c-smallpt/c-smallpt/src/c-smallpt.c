//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "imageio.h"
#include "sampling.h"
#include "specular.h"
#include "sphere.h"

#pragma endregion

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#pragma region

#define REFRACTIVE_INDEX_OUT 1.0
#define REFRACTIVE_INDEX_IN  1.5

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
#pragma region

const Sphere spheres[] = {
	{ 1e5,   { 1e5 + 1.0, 40.8, 81.6 },    { 0.0, 0.0, 0.0 },    { 0.75,0.25,0.25 },      DIFFUSE},	   //Left
	{ 1e5,   { -1e5 + 99.0, 40.8, 81.6 },  { 0.0, 0.0, 0.0 },    { 0.25,0.25,0.75 },      DIFFUSE},	   //Right
	{ 1e5,   { 50.0, 40.8, 1e5 },		   { 0.0, 0.0, 0.0 },    { 0.75, 0.75, 0.75 },    DIFFUSE},	   //Back
	{ 1e5,   { 50.0, 40.8, -1e5 + 170.0 }, { 0.0, 0.0, 0.0 },    { 0.0, 0.0, 0.0 },       DIFFUSE},	   //Front
	{ 1e5,   { 50.0, 1e5, 81.6 },          { 0.0, 0.0, 0.0 },    { 0.75, 0.75, 0.75 },    DIFFUSE},	   //Bottom
	{ 1e5,   { 50.0, -1e5 + 81.6, 81.6 },  { 0.0, 0.0, 0.0 },    { 0.75, 0.75, 0.75 },    DIFFUSE},	   //Top
	{ 16.5,  { 27.0, 16.5, 47.0 },         { 0.0, 0.0, 0.0 },    { 0.999, 0.999, 0.999 }, SPECULAR},   //Mirror
	{ 16.5,  { 73.0, 16.5, 78.0 },         { 0.0, 0.0, 0.0 },    { 0.999, 0.999, 0.999 }, REFRACTIVE}, //Glass
	{ 600.0, { 50.0, 681.6 - .27, 81.6 },  { 12.0, 12.0, 12.0 }, { 0.0, 0.0, 0.0 },       DIFFUSE}	   //Light
};

inline bool intersect(Ray* ray, size_t* id) {
	bool hit = false;
	const size_t n = sizeof(spheres) / sizeof(Sphere);
	for (size_t i = 0u; i < n; ++i) {
		if (intersect_sphere(&spheres[i], ray)) {
			hit = true;
			*id = i;
		}
	}

	return hit;
}

static Vector3 radiance(Ray* ray, unsigned short xseed[3]) {
	Ray* r = ray;
	Vector3 L = { 0.0, 0.0, 0.0 };
	Vector3 F = { 1.0, 1.0, 1.0 };

	while (true) {
		size_t id;
		if (!intersect(r, &id)) {
			return L;
		}

		const Sphere* shape = &spheres[id];
		const Vector3 p = eval_r(r, r->tmax);
		Vector3 n = sub_v3v3(&p, &shape->p);
		normalize_v3(&n);

		const Vector3 l = mul_v3v3(&F, &shape->e);
		L = add_v3v3(&L, &l);
		F = mul_v3v3(&F, &shape->f);

		// Russian roulette
		if (4u < r->depth) {
			const double continue_probability = max_v3(&shape->f);
			if (erand48(xseed) >= continue_probability) {
				return L;
			}
			F = div_v3d(&F, continue_probability);
		}

		// Next path segment
		switch (shape->reflection_t) {
		
		case SPECULAR: {
			r->o = p;
			r->d = ideal_specular_reflect(&r->d, &n);
			r->tmin = EPSILON_SPHERE;
			r->tmax = INFINITY;
			r->depth++;
			break;
		}
		
		case REFRACTIVE: {
			r->o = p;
			double pr;
			r->d = ideal_specular_transmit(&r->d, &n, REFRACTIVE_INDEX_OUT, REFRACTIVE_INDEX_IN, &pr, xseed);
			F = mul_v3d(&F, pr);
			r->tmin = EPSILON_SPHERE;
			r->tmax = INFINITY;
			r->depth++;
			break;
		}
		
		default: {
			const Vector3 w = (0.0 > dot_v3v3(&n, &r->d)) ? n : minus_v3(&n);
			Vector3 _u = { 0.0, 0.0, 0.0 };
			if (fabs(w.x) > 0.1) {
				_u.y = 1.0;
			}
			else {
				_u.x = 1.0;
			}
			Vector3 u = cross_v3v3(&_u, &w);
			normalize_v3(&u);
			const Vector3 v = cross_v3v3(&w, &u);

			const Vector3 sample_d = cosine_weighted_sample_on_hemisphere(erand48(xseed), erand48(xseed));
			const Vector3 _x = mul_dv3(sample_d.x, &u);
			const Vector3 _y = mul_dv3(sample_d.y, &v);
			const Vector3 _z = mul_dv3(sample_d.z, &w);
			const Vector3 _xy = add_v3v3(&_x, &_y);
			Vector3 d = add_v3v3(&_xy, &_z);
			r->o = p;
			r->d = *normalize_v3(&d);;
			r->tmin = EPSILON_SPHERE;
			r->tmax = INFINITY;
			r->depth++;
		}
		}
	}
}

int main(int argc, char* argv[]) {
	const unsigned int nb_samples = (2 == argc) ? atoi(argv[1]) / 4 : 1;

	const unsigned int w = 1024u;
	const unsigned int h = 768u;

	const Vector3 eye = { 50, 52, 295.6 };
	Vector3 gaze = { 0, -0.042612, -1 };
	normalize_v3(&gaze);
	const double fov = 0.5135;
	const Vector3 cx = { w * fov / h, 0.0, 0.0 };
	Vector3 _cy = cross_v3v3(&cx, &gaze);
	normalize_v3(&_cy);
	const Vector3 cy = mul_v3d(&_cy, fov);

	Vector3* Ls = malloc(w * h * sizeof(Vector3));

	for (unsigned int y = 0u; y < h; ++y) { // pixel row
		
		fprintf(stderr, "\rRendering (%u spp) %5.2f%%", nb_samples * 4, 100.0 * y / (h - 1));
		
		unsigned short xseed[3] = { 0, 0, (unsigned short)(y * y * y) };
		for (unsigned int x = 0u; x < w; ++x) { // pixel column
			
			for (unsigned int sy = 0u, i = (h - 1u - y) * w + x; sy < 2u; ++sy) { // 2 subpixel row
				
				for (unsigned int sx = 0u; sx < 2u; ++sx) { // 2 subpixel column
					
					Vector3 L = { 0.0, 0.0, 0.0 };
					
					for (unsigned int s = 0u; s < nb_samples; ++s) { // samples per subpixel
						const double u1 = 2.0 * erand48(xseed);
						const double u2 = 2.0 * erand48(xseed);
						const double dx = (u1 < 1.0f) ? sqrt(u1) - 1.0 : 1.0 - sqrt(2.0 - u1);
						const double dy = (u2 < 1.0f) ? sqrt(u2) - 1.0 : 1.0 - sqrt(2.0 - u2);

						const Vector3 _a = mul_v3d(&cx, (((sx + 0.5 + dx) / 2.0 + x) / w - 0.5));
						const Vector3 _b = mul_v3d(&cy, (((sy + 0.5 + dy) / 2.0 + y) / h - 0.5));
						const Vector3 _ab = add_v3v3(&_a, &_b);
						Vector3 d = add_v3v3(&_ab, &gaze);
						const Vector3 d130 = mul_v3d(&d, 130.0);
						Ray ray = { add_v3v3(&eye, &d130), *normalize_v3(&d), EPSILON_SPHERE, INFINITY, 0 };

						const Vector3 _l = radiance(&ray, xseed);
						const Vector3 l = div_v3d(&_l, (double)nb_samples);
						L = add_v3v3(&L, &l);
					}
					
					const Vector3 _l = clamp_v3(&L, 0.0, 1.0);
					const Vector3 l = mul_dv3(0.25, &_l);
					Ls[i] = add_v3v3(&Ls[i], &l);
				}
			}
		}
	}

	write_ppm(w, h, Ls, "c-image.ppm");

	free(Ls);
}

#pragma endregion
