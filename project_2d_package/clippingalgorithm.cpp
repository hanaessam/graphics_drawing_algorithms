#include "clippingalgorithm.h"
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <cmath>

struct Vertex
{
    double x,y;
    Vertex(int x1=0,int y1=0)
    {
        x=x1;
        y=y1;
    }
};
union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1, top:1, right:1, bottom:1;
    };
};
enum CircleSide
{
    LEFT,
    TOP,
    RIGHT,
    BOTTOM
};

typedef std::vector<Vertex> VertexList;
typedef bool (*IsInFunc)(Vertex& v,int edge);
typedef Vertex (*IntersectFunc)(Vertex& v1,Vertex& v2,int edge);

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

VertexList ClipWithEdge(VertexList p,int edge,IsInFunc In,IntersectFunc Intersect)
{
    VertexList OutList;
    Vertex v1=p[p.size()-1];
    bool v1_in=In(v1,edge);
    for(int i=0; i<(int)p.size(); i++)
    {
        Vertex v2=p[i];
        bool v2_in=In(v2,edge);
        if(!v1_in && v2_in)
        {
            OutList.push_back(Intersect(v1,v2,edge));
            OutList.push_back(v2);
        }
        else if(v1_in && v2_in) OutList.push_back(v2);
        else if(v1_in) OutList.push_back(Intersect(v1,v2,edge));
        v1=v2;
        v1_in=v2_in;
    }
    return OutList;
}

bool InLeft(Vertex& v,int edge)
{
    return v.x>=edge;
}
bool InRight(Vertex& v,int edge)
{
    return v.x<=edge;
}
bool InTop(Vertex& v,int edge)
{
    return v.y>=edge;
}
bool InBottom(Vertex& v,int edge)
{
    return v.y<=edge;
}
Vertex VIntersect(Vertex& v1,Vertex& v2,int xedge)
{
    Vertex res;
    res.x=xedge;
    res.y=v1.y+(xedge-v1.x)*(v2.y-v1.y)/(v2.x-v1.x);
    return res;
}
Vertex HIntersect(Vertex& v1,Vertex& v2,int yedge)
{
    Vertex res;
    res.y=yedge;
    res.x=v1.x+(yedge-v1.y)*(v2.x-v1.x)/(v2.y-v1.y);
    return res;
}
void ClippingAlgorithm::PolygonClip(HDC hdc,POINT *p,int n,int xleft,int ytop,int xright,int ybottom)
{
    VertexList vlist;
    for(int i=0; i<n; i++)
        vlist.push_back(Vertex(p[i].x,p[i].y));
    vlist= ClipWithEdge(vlist,xleft,InLeft,VIntersect);
    vlist= ClipWithEdge(vlist,ytop,InTop,HIntersect);
    vlist= ClipWithEdge(vlist,xright,InRight,VIntersect);
    vlist= ClipWithEdge(vlist,ybottom,InBottom,HIntersect);
    Vertex v1=vlist[vlist.size()-1];
    for(int i=0; i<(int)vlist.size(); i++)
    {
        Vertex v2=vlist[i];
        MoveToEx(hdc,round(v1.x),round(v1.y),NULL);
        LineTo(hdc,round(v2.x),round(v2.y));
        v1=v2;
    }
}
void VIntersect(double xs, double ys, double xe, double ye, int x, double * xi, double *yi)
{
    *xi = x;
    *yi = ys + (x-xs)*(ye-ys)/(xe-xs);
}
void HIntersect(double xs, double ys, double xe, double ye, int y, double * xi, double *yi)
{
    *yi = y;
    *xi = xs + (y-ys)*(xe-xs)/(ye-ys);
}

void ClippingAlgorithm::PointClipping(HDC hdc, int x,int y,int xleft,int ytop,int xright,int ybottom,COLORREF color)
{
    if(x>=xleft && x<= xright && y>=ytop && y<=ybottom)
        SetPixel(hdc,x,y,color);
}

void ClippingAlgorithm::CohenSuth(HDC hdc, int xs, int ys, int xe,int ye, int xleft, int ytop, int xright, int ybottom)
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


OutCode GetCircleOutCode(int x, int y, int centerX, int centerY, int radiusSquared)
{
    OutCode out;
    out.All = 0;

    if (x < centerX - radiusSquared)
        out.left = 1;
    else if (x > centerX + radiusSquared)
        out.right = 1;

    if (y < centerY - radiusSquared)
        out.top = 1;
    else if (y > centerY + radiusSquared)
        out.bottom = 1;

    // Check if the point is inside the circle
    if ((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) > radiusSquared)
        out.All = 1;
    return out;
}

void  GetCircleIntersect(int xs, int ys, int xe, int ye, int centerX, int centerY, int radiusSquared, CircleSide side, double* xi, double* yi)
{
    double dx = xe - xs;
    double dy = ye - ys;

// Calculate the intersection point with the circle
    switch (side)
    {
    case LEFT:
        *xi = centerX - sqrt(radiusSquared - (ys - centerY) * (ys - centerY));
        *yi = ys + dy * (*xi - xs) / dx;
        break;
    case TOP:
        *yi = centerY - sqrt(radiusSquared - (xs - centerX) * (xs - centerX));
        *xi = xs + dx * (*yi - ys) / dy;
        break;
    case RIGHT:
        *xi = centerX + sqrt(radiusSquared - (ys - centerY) * (ys - centerY));
        *yi = ys + dy * (*xi - xs) / dx;
        break;
    case BOTTOM:
        *yi = centerY + sqrt(radiusSquared - (xs - centerX) * (xs - centerX));
        *xi = xs + dx * (*yi - ys) / dy;
        break;
    }
}


void ClippingAlgorithm::CohenSuthCircle(HDC hdc, int xs, int ys, int xe, int ye, int centerX, int centerY, int radius)
{
    // Calculate the squared radius for faster distance comparison
    int radiusSquared = radius * radius;

    // Compute the outcodes for the start and end points
    OutCode out1 = GetCircleOutCode(xs, ys, centerX, centerY, radiusSquared);
    OutCode out2 = GetCircleOutCode(xe, ye, centerX, centerY, radiusSquared);

    // Perform Cohen-Sutherland line clipping
    while ((out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi, yi;

        if (out1.All)
        {
            if (out1.left)
                GetCircleIntersect(xs, ys, xe, ye, centerX, centerY, radiusSquared, LEFT, &xi, &yi);
            else if (out1.top)
                GetCircleIntersect(xs, ys, xe, ye, centerX, centerY, radiusSquared, TOP, &xi, &yi);
            else if (out1.right)
                GetCircleIntersect(xs, ys, xe, ye, centerX, centerY, radiusSquared, RIGHT, &xi, &yi);
            else if (out1.bottom)
                GetCircleIntersect(xs, ys, xe, ye, centerX, centerY, radiusSquared, BOTTOM, &xi, &yi);

            xs = static_cast<int>(xi);
            ys = static_cast<int>(yi);
            out1 = GetCircleOutCode(xs, ys, centerX, centerY, radiusSquared);
        }
        else if (out2.All)
        {
            if (out2.left)
                GetCircleIntersect(xs, ys, xe, ye, centerX, centerY, radiusSquared, LEFT, &xi, &yi);
            else if (out2.top)
                GetCircleIntersect(xs, ys, xe, ye, centerX, centerY, radiusSquared, TOP, &xi, &yi);
            else if (out2.right)
                GetCircleIntersect(xs, ys, xe, ye, centerX, centerY, radiusSquared, RIGHT, &xi, &yi);
            else if (out2.bottom)
                GetCircleIntersect(xs, ys, xe, ye, centerX, centerY, radiusSquared, BOTTOM, &xi, &yi);

            xe = static_cast<int>(xi);
            ye = static_cast<int>(yi);
            out2 = GetCircleOutCode(xe, ye, centerX, centerY, radiusSquared);
        }
    }

    // If the line is completely outside the clipping window, don't draw it
    if (!out1.All && !out2.All)
    {
        MoveToEx(hdc, xs, ys, nullptr);
        LineTo(hdc, xe, ye);
    }
}


ClippingAlgorithm::~ClippingAlgorithm()
{
    //dtor
}
