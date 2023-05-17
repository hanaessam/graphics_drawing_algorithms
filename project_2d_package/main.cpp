
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "ids.h"


#define PI 3.14



using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
HMENU hMenu;
void addMenu(HWND);
void SaveScreenDataToFile(HWND, const string&);
void LoadScreenDataFromFile(HWND, const string&);
HBRUSH hSolidBrush;
HBRUSH hHatchedBrush;
bool useHatchedBrush = false;



int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;



    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               1000,                 /* The programs width */
               750,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

HBRUSH SelectHatchedBrush(HDC hdc, HBRUSH hSolidBrush, HBRUSH hHatchedBrush, bool useHatchedBrush)
{
    HBRUSH hSelectedBrush = useHatchedBrush ? hHatchedBrush : hSolidBrush;
    return (HBRUSH)SelectObject(hdc, hSelectedBrush);
}
/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int x0, y0, size, width, height;
    static int x1, x2, y1, y2, a, b;
    static int xleft, xright,ytop, ybottom;
    static int radius, squareRadius;
    static int numpoints = 10000;
    static int pointscount = 200;
    static POINT controlPoints[10];
    static int numControlPoints = 0;
    static double tension = 0.5;
    static COLORREF color = RGB(255, 255, 255);
    static int convexNum = 0;
    static int counter = 0;
    static int quarter = 0;
    static POINT* shapePoints = new POINT[convexNum];
    HDC hdc = GetDC(hwnd);


    HBRUSH hBrush;
    HBRUSH hOldBrush;

    LineAlgorithm line;
    CircleAlgorithm circle;
    EllipseAlgorithm ellipse;
    ClippingAlgorithm clipping;
    CurveAlgorithm curve;
    FillingAlgorithm filling;

    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case CLEAR_SCREEN:
            RECT rect;
            GetClientRect(hwnd, &rect);
            hOldBrush = (HBRUSH)GetClassLongPtr(hwnd, GCLP_HBRBACKGROUND);
            FillRect(hdc, &rect, hOldBrush);
            ReleaseDC(hwnd, hdc);
            break;
        case SAVE_SCREEN:
            SaveScreenDataToFile(hwnd, "screen_data.txt");
            break;

        case LOAD_FILE:
            LoadScreenDataFromFile(hwnd, "screen_data.txt");
            break;

        case FILE_MENU_EXIT:
            DestroyWindow(hwnd);
            break;

        case MOUSE_CURSOR:
            SetClassLongPtr(hwnd, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(LoadCursor(nullptr, IDC_CROSS)));
            RedrawWindow(hwnd, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE);
            break;


        case CHANGE_BRUSH:
            // Toggle between solid and hatched brush
            useHatchedBrush = !useHatchedBrush;
            SelectHatchedBrush(hdc, hSolidBrush, hHatchedBrush, useHatchedBrush);
            rect = { 50, 50, 200, 200 };
            FillRect(hdc, &rect, SelectHatchedBrush(hdc, hSolidBrush, hHatchedBrush, useHatchedBrush));
            break;


        case BLACK_CLR:
            color = RGB(0,0,0);
            break;
        case RED_CLR:
            color = RGB(164, 0, 2);
            break;
        case GREEN_CLR:
            color = RGB(0, 129, 64);
            break;
        case BLUE_CLR:
            color = RGB(7, 120, 186);
            break;
        case PURPLE_CLR:
            color = RGB(121, 37, 199);
            break;
        case YELLOW_CLR:
            color = RGB(245, 220, 6);
            break;
        case PINK_CLR:
            color = RGB(253, 93, 168);
            break;

        case WHITE_BG:
            hBrush = CreateSolidBrush(RGB(255, 255, 255));
            SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);
            DeleteObject(hBrush);
            InvalidateRect(hwnd, NULL, TRUE);
            break;

        case LINE_DDA:
            line.drawLineDDA(hdc, x1, y1, x2, y2, color);
            break;

        case LINE_MIDPOINT:
            line.drawLineMidPoint(hdc, x1, y1, x2, y2, color);
            break;

        case LINE_PARAMETRIC:
            line.drawLineParametric(hdc, x1, y1, x2, y2, color);
            break;

        case CIRCLE_DIRECT:
            circle.drawCircleDirect(hdc, x1,y1,radius, color);
            break;
        case CIRCLE_POLAR:
            circle.drawCirclePolar(hdc, x1,y1,radius, color);
            break;
        case CIRCLE_ITERATIVE_POLAR:
            circle.drawCircleIterativePolar(hdc, x1,y1,radius, color);
            break;
        case CIRCLE_MIDPOINT:
            circle.drawCircleBresenham(hdc, x1,y1,radius, color);
            break;
        case CIRCLE_MODIFIED_MIDPOINT:
            circle.drawCircleFasterBresenham(hdc, x1,y1,radius, color);
            break;

        case ELLIPSE_DIRECT:
            ellipse.drawEllipseDirect(hdc, x1,y1,a,b, color);
            break;
        case ELLIPSE_POLAR:
            ellipse.drawEllipsePolar(hdc, x1,y1,a,b, color);
            break;
        case ELLIPSE_MIDPOINT:
            ellipse.drawEllipseBresenham(hdc, x1,y1,a,b, color);
            break;

        case CLIPPING_USING_RECTANGLE_LINE:
            Rectangle(hdc,100,300,500,100);
            clipping.CohenSuth(hdc,x1,y1,x2,y2,100,300,500,100);
            counter = 0;
            break;

        case CLIPPING_USING_RECTANGLE_POINT:
            Rectangle(hdc,100,300,500,100);
            clipping.PointClipping(hdc,x1,y1,100,300,500,100, color);
            break;
        case CLIPPING_USING_RECTANGLE_POLYGON:
            Rectangle(hdc,100,100,400,400);
            clipping.PolygonClip(hdc,shapePoints,convexNum,100,100,400,400);
            counter = 0;
            break;
        case CLIPPING_USING_SQUARE_LINE:
            Rectangle(hdc,100,100,400,400);
            clipping.PolygonClip(hdc,shapePoints,3,100,100,400,400);
            //clipping.clippedLineWithSquare(hdc,ytop,xleft,squareRadius,color);
            counter =0;
            break;

        case CLIPPING_USING_SQUARE_POINT:
            Rectangle(hdc,xleft, ytop, xright, ybottom);
            clipping.CohenSuth(hdc, x1, y1, x2, y2, xleft, ytop, xright, ybottom);
            clipping.PointClipping(hdc,x1,y1,xleft, ytop, xright, ybottom, color);
            break;

        case CLIPPING_USING_CIRCLE_LINE:
            //circle.drawCircleBresenham(hdc,x1,y1,radius,color);
            clipping.clippedLineWithCircle(hdc,x1,y1,x2,y2,color,x1,y1,radius);
            break;

        case CLIPPING_USING_CIRCLE_POINT:
            circle.drawCircleBresenham(hdc,x1,y1,radius,color);
            clipping.clippedPointWithCircle(hdc,x1,y1,color,x1,y1,radius);
            break;

        case CARDINAL_SPLINE_CURVE:
            curve.DrawCardinalSpline(hdc, controlPoints, numControlPoints, tension, numpoints, color);
            numControlPoints = 0;
            break;

        case FILL_CIRCLE_WITH_LINES:
            filling.FillQuarterCircle(hdc, x1,y1,radius,quarter,color);
            break;

        case FILL_CIRCLE_WITH_CIRCLES:
            filling.FillCircleWithChircles(hdc, x1,y1,radius,quarter,color);
            break;

        case FILL_SQUARE_WITH_HERMITE_CURVE:
            filling.FillSquareWithVerticalCurves(hdc, x0, y0, size, numpoints, color);
            break;

        case FILL_RECTANGLE_WITH_BEZIER_CURVE:
            filling.FillRectangleWithHorizontalCurves(hdc, x0, y0, width, height, pointscount, color);
            break;

        case CONVEX_FILL:
            filling.ConvexFill(hdc, shapePoints, convexNum, color);
            counter = 0;
            break;
        case NON_CONVEX_FILL:
            filling.GeneralPolygonFill(hdc, shapePoints, convexNum,color);
            counter = 0;
            break;

        case RECURSIVE_FLOOD_FILL:
            filling.FloodFill(hdc,x1,y1,color, color);
            break;
        case NON_RECURSIVE_FLOOD_FILL:
            filling.NRFloodFill(hdc,x1,y1,color, color);
            break;



        }
        break;
    case WM_LBUTTONDOWN:
        if (counter <= convexNum)
        {
            shapePoints[counter].x = LOWORD(lParam);
            shapePoints[counter].y = HIWORD(lParam);
            counter++;
        }

        x1 = LOWORD(lParam);
        y1 = HIWORD(lParam);
        if (numControlPoints < 10)
        {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);
            controlPoints[numControlPoints].x = xPos;
            controlPoints[numControlPoints].y = yPos;
            numControlPoints++;
        }
        x0 = LOWORD(lParam);
        y0 = HIWORD(lParam);

        xleft = LOWORD(lParam);
        ytop = HIWORD(lParam);
        break;

    case WM_RBUTTONDOWN:
        x2 = LOWORD(lParam);
        y2 = HIWORD(lParam);

        xright = LOWORD(lParam);
        ybottom = HIWORD(lParam);

        a = abs(x2 - x1);
        b = abs(y2 - y1);
        radius = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        squareRadius = min(xright, xleft);
        size = abs(x0 - LOWORD(lParam));
        width = abs(x2 - x0);
        height = abs(y2 - y0);

        break;

    case WM_CREATE:
        cout << "Enter the number of points for convex fill: ";
        cin >> convexNum;
        cout << "Enter the quarter of the circle you want to fill: ";
        cin >> quarter;
        addMenu(hwnd);
        // Create the solid brush
        hSolidBrush = CreateSolidBrush(RGB(0, 0, 0));
        // Create the hatched brush
        hHatchedBrush = CreateHatchBrush(HS_DIAGCROSS, RGB(0, 0, 255));
        break;

    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void addMenu(HWND hwnd)
{
    hMenu = CreateMenu();
    HMENU hMenuItem = CreateMenu();
    HMENU hColorMenu = CreateMenu();
    HMENU hBGColorMenu = CreateMenu();

    AppendMenu(hMenu, MF_STRING, LOAD_FILE, "Load");
    AppendMenu(hMenu, MF_STRING, SAVE_SCREEN, "Save");

    AppendMenu(hMenu, MF_STRING, CHANGE_BRUSH, "Brush");


    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hColorMenu, "Color");
    AppendMenu(hColorMenu, MF_STRING, RED_CLR, "Red");
    AppendMenu(hColorMenu, MF_STRING, GREEN_CLR, "Green");
    AppendMenu(hColorMenu, MF_STRING, BLUE_CLR, "Blue");
    AppendMenu(hColorMenu, MF_STRING, PURPLE_CLR, "Purple");
    AppendMenu(hColorMenu, MF_STRING, YELLOW_CLR, "Yellow");
    AppendMenu(hColorMenu, MF_STRING, PINK_CLR, "Pink");
    AppendMenu(hColorMenu, MF_STRING, BLACK_CLR, "Black");

    AppendMenu(hMenu, MF_STRING, MOUSE_CURSOR, "Mouse");


    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hBGColorMenu, "Background Color");
    AppendMenu(hBGColorMenu, MF_STRING, WHITE_BG, "White");




    AppendMenu(hMenu,MF_POPUP, (UINT_PTR) hMenuItem, "Choose Algorithm");
    AppendMenu(hMenuItem, MF_STRING, LINE_DDA, "Line DDA");
    AppendMenu(hMenuItem, MF_STRING, LINE_MIDPOINT, "Line Midpoint");
    AppendMenu(hMenuItem, MF_STRING, LINE_PARAMETRIC, "Line Parametric");

    AppendMenu(hMenuItem, MF_STRING, CIRCLE_DIRECT, "Circle Direct");
    AppendMenu(hMenuItem, MF_STRING, CIRCLE_POLAR, "Circle Polar");
    AppendMenu(hMenuItem, MF_STRING, CIRCLE_ITERATIVE_POLAR, "Circle Iterative Polar");
    AppendMenu(hMenuItem, MF_STRING, CIRCLE_MIDPOINT, "Circle Midpoint");
    AppendMenu(hMenuItem, MF_STRING, CIRCLE_MODIFIED_MIDPOINT, "Circle Modified Midpoint");

    AppendMenu(hMenuItem, MF_STRING, FILL_CIRCLE_WITH_LINES, "Fill Circle with Lines (take filling quarter from user)");
    AppendMenu(hMenuItem, MF_STRING, FILL_CIRCLE_WITH_CIRCLES, "Fill Circle with Other Circles");
    AppendMenu(hMenuItem, MF_STRING, FILL_SQUARE_WITH_HERMITE_CURVE, "Fill Square with Hermite Curve (vertical)");
    AppendMenu(hMenuItem, MF_STRING, FILL_RECTANGLE_WITH_BEZIER_CURVE, "Fill Rectangle with Bezier Curve (horizontal)");

    AppendMenu(hMenuItem, MF_STRING, CONVEX_FILL, "Convex Fill");
    AppendMenu(hMenuItem, MF_STRING, NON_CONVEX_FILL, "Non Convex Fill");
    AppendMenu(hMenuItem, MF_STRING, RECURSIVE_FLOOD_FILL, "Recursive Flood Fill");
    AppendMenu(hMenuItem, MF_STRING, NON_RECURSIVE_FLOOD_FILL, "Non Recursive Flood Fill");
    AppendMenu(hMenuItem, MF_STRING, CARDINAL_SPLINE_CURVE, "Cardinal Spline Curve");

    AppendMenu(hMenuItem, MF_STRING, ELLIPSE_DIRECT, "Ellipse Direct");
    AppendMenu(hMenuItem, MF_STRING, ELLIPSE_POLAR, "Ellipse Polar");
    AppendMenu(hMenuItem, MF_STRING, ELLIPSE_MIDPOINT, "Ellipse Midpoint");


    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_RECTANGLE_LINE, "Clipping using Rectangle (line)");
    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_RECTANGLE_POINT, "Clipping using Rectangle (point)");
    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_RECTANGLE_POLYGON, "Clipping using Rectangle (Polygon)");

    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_SQUARE_LINE, "Clipping using Square (line)");
    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_SQUARE_POINT, "Clipping using Square (point)");

    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_CIRCLE_LINE, "Clipping using Circle (line)");
    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_CIRCLE_POINT, "Clipping using Circle (point)");

    AppendMenu(hMenu, MF_STRING, CLEAR_SCREEN, "Clear Screen");
    AppendMenu(hMenu, MF_STRING, FILE_MENU_EXIT, "Exit");


    SetMenu(hwnd, hMenu);

}



void SaveScreenDataToFile(HWND hwnd, const string& filename)
{
    HDC hdc = GetDC(hwnd);
    RECT rect;
    GetClientRect(hwnd, &rect);
    int widthScreen = rect.right - rect.left;
    int heightScreen = rect.bottom - rect.top;

    // Create a vector to store the screen data
    vector<vector<COLORREF>> screenData(heightScreen,vector<COLORREF>(widthScreen));

    for (int y = 0; y < heightScreen; y++)
    {
        for (int x = 0; x < widthScreen; x++)
        {
            COLORREF color = GetPixel(hdc, x, y);
            screenData[y][x] = color;
        }
    }

    ReleaseDC(hwnd, hdc);

    // Save the screen data to a file
    ofstream file(filename);
    if (file.is_open())
    {
        for (int y = 0; y < heightScreen; y++)
        {
            for (int x = 0; x < widthScreen; x++)
            {
                file << hex << setw(6) <<setfill('0') << screenData[y][x] << " ";
            }
            file << endl;
        }
        file.close();
        MessageBox(hwnd, _T("Screen data saved successfully!"), _T("Success"), MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBox(hwnd, _T("Failed to save screen data to file!"), _T("Error"), MB_OK | MB_ICONERROR);
    }
}

void LoadScreenDataFromFile(HWND hwnd, const string& filename)
{
    HDC hdc = GetDC(hwnd);
    RECT rect;
    GetClientRect(hwnd, &rect);
    int widthScreen = rect.right - rect.left;
    int heightScreen = rect.bottom - rect.top;

    ifstream file(filename);
    if (file.is_open())
    {
        vector<vector<COLORREF>> screenData(heightScreen, vector<COLORREF>(widthScreen));

        for (int y = 0; y < heightScreen; y++)
        {
            for (int x = 0; x < widthScreen; x++)
            {
                if (!(file >> hex >> screenData[y][x]))
                {
                    MessageBox(hwnd, _T("Failed to read screen data from file!"), _T("Error"), MB_OK | MB_ICONERROR);
                    file.close();
                    ReleaseDC(hwnd, hdc);
                    return;
                }
            }
        }

        file.close();

        for (int y = 0; y < heightScreen; y++)
        {
            for (int x = 0; x < widthScreen; x++)
            {
                SetPixel(hdc, x, y, screenData[y][x]);
            }
        }

        MessageBox(hwnd, _T("Screen data loaded successfully!"), _T("Success"), MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBox(hwnd, _T("Failed to open screen data file!"), _T("Error"), MB_OK | MB_ICONERROR);
    }
    ReleaseDC(hwnd, hdc);
}

