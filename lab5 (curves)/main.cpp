#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <cmath>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

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
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
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


void DrawHermiteCurve(HDC hdc, POINT& P0, POINT& T0, POINT& P1, POINT& T1, int numpoints, COLORREF color)
{
    if (numpoints < 2) return;

    // Calculate the step size
    double dt = 1.0 / (numpoints - 1);

    // Calculate the Hermite curve points and draw them using SetPixel
    for (int i = 0; i < numpoints; i++)
    {
        double t = i * dt;

        // Calculate the blending functions
        double h1 = 2 * pow(t, 3) - 3 * pow(t, 2) + 1;
        double h2 = -2 * pow(t, 3) + 3 * pow(t, 2);
        double h3 = pow(t, 3) - 2 * pow(t, 2) + t;
        double h4 = pow(t, 3) - pow(t, 2);

        // Calculate the x and y coordinates of the curve point
        double x = h1 * P0.x + h2 * P1.x + h3 * T0.x + h4 * T1.x;
        double y = h1 * P0.y + h2 * P1.y + h3 * T0.y + h4 * T1.y;

        // Round the coordinates to integers
        int pixelX = round(x);
        int pixelY = round(y);

        // Set the pixel color using SetPixel
        SetPixel(hdc, pixelX, pixelY, color);
    }
}


void DrawBezierCurve(HDC hdc, POINT& P0, POINT& P1, POINT& P2, POINT& P3, int numpoints, COLORREF color)
{
    // Calculate the control points for the Hermite curve based on the Bezier curve points
    POINT T0 = { P0.x + (P1.x - P0.x) / 3, P0.y + (P1.y - P0.y) / 3 };
    POINT T1 = { P3.x - (P3.x - P2.x) / 3, P3.y - (P3.y - P2.y) / 3 };

    // Draw the Hermite curve using the control points and the specified color
    DrawHermiteCurve(hdc, P0, T0, P3, T1, numpoints, color);
}


void DrawCardinalSpline(HDC hdc, POINT P[], int n, double c, int numpoints, COLORREF color)
{
    double c1 = 1 - c;

    for (int i = 2; i < n - 1; i++)
    {
        // Calculate the control points for the Hermite curve based on the Cardinal spline points
        POINT T0 = { static_cast<int>(c1 * (P[i].x - P[i - 1].x)), static_cast<int>(c1 * (P[i].y - P[i - 1].y)) };
        POINT T1 = { static_cast<int>(c1 * (P[i + 1].x - P[i - 1].x)), static_cast<int>(c1 * (P[i + 1].y - P[i - 1].y)) };

        // Draw the Hermite curve using the control points and the specified color
        DrawHermiteCurve(hdc, P[i - 1], T0, P[i], T1, numpoints, color);
    }
}



/*  This function is called by the Windows function DispatchMessage()  */
// Global variables


LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc;
    static PAINTSTRUCT ps;
    COLORREF color = RGB(0,0,0);
    // Set the number of points on the curve
    static int numpoints =100;
    // Draw the Hermite curve
    POINT P0 = { 100, 100 };
    POINT T0 = { 150, 50 };
    POINT P1 = { 200, 200 };
    POINT T1 = { 250, 100 };
    POINT controlPoints[] =
    {
        { 100, 200 },
        { 200, 100 },
        { 300, 300 },
        { 400, 200 },
        { 500, 400 }
    };
    static int numControlPoints = sizeof(controlPoints) / sizeof(controlPoints[0]);
    // Set the tension parameter for the Cardinal spline
    static double tension = 0.5;


    switch (message)                  /* handle the messages */
    {
    /*
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);

            DrawBezierCurve(hdc, P0, T0, P1, T1, numpoints, color);

            EndPaint(hwnd, &ps);
            break;
    */
    case WM_PAINT:
    {

        hdc = BeginPaint(hwnd, &ps);
        // Call the DrawCardinalSpline function to draw the curve
        DrawCardinalSpline(hdc, controlPoints, numControlPoints, tension, numCurvePoints, curveColor);

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;

    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
