#include "clippingalgorithm.h"
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <cmath>

using namespace std;

union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1, top:1, right:1, bottom:1;
    };
};
OutCode GetOutCode(double x, double y, int xleft, int ytop,int xright, int ybottom)
{
    OutCode out;
    out.All = 0;
    if(x < xleft)
    {
        out.left = 1;
    }
    else if(x> xright)
    {
        out.right = 1;

    }
    if(y> ytop)
    {
        out.top =1 ;
    }
    else if(y< ybottom)
    {
        out.bottom  =1;
    }
    return out;
}
ClippingAlgorithm::ClippingAlgorithm()
{
    //ctor
}


void ClippingAlgorithm::VIntersect(double xs, double ys, double xe, double ye, int x, double * xi, double *yi)
{
    *xi = x;
    *yi = ys + (x-xs)*(ye-ys)/(xe-xs);
}
void ClippingAlgorithm::HIntersect(double xs, double ys, double xe, double ye, int y, double * xi, double *yi)
{
    *yi = y;
    *xi = xs + (y-ys)*(xe-xs)/(ye-ys);
}


void ClippingAlgorithm::CohenRect(HDC hdc, int xs, int ys, int xe,int ye, int xleft, int ytop, int xright, int ybottom)
{
    int x1 = xs,y1 = ys, x2 = xe, y2 = ye;
    OutCode out1 = GetOutCode(x1,y1,xleft,ytop, xright,ybottom);
    OutCode out2 = GetOutCode(x2,y2,xleft,ytop, xright, ybottom);
    while((out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)
            {
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            }
            else if(out1.top)
            {
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            }
            else if(out1.right)
            {
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            }
            else
            {
                HIntersect(x1,y1,x2,y2,ybottom,&xi, &yi);
            }
            x1=xi;
            y1= yi;
            out1 = GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else if(out2.All)
        {
            if(out1.left)
            {
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            }
            else if(out2.top)
            {
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            }
            else if(out2.right)
            {
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            }
            else
            {
                HIntersect(x1,y1,x2,y2,ybottom,&xi, &yi);
            }
            x2=xi;
            y2= yi;
            out2 = GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }
    if(!out1.All && !out2.All)
    {
        MoveToEx(hdc,round(x1),round(y1),NULL);
        LineTo(hdc,round(x2),round(y2));
    }
}


ClippingAlgorithm::~ClippingAlgorithm()
{
    //dtor
}
