#pragma once

#define M_GAMMA 2.2

inline void write_ppm(int w, int h, const struct Vector3 *Ls, const char *fname) {
	FILE *fp;
	fopen_s(&fp, fname, "w");
	fprintf(fp, "P3\n%d %d\n%d\n", w, h, 255);
	for (size_t i = 0; i < w * h; ++i)
		fprintf(fp, "%d %d %d ", to_byte(Ls[i].x, M_GAMMA), to_byte(Ls[i].y, M_GAMMA), to_byte(Ls[i].z, M_GAMMA));
	fclose(fp);
}