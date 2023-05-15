#ifndef FILLINGALGORITHMS _H
#define FILLINGALGORITHMS _H


class FillingAlgorithms
{
    public:
        FillingAlgorithms ();
        void DrawVerticalHermiteCurve(HDC hdc, int x, int y0, int y1, int dy0, int dy1, int numpoints, COLORREF color);
        virtual ~FillingAlgorithms ();

    protected:

    private:
};

#endif // FILLINGALGORITHMS _H
