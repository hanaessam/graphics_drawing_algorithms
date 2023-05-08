
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
#include "linealgorithm.h"
#include "circlealgorithm.h"
#include "ellipseAlgorithm.h"
#include "clippingalgorithm.h"
#include "ids.h"






using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");


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
    static int x1, x2, y1, y2, a, b ;
    static int radius;

    static COLORREF color = RGB(255, 255, 255);
    HDC hdc = GetDC(hwnd);

    HBRUSH hBrush;
    HBRUSH hOldBrush;


    LineAlgorithm line;
    CircleAlgorithm circle;
    EllipseAlgorithm ellipse;
    ClippingAlgorithm clipping;

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

        case FILE_MENU_EXIT:
            DestroyWindow(hwnd);
            break;

        case BLACK_CLR:
            color = RGB(0,0,0);
            break;
        case RED_CLR:
            color = RGB(255,0,0);
            break;
        case GREEN_CLR:
            color = RGB(0,255,0);
            break;
        case BLUE_CLR:
            color = RGB(0,0,255);
            break;
        case PURPLE_CLR:
            color = RGB(255,0,255);
            break;
        case YELLOW_CLR:
            color = RGB(255,255,0);
            break;
        case PINK_CLR:
            color = RGB(255,192,203);
            break;

        case WHITE_BG:
            hBrush = CreateSolidBrush(RGB(255, 255, 255));
            SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);
            DeleteObject(hBrush);
            InvalidateRect(hwnd, NULL, TRUE);
            break;


        case LINE_DDA:
            // Draw DDA line
            line.drawLineDDA(hdc, x1, y1, x2, y2, color);
            break;

        case LINE_MIDPOINT:
            // Draw midpoint line
            line.drawLineMidPoint(hdc, x1, y1, x2, y2, color);
            break;

        case LINE_PARAMETRIC:
            // Draw parametric line
            line.drawLineParametric(hdc, x1, y1, x2, y2, color);
            break;

        case CIRCLE_DIRECT:
            // Draw Circle Direct
            circle.drawCircleDirect(hdc, x1,y1,radius, color);
            break;
        case CIRCLE_POLAR:
            //Draw Circle polar
            circle.drawCirclePolar(hdc, x1,y1,radius, color);
            break;
        case CIRCLE_ITERATIVE_POLAR:
            //Draw circle iterative polar
            circle.drawCircleIterativePolar(hdc, x1,y1,radius, color);
            break;
        case CIRCLE_MIDPOINT:
            // Draw circle midpoint
            circle.drawCircleBresenham(hdc, x1,y1,radius, color);
            break;
        case CIRCLE_MODIFIED_MIDPOINT:
            // Draw circle modified midpoint
            circle.drawCircleFasterBresenham(hdc, x1,y1,radius, color);
            break;

        case ELLIPSE_DIRECT:
            // Draw ellipse direct
            ellipse.drawEllipseDirect(hdc, x1,y1,a,b, color);
            break;
        case ELLIPSE_POLAR:
            // Draw ellipse polar
            ellipse.drawEllipsePolar(hdc, x1,y1,a,b, color);
            break;
        case ELLIPSE_MIDPOINT:
            // Draw ellipse midpoint
            ellipse.drawEllipseBresenham(hdc, x1,y1,a,b, color);
            break;

        case CLIPPING_USING_RECTANGLE:
            Rectangle(hdc,100,300,500,100);
            clipping.CohenRect(hdc,x1,y1,x2,y2,100,300,500,100);
            break;



        }
        break;

    case WM_LBUTTONDOWN:
        x1 = LOWORD(lParam);
        y1 = HIWORD(lParam);
        break;

    case WM_RBUTTONDOWN:
        x2 = LOWORD(lParam);
        y2 = HIWORD(lParam);
        a = abs(x2 - x1);
        b = abs(y2 - y1);
        radius = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        break;




    case WM_CREATE:
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
    HMENU hFileMenu = CreateMenu();
    HMENU hMenuItem = CreateMenu();
    HMENU hColorMenu = CreateMenu();
    HMENU hBGColorMenu = CreateMenu();

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hFileMenu, "Load File");
    AppendMenu(hFileMenu, MF_STRING,FILE_MENU_NEW, "File" );


    AppendMenu(hMenu, MF_STRING, 1, "Save");
    AppendMenu(hMenu, MF_STRING, 1, "Brush");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hColorMenu, "Color");
    AppendMenu(hColorMenu, MF_STRING, RED_CLR, "Red");
    AppendMenu(hColorMenu, MF_STRING, GREEN_CLR, "Green");
    AppendMenu(hColorMenu, MF_STRING, BLUE_CLR, "Blue");
    AppendMenu(hColorMenu, MF_STRING, PURPLE_CLR, "Purple");
    AppendMenu(hColorMenu, MF_STRING, YELLOW_CLR, "Yellow");
    AppendMenu(hColorMenu, MF_STRING, PINK_CLR, "Pink");
    AppendMenu(hColorMenu, MF_STRING, BLACK_CLR, "Black");

    AppendMenu(hMenu, MF_STRING, 1, "Mouse");
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


    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_RECTANGLE, "Clipping using Rectangle (point, line, polygon)");
    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_SQUARE, "Clipping using Square (point, line)");
    AppendMenu(hMenuItem, MF_STRING, CLIPPING_USING_CIRCLE, "Clipping using Circle (point, line)");

    AppendMenu(hMenu, MF_STRING, CLEAR_SCREEN, "Clear Screen");
    AppendMenu(hMenu, MF_STRING, FILE_MENU_EXIT, "Exit");


    SetMenu(hwnd, hMenu);

}
