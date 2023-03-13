/**
 * @file        student.cpp
 * @author      Ladislav Mosner, VUT FIT Brno, imosner@fit.vutbr.cz
 * @author      Petr Kleparnik, VUT FIT Brno, ikleparnik@fit.vutbr.cz
 * @author      Kamil Behun, VUT FIT Brno, ibehun@fit.vutbr.cz
 * @date        11.03.2018
 *
 * @brief       Deklarace funkci studentu. DOPLNUJI STUDENTI
 *
 */

// xkucht11

#include "base.h"
#include "student.h"
#include "globals.h"
#include <math.h>

/**
 * @brief Vraci barvu pixelu z pozice [x, y]
 * @param[in] x X souradnice pixelu
 * @param[in] y Y souradnice pixelu
 * @return Barva pixelu na pozici [x, y] ve formatu RGBA
 */
RGBA getPixel(int x, int y)
{
    if (x >= width || y >= height || x < 0 || y < 0) {
        IZG_ERROR("Pristup do framebufferu mimo hranice okna\n");
    }
    return framebuffer[y * width + x];
}

/**
 * @brief Nastavi barvu pixelu na pozici [x, y]
 * @param[in] x X souradnice pixelu
 * @param[in] y Y souradnice pixelu
 * @param[in] color Barva pixelu ve formatu RGBA
 */
void putPixel(int x, int y, RGBA color)
{
    if (x >= width || y >= height || x < 0 || y < 0) {
        IZG_ERROR("Pristup do framebufferu mimo hranice okna\n");
    }
    framebuffer[y * width + x] = color;
}

/**
 * @brief Vykresli usecku se souradnicemi [x1, y1] a [x2, y2]
 * @param[in] x1 X souradnice 1. bodu usecky
 * @param[in] y1 Y souradnice 1. bodu usecky
 * @param[in] x2 X souradnice 2. bodu usecky
 * @param[in] y2 Y souradnice 2. bodu usecky
 * @param[in] color Barva pixelu usecky ve formatu RGBA
 * @param[in] arrow Priznak pro vykresleni sipky (orientace hrany)
 */
void drawLine(int x1, int y1, int x2, int y2, RGBA color, bool arrow = false)
{

    if (arrow) {
        // Sipka na konci hrany
        double vx1 = x2 - x1;
        double vy1 = y2 - y1;
        double length = sqrt(vx1 * vx1 + vy1 * vy1);
        double vx1N = vx1 / length;
        double vy1N = vy1 / length;
        double vx1NN = -vy1N;
        double vy1NN = vx1N;
        int w = 3;
        int h = 10;
        int xT = (int) (x2 + w * vx1NN - h * vx1N);
        int yT = (int) (y2 + w * vy1NN - h * vy1N);
        int xB = (int) (x2 - w * vx1NN - h * vx1N);
        int yB = (int) (y2 - w * vy1NN - h * vy1N);
        pinedaTriangle(Point(x2, y2), Point(xT, yT), Point(xB, yB), color, color, false);
    }

    bool steep = abs(y2 - y1) > abs(x2 - x1);

    if (steep) {
        SWAP(x1, y1);
        SWAP(x2, y2);
    }

    if (x1 > x2) {
        SWAP(x1, x2);
        SWAP(y1, y2);
    }

    const int dx = x2 - x1, dy = abs(y2 - y1);
    const int P1 = 2 * dy, P2 = P1 - 2 * dx;
    int P = 2 * dy - dx;
    int y = y1;
    int ystep = 1;
    if (y1 > y2) ystep = -1;

    for (int x = x1; x <= x2; x++) {
        if (steep) {
            if (y >= 0 && y < width && x >= 0 && x < height) {
                putPixel(y, x, color);
            }
        } else {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                putPixel(x, y, color);
            }
        }

        if (P >= 0) {
            P += P2;
            y += ystep;
        } else {
            P += P1;
        }
    }
}


void pinedaTriangle(const Point &v1, const Point &v2, const Point &v3, const RGBA &color1, const RGBA &color2, bool arrow) {

    int x_min, y_min, x_max, y_max;  // hranice obálky
    int A1, A2, A3, B1, B2, B3, C1, C2, C3;  // parametry obecné rovnice primky
    int	Edge1, Edge2, Edge3;  // jednotlivé hranové funkce úseček trojuhelníku
    int x, y;  // počítadla v cyklu

  // Nalezeni obalky (x_min, x_max), (y_min, y_max) trojuhleniku.
    x_min = MIN(v1.x, MIN(v2.x, v3.x));
    y_min = MIN(v1.y, MIN(v2.y, v3.y));
    x_max = MAX(v1.x, MAX(v2.x, v3.x));
    y_max = MAX(v1.y, MAX(v2.y, v3.y));

  // Oriznuti obalky (x_min, x_max, y_min, y_max) trojuhleniku podle rozmeru okna.
    x_min = MAX(x_min, 0);
    y_min = MAX(y_min, 0);
    x_max = MIN(x_max, width - 1);
    y_max = MIN(y_max, height - 1);

  // Hodnoty deltaX, deltaY jsou souradnicemi vektoru, ktery ma pocatek v prvnim vrcholu hrany, konec v druhem vrcholu.
  // hranova funkce je obecna rovnice primky Ax + By + C = 0
  // primku prochazejici body (x1, y1) a (x2, y2) urcime podle (http://daniel.microdor.com/LineEquations.html)
  // (y1 - y2)x + (x2 - x1)y + (x1y2 - x2y1) = 0 
  //    (A)x    +    (B)y    +      (C)      = 0
  // Spocitani parametru hranove funkce (deltaX, deltaY) pro kazdou hranu.

  // A
    A1 = v1.y - v2.y;  // (y1 - y2)
    A2 = v2.y - v3.y;  // (y2 - y3)
    A3 = v3.y - v1.y;  // (y3 - y1)

  // B
    B1 = v2.x - v1.x;  // (x2 - x1)
    B2 = v3.x - v2.x;  // (x3 - x2)
    B3 = v1.x - v3.x;  // (x1 - x3)

  // C
    C1 = (v1.x * v2.y) - (v2.x * v1.y);  // x1y2 - x2y1
    C2 = (v2.x * v3.y) - (v3.x * v2.y);  // x2y3 - x3y2
    C3 = (v3.x * v1.y) - (v1.x * v3.y);  // x3y1 - x1y3


  // 𝐸(𝑥, 𝑦) = (𝑦−𝑦0)∆𝑥 − (𝑥−𝑥0)∆𝑦
  // Pro každou hranu trojúhelníku inicializuj 𝐸𝑖(𝑥𝑚𝑖𝑛, 𝑦𝑚𝑖𝑛).
    Edge1 = A1 * x_min + B1 * y_min + C1;
    Edge2 = A2 * x_min + B2 * y_min + C2;
    Edge3 = A3 * x_min + B3 * y_min + C3;

  // Vyplnovani: Cyklus pres vsechny body (𝒙, 𝒚) v obdelniku (x_min, y_min), (x_max, y_max)
  // Pro aktualizaci hodnot hranove funkce v bode P(𝒙 ± 𝟏, 𝒚) nebo P(𝒙, 𝒚 ± 𝟏)

    for (y = y_min; y <= y_max; y++) {
        bool even = (y - y_min) % 2 == 0;

        int StartX = even ? x_min : x_max;
        int endX = even ? x_max + 1 : x_min - 1;
        int stepX = even ? 1 : -1;

        for (int x = StartX; x != endX; x += stepX) {
            if (Edge1 >= 0 && Edge2 >= 0 && Edge3 >= 0) {  // Je-li 𝐸𝑖(𝑥,𝑦) ≥ 0 pro všechny hrany, pak nastav hodnotu pixelu.
                putPixel(x, y, color1);
            }
            if (x != endX - stepX) {
                Edge1 += even ? A1 : -A1;
                Edge2 += even ? A2 : -A2;
                Edge3 += even ? A3 : -A3;
            }
        }
        Edge1 += B1;
        Edge2 += B2;
        Edge3 += B3;
    }

    // Prekresleni hranic trojuhelniku barvou color2.
    drawLine(v1.x, v1.y, v2.x, v2.y, color2, arrow);
    drawLine(v2.x, v2.y, v3.x, v3.y, color2, arrow);
    drawLine(v3.x, v3.y, v1.x, v1.y, color2, arrow);
}



void pinedaPolygon(const Point *points, const int size, const RGBA &color1, const RGBA &color2) {
    if (size > 1) {

      // parametry obecné rovnice primky
        typedef std::vector<int> Parameter;
        Parameter A(size);
        Parameter B(size);
        Parameter C(size);

        EdgeFncValues edgeFncValues(size);  // jednotlivé hodnoty hranové funkce úseček polygonu
        EdgeFncValues edgeFncValues_Start(size);

        int      x_min, y_min, x_max, y_max;  // hranice obálky
        int		 i, x, y;  // počítadla v cyklu
        bool	 Inside_polygon;  // určuje, jestli je bod vevnitř polygonu


      // Nalezeni obalky (x_min, x_max), (y_min, y_max) polygonu.
        x_max = x_min = points[0].x;
        y_max = y_min = points[0].y;

        for (i = 0; i < size; i++) {
            x_min = MIN(x_min, points[i].x);
            y_min = MIN(y_min, points[i].y);
            x_max = MAX(x_max, points[i].x);
            y_max = MAX(y_max, points[i].y);
        }

      // Oriznuti obalky (x_min, x_max), (y_min, y_max) polygonu podle rozmeru okna
        x_min = MAX(x_min, 0);
        y_min = MAX(y_min, 0);
        x_max = MIN(x_max, width - 1);
        y_max = MIN(y_max, height - 1);

      // Vypocet prvnotnich hodnot hranovych funkci pro jednotlive hrany.    
        for (i = 0; i < size; i++) {
                A[i] = points[i].y - points[(i + 1) % size].y;
                B[i] = points[(i + 1) % size].x - points[i].x;
                C[i] = points[i].x * points[(i + 1) % size].y - points[(i + 1) % size].x * points[i].y;

                edgeFncValues[i] = A[i] * x_min + B[i] * y_min + C[i];
        }

      //  Test konvexnosti polygonu    
        //  Beyond my understanding.


      // Vyplnovani: Cyklus pres vsechny body (𝒙, 𝒚) v obdelniku (x_min, y_min), (x_max, y_max)
        for (y = y_min; y <= y_max; y++) {
            bool even = (y - y_min) % 2 == 0;

            int StartX = even ? x_min : x_max;
            int endX = even ? x_max + 1 : x_min - 1;
            int stepX = even ? 1 : -1;

            for (int x = StartX; x != endX; x += stepX) {

                Inside_polygon = true;
                for (i = 0; i < size; i++) {
                    if (edgeFncValues[i] < 0) {
                        Inside_polygon = false;
                        break;
                    }
                    else {
                        Inside_polygon = true;
                    }
                }
                if (Inside_polygon) {
                    putPixel(x, y, color1);
                }
                if (x != endX - stepX) {
                    for (i = 0; i < size; i++) {
                        edgeFncValues[i] += even ? A[i] : -A[i];
                    }
                }
            }
            for (i = 0; i < size; i++) {
                edgeFncValues[i] += B[i];
            }
        }

      // Prekresleni hranic polygonu barvou color2.
        for (int i = 0; i < size; i++) {
            drawLine(points[i].x, points[i].y, points[(i + 1) % size].x, points[(i + 1) % size].y, color2);
        }
    }
}
