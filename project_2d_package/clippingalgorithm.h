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
    void CohenSuthCircle(HDC hdc, int xs, int ys, int xe, int ye, int centerX, int centerY, int radius);
    virtual ~ClippingAlgorithm();

protected:

private:
};

#endif // CLIPPINGALGORITHM_H
