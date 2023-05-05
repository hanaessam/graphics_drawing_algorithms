#ifndef CIRCLEALGORITHM_H
#define CIRCLEALGORITHM_H

#include <Windows.h>
class CircleAlgorithm
{
    public:
        CircleAlgorithm();
        void draw8Points(HDC hdc,int xc,int yc, int a, int b,COLORREF color);
        void drawCircleDirect(HDC hdc,int xc,int yc, int R,COLORREF color);
        void drawCirclePolar(HDC hdc,int xc,int yc, int R,COLORREF color);
        void drawCircleIterativePolar(HDC hdc,int xc,int yc, int R,COLORREF color);
        void drawCircleBresenham(HDC hdc,int xc,int yc, int R,COLORREF color);
        void drawCircleFasterBresenham(HDC hdc,int xc,int yc, int R,COLORREF color);
        virtual ~CircleAlgorithm();


    protected:

    private:
};

#endif // CIRCLEALGORITHM_H
