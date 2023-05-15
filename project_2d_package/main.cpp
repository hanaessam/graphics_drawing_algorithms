
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
#include "ids.h"


#define PI 3.14



using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
vector<std::vector<COLORREF>> screenData;

void addMenu(HWND);

HMENU hMenu;

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






/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int x0, y0, size, width, height;
    static int x1, x2, y1, y2, a, b;
    static int radius;
    static int numpoints = 10000;
    static int pointscount = 200;
    static POINT controlPoints[10];
    static int numControlPoints = 0;
    static double tension = 0.5;
    static COLORREF color = RGB(255, 255, 255);
    static int convexNum = 0;
    static int counter = 0;
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
        /*case SAVE_SCREEN:
            GetClientRect(hwnd, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            // Create a vector to store the screen data
            screenData.clear();
            screenData.resize(height, vector<COLORREF>(width));
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    color = GetPixel(hdc, x, y);
                    screenData[y][x] = color;
                }
            }
            ReleaseDC(hwnd, hdc);

            // Save the screen data to a file
            ofstream file("screen_data.txt");
            if (file.is_open())
            {
                for (int y = 0; y < height; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        file << screenData[y][x] << " ";
                    }
                    file << endl;
                }
                file.close();
            }
            else
            {
                MessageBox(hwnd, _T("Failed to save screen data to file!"), _T("Error"), MB_OK | MB_ICONERROR);
            }
            break;

        case LOAD_FILE:
            // Load the screen data from the file
            ifstream file("screen_data.txt");
            if (file.is_open())
            {
                screenData.clear();
                string line;
                while (getline(file, line))
                {
                    vector<COLORREF> row;
                    istringstream iss(line);
                    while (iss >> color)
                    {
                        row.push_back(color);
                    }
                    screenData.push_back(row);
                }
                file.close();

                // Set the loaded screen data on the window
                int width = screenData[0].size();
                int height = screenData.size();
                for (int y = 0; y < height; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        color = screenData[y][x];
                        SetPixel(hdc, x, y, color);
                    }
                }
                ReleaseDC(hwnd, hdc);
            }
            else
            {
                MessageBox(hwnd, _T("Failed to load screen data from file!"), _T("Error"), MB_OK | MB_ICONERROR);
            }
            break;
        */
        case FILE_MENU_EXIT:
            DestroyWindow(hwnd);
            break;
        /* case MOUSE_CURSOR:
             const wchar_t* filePath = L"E:\\Uni\\Year 3 2nd semester\\graphics\\labs\\project_2d_package\\icons8-mouse-50.cur";
             HCURSOR hCursor = (HCURSOR)LoadImage(NULL, (LPCSTR)filePath, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
             if (hCursor == NULL)
             {
                 return 1;
             }

             SetCursor(hCursor);
             ShowCursor(TRUE);
             DestroyCursor(hCursor);
             SetCursor(LoadCursor(NULL, IDC_ARROW));
             break;

        */
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

        case CLIPPING_USING_RECTANGLE:
            Rectangle(hdc,100,300,500,100);
            clipping.CohenSuth(hdc,x1,y1,x2,y2,100,300,500,100);

            break;
        case CLIPPING_USING_RECTANGLE_POLYGON:
            Rectangle(hdc,100,300,500,100);
            clipping.PolygonClip(hdc,shapePoints,convexNum,100,300,500,100);
            counter = 0;
            break;
        case CLIPPING_USING_SQUARE:
            Rectangle(hdc,100,100,400,400);
            clipping.PolygonClip(hdc,shapePoints,3,100,100,400,400);
            counter = 0;
            break;
        case CLIPPING_USING_CIRCLE:
            clipping.CohenSuthCircle(hdc, x1,y1,x2,y2, x1,y1,radius);
            break;
        case CARDINAL_SPLINE_CURVE:
            curve.DrawCardinalSpline(hdc, controlPoints, numControlPoints, tension, numpoints, color);
            numControlPoints = 0;
            break;

        case FILL_CIRCLE_WITH_LINES:
            filling.FillQuarterCircle(hdc, x1, y1, radius, color);
            ReleaseDC(hwnd, hdc);
            break;

        case FILL_CIRCLE_WITH_CIRCLES:
            fillCircleWithConcentricCircles(hdc, x1,y1,radius, 100, color);
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
        break;

    case WM_RBUTTONDOWN:
        x2 = LOWORD(lParam);
        y2 = HIWORD(lParam);
        a = abs(x2 - x1);
        b = abs(y2 - y1);
        radius = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        size = abs(x0 - LOWORD(lParam));
        width = abs(x2 - x0);
        height = abs(y2 - y0);
        break;

    case WM_CREATE:
        cout << "Enter the number of points for convex fill: ";
        cin >> convexNum;
        addMenu(hwnd);
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

    AppendMenu(hMenu, MF_STRING, 1, "Brush");
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


    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_RECTANGLE, "Clipping using Rectangle (point, line)");
    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_RECTANGLE_POLYGON, "Clipping using Rectangle (Polygon)");

    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_SQUARE, "Clipping using Square (point, line)");
    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_CIRCLE, "Clipping using Circle (point, line)");

    AppendMenu(hMenu, MF_STRING, CLEAR_SCREEN, "Clear Screen");
    AppendMenu(hMenu, MF_STRING, FILE_MENU_EXIT, "Exit");


    SetMenu(hwnd, hMenu);

}
