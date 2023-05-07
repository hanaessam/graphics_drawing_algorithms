#ifndef ELLIPSEALGORITHM_H
#define ELLIPSEALGORITHM_H

#include <windows.h>

class EllipseAlgorithm
{
    public:
        EllipseAlgorithm();
        void draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color);
        void drawEllipseDirect(HDC hdc, int xc, int yc, int a,int b, COLORREF color);
        void drawEllipsePolar(HDC hdc, int xc, int yc, int a, int b, COLORREF color);
        void drawEllipseBresenham(HDC hdc, int xc, int yc, int a, int b, COLORREF color);
        virtual ~EllipseAlgorithm();

    protected:

    private:
};

#endif // ELLIPSEALGORITHM_H
