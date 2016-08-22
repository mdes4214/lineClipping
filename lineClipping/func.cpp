#include <iostream>
#include <cmath>
#include "func.h"

using namespace std;

Line::Line(double x0i, double y0i, double x1i, double y1i) {
	x0 = x0i;
	y0 = y0i;
	x1 = x1i;
	y1 = y1i;

	vx = x1 - x0;
	vy = y1 - y0;
}

void Line::normalize(void) {
	double len = sqrt(vx * vx + vy * vy);
	vx/= len;
	vy /= len;
	return;
}

Polygon::Polygon(int sni, double *vertices) {
	sideNum = sni;

	vx = new double[sideNum];
	vy = new double[sideNum];
	a = new double[sideNum];
	b = new double[sideNum];
	c = new double[sideNum];

	for (int i = 0; i < sideNum - 1; i++) {
		vx[i] = vertices[2 * (i + 1)] - vertices[2 * i];
		vy[i] = vertices[2 * (i + 1) + 1] - vertices[2 * i + 1];
		a[i] = vy[i];
		b[i] = -vx[i];
		c[i] = vx[i] * vertices[2 * i + 1] - vy[i] * vertices[2 * i];
	}
	vx[sideNum - 1] = vertices[0] - vertices[2 * (sideNum - 1)];
	vy[sideNum - 1] = vertices[1] - vertices[2 * (sideNum - 1) + 1];
	a[sideNum - 1] = vy[sideNum - 1];
	b[sideNum - 1] = -vx[sideNum - 1];
	c[sideNum - 1] = vx[sideNum - 1] * vertices[2 * (sideNum - 1) + 1] - vy[sideNum - 1] * vertices[2 * (sideNum - 1)];
}

double Polygon::distance(int s, double x0, double y0) {
	return a[s] * x0 + b[s] * y0 + c[s];
}

double Polygon::cross(int s, double _vx, double _vy) {
	return _vy * vx[s] - _vx * vy[s];
}

void Polygon::normalize(void) {
	for (int i = 0; i < sideNum; i++) {
		double len = sqrt(vx[i] * vx[i] + vy[i] * vy[i]);
		vx[i] /= len;
		vy[i] /= len;
	}
	return;
}