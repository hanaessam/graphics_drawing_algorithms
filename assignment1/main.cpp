#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <cmath>
using namespace std;

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
               1000,                 /* The programs width */
               700,                 /* and height in pixels */
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

void drawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx, sy ;
    if(x1 < x2)
    {
        sx = 1;
    }
    else
    {
        sx =-1;
    }
    if(y1 < y2)
    {
        sy = 1;
    }
    else
    {
        sy =-1;
    }

    int change = dx - dy;

    while (true)
    {
        SetPixel(hdc, x1, y1, color);

        if (x1 == x2 && y1 == y2)
        {
            break;
        }

        int change2 = 2 * change;

        if (change2 > -dy)
        {
            change -= dy;
            x1 += sx;
        }

        if (change2 < dx)
        {
            change += dx;
            y1 += sy;
        }
    }
}

// Utility function
void draw8points(HDC hdc,int xc,int yc, int a, int b,COLORREF color)
{
    SetPixel(hdc, xc+a, yc+b, color);
    SetPixel(hdc, xc-a, yc+b, color);
    SetPixel(hdc, xc-a, yc-b, color);
    SetPixel(hdc, xc+a, yc-b, color);
    SetPixel(hdc, xc+b, yc+a, color);
    SetPixel(hdc, xc-b, yc+a, color);
    SetPixel(hdc, xc-b, yc-a, color);
    SetPixel(hdc, xc+b, yc-a, color);
}

void drawCircle(HDC hdc,int xc,int yc, int r,COLORREF color)
{
    int x=0;
    int y=r;
    int d=1-r;
    draw8points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        if(d<0)
        {
            x++;
            d+=2*x+3;
        }
        else
        {
            x++;
            y--;
            d+=2*(x-y)+5;
        }
        draw8points(hdc,xc,yc,x,y,color);
    }
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    int static x, y, x0,y0, radius;
    double static angle = M_PI / 4.0;
    int static r, g, b;
    COLORREF circleColor;
    switch (message)                  /* handle the messages */
    {
    case WM_LBUTTONDBLCLK:

        x = LOWORD(lParam);
        y = HIWORD(lParam);
        break;

    case WM_RBUTTONDBLCLK:
        x0 = LOWORD(lParam);
        y0 = HIWORD(lParam);
        radius = sqrt(pow(x0 - x, 2) + pow(y0 - y, 2));
        // Redraw the window
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
        break;
    case WM_PAINT:
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);
        //Entering the color
        cout << "Enter the color using RGB format: \n";
        cout << "Enter the red value 0-255: ";
        cin >> r;
        cout << "Enter the green value 0-255: ";
        cin >> g;
        cout << "Enter the blue value 0-255: ";
        cin >> b;
        circleColor = RGB(r, g, b);

        // Draw the circle
        drawCircle(hdc, x,y, radius, circleColor);

        // Divide the circle into 8 parts and draw lines
        for (int i = 0; i < 8; i++)
        {

            // Get the start and end points of each line using the polar method
            int x1 = x + radius * cos(i * angle);
            int y1 = y - radius * sin(i * angle);


            int x2 = x + radius * cos((i + 4) * angle);
            int y2 = y - radius * sin((i + 4) * angle);

            // Draw the line
            drawLine(hdc, x1, y1, x2, y2, circleColor);
        }
        EndPaint(hwnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
