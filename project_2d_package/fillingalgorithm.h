#ifndef FILLINGALGORITHM_H
#define FILLINGALGORITHM_H
#include <list>
#include <windows.h>



class FillingAlgorithm
{
public:
    struct Entry
    {
        int xmin,xmax;
    };

    struct EdgeRec
    {
        double x;
        double minv;
        int ymax;
        bool operator<(EdgeRec r)
        {
            return x<r.x;
        }
    };
    struct Vertex
    {
        int x,y;
        Vertex(int x,int y):x(x),y(y)
        {
        }
    };

    typedef std::list<EdgeRec> EdgeList;

    FillingAlgorithm();

    void InitEntries(Entry table[]);
    void ScanEdge(POINT v1,POINT v2,Entry table[]);
    void DrawScanLines(HDC hdc,Entry table[],COLORREF color);
    void ConvexFill(HDC hdc,POINT p[],int n,COLORREF color);

    EdgeRec InitEdgeRec(POINT& v1, POINT& v2);
    void InitEdgeTable(POINT *polygon,int n,EdgeList table[]);
    void GeneralPolygonFill(HDC hdc,POINT *polygon,int n,COLORREF c);

    void DrawVerticalHermiteCurve(HDC hdc, int x, int y0, int y1, int dy0, int dy1, int numpoints, COLORREF color);
    void FillSquareWithVerticalCurves(HDC hdc, int x0, int y0, int size, int numpoints, COLORREF color);
    void DrawHorizontalBezierCurve(HDC hdc, int x0, int y, int width, int dx0, int dx1, int numpoints, COLORREF color);
    void FillRectangleWithHorizontalCurves(HDC hdc, int x0, int y0, int width, int height, int numpoints, COLORREF color);
    void FillQuarterCircle(HDC hdc, int centerX, int centerY, int radius, COLORREF color);
    void FillCircleWithChircles(HDC hdc, int centerX, int centerY, int radius, int numCircles, COLORREF color);
    void FloodFill(HDC hdc,int x,int y,COLORREF Cb,COLORREF Cf);
    void NRFloodFill(HDC hdc,int x,int y,COLORREF Cb,COLORREF Cf);
    virtual ~FillingAlgorithm();

protected:

private:
};

#endif // FILLINGALGORITHM_H
