#include "fillingalgorithm.h"
#include "curvealgorithm.h"
#include "linealgorithm.h"
#include "circlealgorithm.h"
#include <algorithm>
#include <cmath>
#include <stack>
#include<bits/stdc++.h>
#define MAXENTRIES 600

using namespace std;



FillingAlgorithm::FillingAlgorithm()
{
    //ctor
}



void FillingAlgorithm::InitEntries(Entry table[])
{
    for(int i=0; i<MAXENTRIES; i++)
    {
        table[i].xmin= INT_MAX;
        table[i].xmax= -INT_MAX;
    }
}

void FillingAlgorithm::ScanEdge(POINT v1,POINT v2,Entry table[])
{
    if(v1.y==v2.y)return;
    if(v1.y>v2.y)
        swap(v1,v2);
    double m=(double)(v2.x-v1.x)/(v2.y-v1.y);
    double x=v1.x;
    int y=v1.y;
    while(y<v2.y)
    {
        if(x<table[y].xmin)
            table[y].xmin=(int)ceil(x);
        if(x>table[y].xmax)
            table[y].xmax=(int)floor(x);
        y++;
        x+=m;
    }
}

void FillingAlgorithm::DrawScanLines(HDC hdc,Entry table[],COLORREF color)
{
    for(int y=0; y<MAXENTRIES; y++)
        if(table[y].xmin<table[y].xmax)
            for(int x=table[y].xmin; x<=table[y].xmax; x++)
                SetPixel(hdc,x,y,color);
}


void FillingAlgorithm::ConvexFill(HDC hdc,POINT p[],int n,COLORREF color)
{
    Entry *table=new Entry[MAXENTRIES];
    InitEntries(table);
    POINT v1=p[n-1];
    for(int i=0; i<n; i++)
    {
        POINT v2=p[i];
        ScanEdge(v1,v2,table);
        v1=p[i];
    }
    DrawScanLines(hdc,table,color);
    delete table;
}



FillingAlgorithm::EdgeRec FillingAlgorithm::InitEdgeRec(POINT& v1, POINT& v2)
{
    if (v1.y > v2.y)
        swap(v1, v2);
    EdgeRec rec;
    rec.x = v1.x;
    rec.ymax = v2.y;
    rec.minv = static_cast<double>(v2.x - v1.x) / (v2.y - v1.y);
    return rec;
}
void FillingAlgorithm::InitEdgeTable(POINT *polygon,int n,EdgeList table[])
{
    POINT v1=polygon[n-1];
    for(int i=0; i<n; i++)
    {
        POINT v2=polygon[i];
        if(v1.y==v2.y)
        {
            v1=v2;
            continue;
        }
        EdgeRec rec=InitEdgeRec(v1, v2);
        table[v1.y].push_back(rec);
        v1=polygon[i];
    }
}
void FillingAlgorithm::GeneralPolygonFill(HDC hdc,POINT *polygon,int n,COLORREF c)
{
    EdgeList *table=new EdgeList [MAXENTRIES];
    InitEdgeTable(polygon,n,table);
    int y=0;
    while(y<MAXENTRIES && table[y].size()==0)y++;
    if(y==MAXENTRIES)return;
    EdgeList ActiveList=table[y];
    while (ActiveList.size()>0)
    {
        ActiveList.sort();
        for(EdgeList::iterator it=ActiveList.begin(); it!=ActiveList.end(); it++)
        {
            int x1=(int)ceil(it->x);
            it++;
            int x2=(int)floor(it->x);
            for(int x=x1; x<=x2; x++)SetPixel(hdc,x,y,c);
        }
        y++;
        EdgeList::iterator it=ActiveList.begin();
        while(it!=ActiveList.end())
            if(y==it->ymax) it=ActiveList.erase(it);
            else it++;
        for(EdgeList::iterator it=ActiveList.begin(); it!=ActiveList.end(); it++)
            it->x+=it->minv;
        ActiveList.insert(ActiveList.end(),table[y].begin(),table[y].end());
    }
    delete[] table;
}


void FillingAlgorithm::DrawVerticalHermiteCurve(HDC hdc, int x, int y0, int y1, int dy0, int dy1, int numpoints, COLORREF color)
{
    CurveAlgorithm curve;

    // Create POINT objects for the control points
    POINT P0 = { x, y0 };
    POINT T0 = { x, y0 + dy0 };
    POINT P1 = { x, y1 };
    POINT T1 = { x, y1 + dy1 };

    // Call DrawHermiteCurve to draw the vertical Hermite curve
    curve.DrawHermiteCurve(hdc, P0, T0, P1, T1, numpoints, color);
}

// Function to fill a square with vertical Hermite curves
void FillingAlgorithm::FillSquareWithVerticalCurves(HDC hdc, int x0, int y0, int size, int numpoints, COLORREF color)
{
    // Calculate the coordinates of the square
    int x1 = x0 + size;
    int y1 = y0 + size;
    // Draw the square borders
    MoveToEx(hdc, x0, y0, NULL);
    LineTo(hdc, x1, y0);
    LineTo(hdc, x1, y1);
    LineTo(hdc, x0, y1);
    LineTo(hdc, x0, y0);

    // Calculate the vertical control points
    int dy0 = static_cast<int>(0.2 * size);
    int dy1 = static_cast<int>(0.8 * size);

    // Draw vertical Hermite curves in the square
    for (int x = x0; x < x1; x++)
    {
        DrawVerticalHermiteCurve(hdc, x, y0, y1, dy0, dy1, numpoints, color);
    }
}

void FillingAlgorithm::DrawHorizontalBezierCurve(HDC hdc, int x0, int y, int width, int dx0, int dx1, int numpoints, COLORREF color)
{
    CurveAlgorithm curve;
    // Calculate the increment in x direction
    double dx = static_cast<double>(width) / numpoints;
    int x1 = x0 + width;
    // Initialize the starting point
    double x = x0;

    POINT P0 = { x0, y };
    POINT P1 = { x0 + dx0, y };
    POINT P2 = { x1 - dx1, y };
    POINT P3 = { x1, y };


    // Draw the curve using Bezier algorithm
    for (int i = 0; i <= numpoints; i++)
    {
        // Check if the current point is within the rectangle's borders
        if (x >= x0 && x <= x0 + width)
        {
            // Draw the Bezier curve using the provided function
            curve.DrawBezierCurve(hdc, P0, P1, P2, P3, numpoints, color);
        }

        // Update the current point
        x += dx;
    }
}

//Utility function to get the quarter
void DrawQuarterCircles(HDC hdc, int xc, int yc, int a, int b, int quarter, COLORREF color)
{
    if (quarter == 1) {
        SetPixel(hdc, xc + a, yc - b, color);
        SetPixel(hdc, xc + b, yc - a, color);
    }
    else if (quarter == 2) {
        SetPixel(hdc, xc + a, yc + b, color);
        SetPixel(hdc, xc + b, yc + a, color);

    }
    else if (quarter == 3) {
        SetPixel(hdc, xc - a, yc + b, color);
        SetPixel(hdc, xc - b, yc + a, color);
    }
    else {
        SetPixel(hdc, xc - b, yc - a, color);
        SetPixel(hdc, xc - a, yc - b, color);

    }
}
void DarwQuarterLines(HDC hdc, int xc, int yc, int a, int b, int quarter, COLORREF color)
{
    LineAlgorithm line;
    if (quarter == 1) {
        line.drawLineMidPoint(hdc, xc, yc, xc + a, yc - b, color);
        line.drawLineMidPoint(hdc, xc, yc, xc + b, yc - a, color);
    }
    else if (quarter == 2) {
        line.drawLineMidPoint(hdc, xc, yc, xc + a, yc + b, color);
        line.drawLineMidPoint(hdc, xc, yc, xc + b, yc + a, color);

    }
    else if (quarter == 3) {
        line.drawLineMidPoint(hdc, xc, yc, xc - a, yc + b, color);
        line.drawLineMidPoint(hdc, xc, yc, xc - b, yc + a, color);
    }
    else {
        line.drawLineMidPoint(hdc, xc, yc, xc - b, yc - a, color);
        line.drawLineMidPoint(hdc, xc, yc, xc - a, yc - b, color);

    }
}

// Function to fill a rectangle with horizontal Bezier curves
void FillingAlgorithm::FillRectangleWithHorizontalCurves(HDC hdc, int x0, int y0, int width, int height, int numpoints, COLORREF color)
{
    // Calculate the coordinates of the rectangle
    int x1 = x0 + width;
    int y1 = y0 + height;

    // Calculate the horizontal control points
    int dx0 = static_cast<int>(0.2 * width);
    int dx1 = static_cast<int>(0.2 * width);
    // Draw the square borders
    MoveToEx(hdc, x0, y0, NULL);
    LineTo(hdc, x1, y0);
    LineTo(hdc, x1, y1);
    LineTo(hdc, x0, y1);
    LineTo(hdc, x0, y0);

    // Draw horizontal Bezier curves in the rectangle
    for (int y = y0; y < y1; y++)
    {
        DrawHorizontalBezierCurve(hdc, x0, y, width, dx0, dx1,numpoints, color);
    }
}


void FillingAlgorithm::FillQuarterCircle(HDC hdc, int xc, int yc, int r, int quarter, COLORREF color)
{
    CircleAlgorithm circle;
    double x = r, y = 0;
    double dtheta = (double)1 / r;
    double st = sin(dtheta), ct = cos(dtheta);

    while (x > y)
    {
        double x1 = x * ct - y * st;
        y = x * st + y * ct;
        x = x1;
        circle.draw8Points(hdc, xc, yc, round(x), round(y), color);
        DarwQuarterLines(hdc, xc, yc, round(x), round(y), quarter, color);

    }
}


void FillingAlgorithm::FillCircleWithChircles(HDC hdc, int xc, int yc, int r, int quarter, COLORREF color)
{
    CircleAlgorithm circle;
    double x = r, y = 0;
    double dtheta = (double)1 / r;
    double st = sin(dtheta), ct = cos(dtheta);

    while (x > y)
    {
        double x1 = x * ct - y * st;
        y = x * st + y * ct;
        x = x1;
        circle.draw8Points(hdc, xc, yc, round(x), round(y), color);
    }


    while (r) {
        dtheta = (double)1 / r;
        st = sin(dtheta);
        ct = cos(dtheta);
        x = r;
        y = 0;
        while (x > y)
        {
            double x1 = x * ct - y * st;
            y = x * st + y * ct;
            x = x1;
            DrawQuarterCircles(hdc, xc, yc, round(x), round(y), quarter, color);
        }
        r--;

    }
}


void FillingAlgorithm::FloodFill(HDC hdc,int x,int y,COLORREF Cb,COLORREF Cf)
{
    COLORREF C=GetPixel(hdc,x,y);
    if(C==Cb || C==Cf)
        return;
    SetPixel(hdc,x,y,Cf);
    FloodFill(hdc,x,y+1,Cb,Cf);
    FloodFill(hdc,x+1,y,Cb,Cf);
    FloodFill(hdc,x,y-1,Cb,Cf);
    FloodFill(hdc,x-1,y,Cb,Cf);
}
void FillingAlgorithm::NRFloodFill(HDC hdc,int x,int y,COLORREF Cb,COLORREF Cf)
{
    stack<Vertex> S;
    S.push(Vertex(x,y));
    while(!S.empty())
    {
        Vertex v=S.top();
        S.pop();
        COLORREF c=GetPixel(hdc,v.x,v.y);
        if(c==Cb || c==Cf)continue;
        SetPixel(hdc,v.x,v.y,Cf);
        S.push(Vertex(v.x+1,v.y));
        S.push(Vertex(v.x-1,v.y));
        S.push(Vertex(v.x,v.y+1));
        S.push(Vertex(v.x,v.y-1));
    }
}
FillingAlgorithm::~FillingAlgorithm()
{
    //dtor
}
