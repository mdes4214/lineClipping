#include <iostream>

using namespace std;

class Line {
public:
	Line(double, double, double, double);
	void normalize(void);

	double x0, y0, x1, y1;
	double vx, vy;			// vector
};

class Polygon {
public:
	Polygon(int, double*);

	double distance(int, double, double);
	double cross(int, double, double);
	void normalize(void);

	int sideNum;
	double *vx, *vy;		// vector
	double *a, *b, *c;		// coefficients of line function
};