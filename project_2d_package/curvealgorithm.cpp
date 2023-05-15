#include "curvealgorithm.h"
#include <cmath>

CurveAlgorithm::CurveAlgorithm()
{
    //ctor
}

void CurveAlgorithm::DrawHermiteCurve(HDC hdc, POINT& P0, POINT& T0, POINT& P1, POINT& T1, int numpoints, COLORREF color)
{
    if (numpoints < 2) return;

    // Calculate the step size
    double dt = 1.0 / (numpoints - 1);

    // Calculate the Hermite curve points and draw them using SetPixel
    for (int i = 0; i < numpoints; i++)
    {
        double t = i * dt;

        // Calculate the blending functions
        double h1 = 2 * pow(t, 3) - 3 * pow(t, 2) + 1;
        double h2 = -2 * pow(t, 3) + 3 * pow(t, 2);
        double h3 = pow(t, 3) - 2 * pow(t, 2) + t;
        double h4 = pow(t, 3) - pow(t, 2);

        // Calculate the x and y coordinates of the curve point
        double x = h1 * P0.x + h2 * P1.x + h3 * T0.x + h4 * T1.x;
        double y = h1 * P0.y + h2 * P1.y + h3 * T0.y + h4 * T1.y;

        // Round the coordinates to integers
        int pixelX = round(x);
        int pixelY = round(y);

        // Set the pixel color using SetPixel
        SetPixel(hdc, pixelX, pixelY, color);
    }
}


void CurveAlgorithm::DrawBezierCurve(HDC hdc, POINT& P0, POINT& P1, POINT& P2, POINT& P3, int numpoints, COLORREF color)
{
    // Calculate the control points for the Hermite curve based on the Bezier curve points
    POINT T0 = { P0.x + (P1.x - P0.x) / 3, P0.y + (P1.y - P0.y) / 3 };
    POINT T1 = { P3.x - (P3.x - P2.x) / 3, P3.y - (P3.y - P2.y) / 3 };

    // Draw the Hermite curve using the control points and the specified color
    DrawHermiteCurve(hdc, P0, T0, P3, T1, numpoints, color);
}


void CurveAlgorithm::DrawCardinalSpline(HDC hdc, POINT P[], int n, double c, int numpoints, COLORREF color)
{
    double c1 = 1 - c;

    for (int i = 2; i < n - 1; i++)
    {
        // Calculate the control points for the Hermite curve based on the Cardinal spline points
        POINT T0 = { static_cast<int>(c1 * (P[i].x - P[i - 1].x)), static_cast<int>(c1 * (P[i].y - P[i - 1].y)) };
        POINT T1 = { static_cast<int>(c1 * (P[i + 1].x - P[i - 1].x)), static_cast<int>(c1 * (P[i + 1].y - P[i - 1].y)) };

        // Draw the Hermite curve using the control points and the specified color
        DrawHermiteCurve(hdc, P[i - 1], T0, P[i], T1, numpoints, color);
    }
}
CurveAlgorithm::~CurveAlgorithm()
{
    //dtor
}
