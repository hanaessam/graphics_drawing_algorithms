#include "circlealgorithm.h"
#include <cmath>
#include <algorithm>
#include <windows.h>
CircleAlgorithm::CircleAlgorithm()
{
    //ctor
}
void CircleAlgorithm::draw8Points(HDC hdc,int xc,int yc, int a, int b,COLORREF color)
{
    SetPixel(hdc, xc+a, yc+b, color);
    SetPixel(hdc, xc-a, yc+b, color);
    SetPixel(hdc, xc-a, yc-b, color);
    SetPixel(hdc, xc+a, yc-b, color);
    SetPixel(hdc, xc+b, yc+a, color);
    SetPixel(hdc, xc-b, yc+a, color);
    SetPixel(hdc, xc-b, yc-a, color);
    SetPixel(hdc, xc+b, yc-a, color);
}
void CircleAlgorithm::drawCircleDirect(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int R2=R*R;
    draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        x++;
        y=round(sqrt((double)(R2-x*x)));
        draw8Points(hdc,xc,yc,x,y,color);
    }
}
void CircleAlgorithm::drawCirclePolar(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=R,y=0;
    double theta=0,dtheta=1.0/R;
    draw8Points(hdc,xc,yc,x,y,color);
    while(x>y)
    {
        theta+=dtheta;
        x=round(R*cos(theta));
        y=round(R*sin(theta));
        draw8Points(hdc,xc,yc,x,y,color);
    }
}
void CircleAlgorithm::drawCircleIterativePolar(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    double x=R,y=0;
    double dtheta=1.0/R;
    double cdtheta=cos(dtheta),sdtheta=sin(dtheta);
    draw8Points(hdc,xc,yc,R,0,color);
    while(x>y)
    {
        double x1=x*cdtheta-y*sdtheta;
        y=x*sdtheta+y*cdtheta;
        x=x1;
        draw8Points(hdc,xc,yc,round(x),round(y),color);
    }
}

void CircleAlgorithm::drawCircleBresenham(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int d=1-R;
    draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        if(d<0)
            d+=2*x+2;
        else
        {
            d+=2*(x-y)+5;
            y--;
        }
        x++;
        draw8Points(hdc,xc,yc,x,y,color);
    }
}

void CircleAlgorithm::drawCircleFasterBresenham(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int d=1-R;
    int c1=3, c2=5-2*R;
    draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        if(d<0)
        {
            d+=c1;
            c2+=2;
        }
        else
        {
            d+=c2;
            c2+=4;
            y--;
        }
        c1+=2;
        x++;
        draw8Points(hdc,xc,yc,x,y,color);
    }
}

void CircleAlgorithm::drawQuarterCircle(HDC hdc, int centerX, int centerY, int radius, COLORREF color)
{
    int x = 0;
    int y = radius;
    int decision = 3 - 2 * radius;

    while (x <= y)
    {
        // Plot the points in the first quadrant
        SetPixel(hdc, centerX + x, centerY - y, color);
        SetPixel(hdc, centerX + y, centerY - x, color);

        if (decision <= 0)
        {
            decision += 4 * x + 6;
        }
        else
        {
            decision += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

CircleAlgorithm::~CircleAlgorithm()
{
    //dtor
}
