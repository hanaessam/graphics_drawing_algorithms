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


#define FILE_MENU_NEW 1
#define FILE_MENU_EXIT 2
#define CLEAR_SCREEN 3

#define RED_CLR 2001
#define GREEN_CLR 2002
#define BLUE_CLR 2003
#define PURPLE_CLR 2004
#define YELLOW_CLR 2005
#define PINK_CLR 2006
#define BLACK_CLR 2007



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
    static int x1, x2, y1, y2;
    static int radius;

    static COLORREF color = RGB(255, 255, 255);
    HDC hdc = GetDC(hwnd);
    static bool drawing = false;
    HBRUSH hBrush = CreateSolidBrush(RGB(192, 192, 192));
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);

    LineAlgorithm line;
    CircleAlgorithm circle;

    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case CLEAR_SCREEN: // Clear screen menu item
            // Code to clear the screen
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, hBrush);
            ReleaseDC(hwnd, hdc);
            DeleteObject(hBrush);

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


        case 1001:
            // Draw DDA line
            line.drawLineDDA(hdc, x1, y1, x2, y2, color);
            break;

        case 1002:
            // Draw midpoint line
            line.drawLineMidPoint(hdc, x1, y1, x2, y2, color);
            break;

        case 1003:
            // Draw parametric line
            line.drawLineParametric(hdc, x1, y1, x2, y2, color);
            break;

        case 1004:
            circle.drawCircleDirect(hdc, x1,y1,radius, color);
            break;
        case 1005:
            circle.drawCirclePolar(hdc, x1,y1,radius, color);
            break;
        case 1006:
            circle.drawCircleIterativePolar(hdc, x1,y1,radius, color);
            break;
        case 1007:
            circle.drawCircleBresenham(hdc, x1,y1,radius, color);
            break;
        case 1008:
            circle.drawCircleFasterBresenham(hdc, x1,y1,radius, color);
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

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hFileMenu, "Load File");
    AppendMenu(hFileMenu, MF_STRING,FILE_MENU_NEW, "File" );


    AppendMenu(hMenu, MF_STRING, 1, "Save");
    AppendMenu(hMenu, MF_STRING, 1, "Brush");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hColorMenu, "Color");
    AppendMenu(hColorMenu, MF_STRING, 2001, "Red");
    AppendMenu(hColorMenu, MF_STRING, 2002, "Green");
    AppendMenu(hColorMenu, MF_STRING, 2003, "Blue");
    AppendMenu(hColorMenu, MF_STRING, 2004, "Purple");
    AppendMenu(hColorMenu, MF_STRING, 2005, "Yellow");
    AppendMenu(hColorMenu, MF_STRING, 2006, "Pink");
    AppendMenu(hColorMenu, MF_STRING, 2007, "Black");

    AppendMenu(hMenu, MF_STRING, 1, "Mouse");
    AppendMenu(hMenu, MF_STRING, 1, "Background Color");




    AppendMenu(hMenu,MF_POPUP, (UINT_PTR) hMenuItem, "Choose Algorithm");
    AppendMenu(hMenuItem, MF_STRING, 1001, "Line DDA");
    AppendMenu(hMenuItem, MF_STRING, 1002, "Line Midpoint");
    AppendMenu(hMenuItem, MF_STRING, 1003, "Line Parametric");

    AppendMenu(hMenuItem, MF_STRING, 1004, "Circle Direct");
    AppendMenu(hMenuItem, MF_STRING, 1005, "Circle Polar");
    AppendMenu(hMenuItem, MF_STRING, 1006, "Circle Iterative Polar");
    AppendMenu(hMenuItem, MF_STRING, 1007, "Circle Midpoint");
    AppendMenu(hMenuItem, MF_STRING, 1008, "Circle Modified Midpoint");

    AppendMenu(hMenuItem, MF_STRING, 1009, "Fill Circle with Lines (take filling quarter from user)");
    AppendMenu(hMenuItem, MF_STRING, 1010, "Fill Circle with Other Circles");
    AppendMenu(hMenuItem, MF_STRING, 1011, "Fill Square with Hermite Curve (vertical)");
    AppendMenu(hMenuItem, MF_STRING, 1012, "Fill Rectangle with Bezier Curve (horizontal)");
    AppendMenu(hMenuItem, MF_STRING, 1013, "Convex Fill");
    AppendMenu(hMenuItem, MF_STRING, 1014, "Non Convex Fill");
    AppendMenu(hMenuItem, MF_STRING, 1015, "Recursive Flood Fill");
    AppendMenu(hMenuItem, MF_STRING, 1016, "Non Recursive Flood Fill");
    AppendMenu(hMenuItem, MF_STRING, 1017, "Cardinal Spline Curve");
    AppendMenu(hMenuItem, MF_STRING, 1018, "Ellipse Direct");
    AppendMenu(hMenuItem, MF_STRING, 1019, "Ellipse Polar");
    AppendMenu(hMenuItem, MF_STRING, 1020, "Ellipse Midpoint");
    AppendMenu(hMenuItem, MF_STRING, 1021, "Clipping using Rectangle (point, line, polygon)");
    AppendMenu(hMenuItem, MF_STRING, 1022, "Clipping using Square (point, line)");
    AppendMenu(hMenuItem, MF_STRING, 1023, "Clipping using Circle (point, line)");

    AppendMenu(hMenu, MF_STRING, CLEAR_SCREEN, "Clear Screen");
    AppendMenu(hMenu, MF_STRING, FILE_MENU_EXIT, "Exit");


    SetMenu(hwnd, hMenu);

}
