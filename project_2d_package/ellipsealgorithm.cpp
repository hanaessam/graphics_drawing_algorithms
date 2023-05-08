#include "ellipsealgorithm.h"
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <cmath>

#define PI 3.14
using namespace std;


EllipseAlgorithm::EllipseAlgorithm()
{
    //ctor
}
void EllipseAlgorithm::draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
    SetPixel(hdc,xc + x, yc + y, color);
    SetPixel(hdc,xc + x, yc - y, color);
    SetPixel(hdc,xc - x, yc - y, color);
    SetPixel(hdc,xc - x, yc + y, color);
}


void EllipseAlgorithm::drawEllipseDirect(HDC hdc, int xc, int yc, int a,int b, COLORREF color)
{
    int x = 0;
    double y = b;
    draw4Points(hdc, xc, yc, 0,b, color);
    while (x*b*b < y*a*a)
    {
        x++;
        y = b*sqrt(1.0 - (double)x*x/(a*a));
        draw4Points(hdc, xc, yc, x, round(y), color);
    }
    int y1 = 0;
    double x1 = a;
    draw4Points(hdc, xc, yc, a, 0, color);
    while (x1*b*b > y1*a*a)
    {
        y1++;
        x1 = a*sqrt(1.0 - (double)y1*y1 / (b*b));
        draw4Points(hdc, xc, yc, round(x1), y1, color);
    }

}


void EllipseAlgorithm::drawEllipsePolar(HDC hdc, int xc, int yc, int a, int b, COLORREF color)
{
    double theta = 0.0;
    double dtheta = 1.0 / max(a, b);
    double ct = cos(theta);
    double st = sin(theta);
    double x = a * ct;
    double y = b * st;

    while (theta < PI / 2)
    {
        draw4Points(hdc, xc, yc, round(x), round(y), color);
        theta += dtheta;
        ct = cos(theta);
        st = sin(theta);
        x = a * ct;
        y = b * st;
    }
}

void EllipseAlgorithm::drawEllipseBresenham(HDC hdc, int xc, int yc, int a, int b, COLORREF color)
{
    int a2 = a * a;
    int b2 = b * b;
    int x = 0;
    int y = b;
    int d = 4 * b2 - 4 * a2 * b + a2;

    while (x * b2 <= y * a2)
    {
        draw4Points(hdc, xc, yc, x, y, color);
        x++;

        if (d < 0)
        {
            d += 4 * b2 * (2 * x + 3);
        }
        else
        {
            y--;
            d += 4 * b2 * (2 * x + 3) + 4 * a2 * (-2 * y + 2);
        }
    }

    x = a;
    y = 0;
    d = 4 * a2 - 4 * a2 * b + b2;

    while (x * b2 > y * a2)
    {
        draw4Points(hdc, xc, yc, x, y, color);
        y++;

        if (d < 0)
        {
            d += 4 * a2 * (2 * y + 3);
        }
        else
        {
            x--;
            d += 4 * a2 * (2 * y + 3) + 4 * b2 * (-2 * x + 2);
        }
    }
}


EllipseAlgorithm::~EllipseAlgorithm()
{
    //dtor
}
