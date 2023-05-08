#ifndef CLIPPINGALGORITHM_H
#define CLIPPINGALGORITHM_H

#include <windows.h>

class ClippingAlgorithm
{
    public:
        ClippingAlgorithm();
        void VIntersect(double xs, double ys, double xe, double ye, int x, double * xi, double *yi);
        void HIntersect(double xs, double ys, double xe, double ye, int y, double * xi, double *yi);
        void CohenRect(HDC hdc, int xs, int ys, int xe,int ye, int xleft, int ytop, int xright, int ybottom);

        virtual ~ClippingAlgorithm();

    protected:

    private:
};

#endif // CLIPPINGALGORITHM_H
