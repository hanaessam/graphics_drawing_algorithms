#ifndef CLIPPINGALGORITHM_H
#define CLIPPINGALGORITHM_H
#include <vector>
#include <windows.h>

class ClippingAlgorithm
{
public:

    ClippingAlgorithm();
    void PolygonClip(HDC hdc, POINT* p, int n, int xleft, int ytop, int xright, int ybottom);
    void PointClipping(HDC hdc, int x, int y, int xleft, int ytop, int xright, int ybottom, COLORREF color);
    void CohenSuth(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int ytop, int xright, int ybottom);
    void clippedLineWithSquare(HDC hdc, int top, int left, int r, COLORREF color);
    void clippedLineWithCircle(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c, int xc, int yc, int r);
    void clippedPointWithCircle(HDC hdc, int xc, int yc, int r, int x, int y, COLORREF c);

    virtual ~ClippingAlgorithm();

protected:

private:
};

#endif // CLIPPINGALGORITHM_H
