#ifndef LINEALGORITHM_H
#define LINEALGORITHM_H
#include <Windows.h>

class LineAlgorithm
{
    public:
        LineAlgorithm();
        void drawLineDDA(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color);
        void drawLineMidPoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color);
        void drawLineParametric(HDC hdc,int xs,int ys,int xe,int ye,COLORREF color);

        virtual ~LineAlgorithm();

    protected:

    private:
};

#endif // LINEALGORITHM_H
