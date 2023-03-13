/******************************************************************************
 * Laborator 04 Krivky - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - isvoboda@fit.vutbr.cz
 *
 */

#include "student.h"
#include "globals.h"
#include "main.h"

#include <time.h>
#include <math.h>

//Viz hlavicka vector.h
USE_VECTOR_OF(Point2d, point2d_vec)
#define point2d_vecGet(pVec, i) (*point2d_vecGetPtr((pVec), (i)))

/* Secte dva body Point2d a vysledek vrati v result (musi byt inicializovan a alokovan)*/
void	addPoint2d(const Point2d *a, const Point2d *b, Point2d *result)
{
	IZG_ASSERT(result);
	result->x = a->x+b->x;
	result->y = a->y+b->y;
}

/* Vynasobi bod Point2d skalarni hodnotou typu double a vysledek vrati v result (musi byt inicializovan a alokovan)*/
void	mullPoint2d(double val, const Point2d *p, Point2d *result)
{
	IZG_ASSERT(result);
	result->x = p->x*val;
	result->y = p->y*val;
}

/**
 * Inicializace ridicich bodu horni trajektorie ve vykreslovacim okne.
 * Pocatek souradnicove soustavy je v levem hornim rohu okna. Xova souradnice
 * roste smerem doprava, Y smerem dolu.
 * @param points vystup, kam se pridavaji ridici body
 * @param offset_x posun vsech ridicich bodu v horizontalnim smeru (aby se mohli souradnice zadavat s pocatkem [0,0])
 * @param offset_y posun vsech ridicich bodu ve vertikalnim smeru
 */
void initControlPointsUp(S_Vector **points, int offset_x, int offset_y) {
  *points = vecCreateEmpty(sizeof(Point2d));
  Point2d p;
  p.x = 0;   p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 40;  p.y = -250; point2d_vecPushBack(*points, p);
  p.x = 160; p.y = -250; point2d_vecPushBack(*points, p);
  
  p.x = 200; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 210; p.y = -180; point2d_vecPushBack(*points, p);
  p.x = 350; p.y = -180; point2d_vecPushBack(*points, p);
  
  p.x = 360; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 390; p.y = -120;  point2d_vecPushBack(*points, p);
  p.x = 430; p.y = -120;  point2d_vecPushBack(*points, p);
  
  p.x = 460; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 470; p.y = -70;  point2d_vecPushBack(*points, p);
  p.x = 525; p.y = -70;  point2d_vecPushBack(*points, p);
  
  p.x = 535; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 545; p.y = -40;  point2d_vecPushBack(*points, p);
  p.x = 575; p.y = -40;  point2d_vecPushBack(*points, p);
  
  p.x = 585; p.y = 0;    point2d_vecPushBack(*points, p);

  Point2d offset = {offset_x, offset_y, 1.0};
  for(int i = 0; i < (*points)->size; i++) {
    addPoint2d(point2d_vecGetPtr(*points, i), &offset, point2d_vecGetPtr(*points, i));
  }
}

void initControlPointsDown(S_Vector** points, int offset_x, int offset_y) {

	*points = vecCreateEmpty(sizeof(Point2d));
	Point2d p;
	p.x = 0;   p.y = 0;    point2d_vecPushBack(*points, p);

	p.x = 40;  p.y = -250; point2d_vecPushBack(*points, p);
	p.x = 160; p.y = -250; point2d_vecPushBack(*points, p);  // rovnica priamky = (x2-x1) / (y2-y1)

	p.x = 200; p.y = 0;    point2d_vecPushBack(*points, p);  // slope = 6.25  (40/250)

	p.x = 228.8; p.y = 180; point2d_vecPushBack(*points, p); // x > 210  180/6.25 = 28.8  200+28.8 = 228.8
	p.x = 315; p.y = 180; point2d_vecPushBack(*points, p);   // x < 350  180/-4   = -45   360-45   = 315

	p.x = 360; p.y = 0;    point2d_vecPushBack(*points, p);  // slope = -4  (30/-120)

	p.x = 390; p.y = -120;  point2d_vecPushBack(*points, p);
	p.x = 430; p.y = -120;  point2d_vecPushBack(*points, p);

	p.x = 460; p.y = 0;    point2d_vecPushBack(*points, p);  // slope = 4 (30/120)

	p.x = 477.5; p.y = 70;  point2d_vecPushBack(*points, p); // x > 470  70/4  = 17,5   460 + 17,5 = 477,5
	p.x = 517.5; p.y = 70;  point2d_vecPushBack(*points, p); // x > 525  70/-4 = -17,5  535 - 17,5 = 517,5

	p.x = 535; p.y = 0;    point2d_vecPushBack(*points, p);  // slope = -4  (10/-40)

	p.x = 545; p.y = -40;  point2d_vecPushBack(*points, p);
	p.x = 575; p.y = -40;  point2d_vecPushBack(*points, p);

	p.x = 585; p.y = 0;    point2d_vecPushBack(*points, p);

	Point2d offset = { offset_x, offset_y, 1.0 };
	for (int i = 0; i < (*points)->size; i++) {
		addPoint2d(point2d_vecGetPtr(*points, i), &offset, point2d_vecGetPtr(*points, i));
	}
}

/**
 * Implementace vypoctu Bezierove kubiky.
 * @param P0,P1,P2,P3 ridici body kubiky
 * @param quality pocet bodu na krivke, ktere chceme vypocitat
 * @param trajectory_points vystupni vektor bodu kubiky (nemazat, jen pridavat body)
 */
void bezierCubic(const Point2d *P0, const Point2d *P1, const Point2d *P2, const Point2d *P3, 
  const int quality, S_Vector *trajectory_points) {
  
	double step = 1.0 / quality;
	double B0, B1, B2, B3;
	Point2d R0, R1, R2, R3, Result1, Result2, Final;

	for (double t = 0.0; t <= 1.0; t += step) {

		B0 = pow(1.0 - t, 3);
		B1 = 3.0 * t * pow(1.0 - t, 2);
		B2 = 3.0 * t * t * (1.0 - t);
		B3 = pow(t, 3);

		mullPoint2d(B0, P0, &R0);
		mullPoint2d(B1, P1, &R1);
		mullPoint2d(B2, P2, &R2);
		mullPoint2d(B3, P3, &R3);

		addPoint2d(&R0, &R1, &Result1);
		addPoint2d(&R2, &R3, &Result2);

		addPoint2d(&Result1, &Result2, &Final);

		point2d_vecPushBack(trajectory_points, Final);
	}
}

/* 
 * Implementace vypoctu trajektorie, ktera se sklada z Bezierovych kubik.
 * @param quality pocet bodu krivky, ktere mame urcit
 * @param control_points ridici body krivky 
 * @param trajectory_points vystupni body zakrivene trajektorie 
 */
void	bezierCubicsTrajectory(int quality, const S_Vector* control_points, S_Vector* trajectory_points) {
	// Toto musi byt na zacatku funkce, nemazat.
	point2d_vecClean(trajectory_points);


	// Ziskejte postupne 4 ridici body a pro kazdou ctverici vypocitejte body Bezierovy kubiky.

	for (int i = 0; i < point2d_vecSize(control_points) - 3; i += 3) {  // body = 4 + 3*(n-1) | n = pocet kubik
		Point2d* P0 = point2d_vecGetPtr(control_points, i);
		Point2d* P1 = point2d_vecGetPtr(control_points, i + 1);
		Point2d* P2 = point2d_vecGetPtr(control_points, i + 2);
		Point2d* P3 = point2d_vecGetPtr(control_points, i + 3);
		bezierCubic(P0, P1, P2, P3, quality, trajectory_points);
	}

}
