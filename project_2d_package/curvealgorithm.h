#ifndef CURVEALGORITHM_H
#define CURVEALGORITHM_H

#include <windows.h>

class CurveAlgorithm
{
    public:
        CurveAlgorithm();
        void DrawHermiteCurve(HDC hdc, POINT& P0, POINT& T0, POINT& P1, POINT& T1, int numpoints, COLORREF color);
        void DrawBezierCurve(HDC hdc, POINT& P0, POINT& P1, POINT& P2, POINT& P3, int numpoints, COLORREF color);
        void DrawCardinalSpline(HDC hdc, POINT P[], int n, double c, int numpoints, COLORREF color);


        virtual ~CurveAlgorithm();

    protected:

    private:
};

#endif // CURVEALGORITHM_H
