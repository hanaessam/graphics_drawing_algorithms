#include "linealgorithm.h"
#include <cmath>
#include <algorithm>
#include <windows.h>
LineAlgorithm::LineAlgorithm()
{
    //ctor
}

void LineAlgorithm::drawLineDDA(HDC hdc,int xs,int ys,int xe,int ye,COLORREF color)
{

    int dx=xe-xs;
    int dy=ye-ys;
    SetPixel(hdc,xs,ys,color);
    if(abs(dx)>=abs(dy))
    {
        int x=xs,xinc= dx>0?1:-1;
        double y=ys,yinc=(double)dy/dx*xinc;
        while(x!=xe)
        {
            x+=xinc;
            y+=yinc;
            SetPixel(hdc,x,round(y),color);
        }
    }
    {
        int y=ys,yinc= dy>0?1:-1;
        double x=xs,xinc=(double)dx/dy*yinc;
        while(y!=ye)
        {
            x+=xinc;
            y+=yinc;
            SetPixel(hdc,round(x),y,color);
        }
    }
}


void LineAlgorithm::drawLineMidPoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int change = dx - dy;

    while (true)
    {
        SetPixel(hdc, x1, y1, color);

        if (x1 == x2 && y1 == y2)
        {
            break;
        }

        int change2 = 2 * change;

        if (change2 > -dy)
        {
            change -= dy;
            x1 += sx;
        }

        if (change2 < dx)
        {
            change += dx;
            y1 += sy;
        }
    }
}


void LineAlgorithm::drawLineParametric(HDC hdc,int xs,int ys,int xe,int ye,COLORREF color)
{
    int dx=xe-xs;
    int dy=ye-ys;
    double slope=(double)dy/dx;
    if(xs>xe)
    {
        std::swap(xs,xe);
        std::swap(ys,ye);
    }
    for(int x=xs; x<=xe; x++)
    {
        int y=round(ys+(x-xs)*slope);
        SetPixel(hdc,x,y,color);
    }
}


LineAlgorithm::~LineAlgorithm()
{
    //dtor
}
