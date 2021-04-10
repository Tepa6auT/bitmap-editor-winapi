#define _CRT_SECURE_NO_WARNINGS		// Remove secure warnings 

#include <windows.h>
#include <stdbool.h>

#define ID_BUTTON1 1
#define ID_BUTTON2 2
#define ID_BUTTON3 3
#define ID_BUTTON4 4


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    static char szAppName[] = "HelloWin";
    HWND        hwnd;
    MSG         msg;
    WNDCLASSEX  wndclass;

    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wndclass);

    hwnd = CreateWindow(szAppName,         // window class name
        "The Bitmap Algorightms Program",     // window caption
        WS_OVERLAPPEDWINDOW,     // window style
        CW_USEDEFAULT,           // initial x position
        CW_USEDEFAULT,           // initial y position
        CW_USEDEFAULT,           // initial x size
        CW_USEDEFAULT,           // initial y size
        NULL,                    // parent window handle
        NULL,                    // window menu handle
        hInstance,               // program instance handle
        NULL);		             // creation parameters

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC         hdc;
    PAINTSTRUCT ps;
    RECT        rect;

    HBITMAP   hbmp;
    HDC       hmem;
    BITMAP    bmp;


    static bool mirrorv = false; // changes to tru when button2 is clicked
    static bool mirrorh = false; // changes to tru when button2 is clicked
    static int redov = 0; //redo Vertical mirroring
    static int redoh = 0; //redo Horizontal mirroring
    static bool blur = false;


    switch (iMsg) {
    case WM_CREATE:
        // Initialization
        CreateWindowW( //add button
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"Invert Colors",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            0,         // x position 
            0,         // y position 
            150,       // Button width
            40,        // Button height
            hwnd,     // Parent window
            (HMENU)ID_BUTTON1,       // connected to ID defined it the top
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        CreateWindowW(
            L"BUTTON",
            L"Mirror Vertically",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            0,
            40,
            150,
            40,
            hwnd,
            (HMENU)ID_BUTTON2,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);

        CreateWindowW( //add button
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"Mirror Horizontally",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            0,         // x position 
            80,         // y position 
            150,       // Button width
            40,        // Button height
            hwnd,     // Parent window
            (HMENU)ID_BUTTON3,       // connected to ID defined it the top
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        CreateWindowW( //add button
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"Blur",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            0,         // x position 
            120,         // y position 
            150,       // Button width
            40,        // Button height
            hwnd,     // Parent window
            (HMENU)ID_BUTTON4,       // connected to ID defined it the top
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) { //if user clicked button
        case ID_BUTTON1: //if user pressed the button1 -> inversion
            hdc = GetDC(hwnd);
            GetClientRect(hwnd, &rect);
            rect.left = 150;// to not invert buttons
            InvertRect(hdc, &rect); //adds rectangle that inverts colors &rect for window's parameters
            ReleaseDC(NULL, hdc);
            MessageBox(NULL, "Success!", "Invert Color", MB_OK | MB_ICONINFORMATION); //pop-up window that button is pressed, inversion done
            break;

        case ID_BUTTON2: //if user pressed the button1 mirrors and unmirrors vertically
            redov += 1;
            if (redov == 3) {
                redov = 1;
            }
            InvalidateRect(hwnd, NULL, TRUE);
            mirrorv = true;
            MessageBox(NULL, "Success!", "Mirrored vertically", MB_OK | MB_ICONINFORMATION); //pop-up window that button is pressed, mirroring is done
            break;

        case ID_BUTTON3: //if user pressed the button2 mirrors nad unmirrors horizontaly
            redoh += 1;
            if (redoh == 3) {
                redoh = 1;
            }
            InvalidateRect(hwnd, NULL, TRUE);
            mirrorh = true;
            MessageBox(NULL, "Success!", "Mirrored horizontally", MB_OK | MB_ICONINFORMATION); //pop-up window that button is pressed, mirroring is done
            break;

        case ID_BUTTON4: //if user pressed the button2 mirrors nad unmirrors horizontaly
            InvalidateRect(hwnd, NULL, TRUE);
            blur = true;
            MessageBox(NULL, "Success!", "Mirrored horizontally", MB_OK | MB_ICONINFORMATION); //pop-up window that button is pressed, mirroring is done
            break;

            break;
        }
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps); // Get DC handle
        GetClientRect(hwnd, &rect);  // Get client rectangle
        hmem = CreateCompatibleDC(hdc);
        hbmp = LoadImage(0, "\\image.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); // Load bitmap from resource files 
        SelectObject(hmem, hbmp);
        GetObject(hbmp, sizeof(BITMAP), &bmp);
        StretchBlt(hdc, 150, 0, rect.right - 150, rect.bottom, hmem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); //Destination on x is 100 to not being overlaped by buttons

        if (mirrorv) { //mirror and unmirror vertically
            StretchBlt(hdc, 150, rect.bottom, rect.right - 150, -rect.bottom, hmem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);// mirror vertically
            if (redov == 2) {
                StretchBlt(hdc, 150, 0, rect.right - 150, rect.bottom, hmem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); //Destination on x is 100 to not being overlaped by buttons
            }
        }

        if (mirrorh) { //mirror and unmirror horizontally
            StretchBlt(hdc, rect.right, 0, -rect.right + 150, rect.bottom, hmem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
            if (redoh == 2) {
                StretchBlt(hdc, 150, 0, rect.right - 150, rect.bottom, hmem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); //Destination on x is 100 to not being overlaped by buttons
            }
        }

        if (blur) { //mirror and unmirror horizontally
            for (int i = 150; i < rect.right; i++) { // 0 0 start point
                for (int j = 0; j < rect.bottom; j++) {
                    COLORREF center = GetPixel(hdc, i, j);
                    int red = GetRValue(center);
                    int green = GetGValue(center);
                    int blue = GetBValue(center);


                    if (i - 1 >= 0 && j - 1 >= 0) {//has pixel at left & top = top left corner
                        COLORREF topLeft = GetPixel(hdc, i - 1, j - 1);
                        int redTL = GetRValue(topLeft);
                        int greenTL = GetGValue(topLeft);
                        int blueTL = GetBValue(topLeft);
                        red = (red + redTL) / 2;
                        green = (green + greenTL) / 2;
                        blue = (blue + blueTL) / 2;

                    }
                    if (i - 1 >= 0) {//has pixel at left
                        COLORREF left = GetPixel(hdc, i - 1, j);
                        int redL = GetRValue(left);
                        int greenL = GetGValue(left);
                        int blueL = GetBValue(left);
                        red = (red + redL) / 2;
                        green = (green + greenL) / 2;
                        blue = (blue + blueL) / 2;

                    }
                    if (i - 1 >= 0 && j + 1 < rect.bottom) {//has pixel at left & bottom = bottom left corner
                        COLORREF bottomLeft = GetPixel(hdc, i - 1, j + 1);
                        int redBL = GetRValue(bottomLeft);
                        int greenBL = GetGValue(bottomLeft);
                        int blueBL = GetBValue(bottomLeft);
                        red = (red + redBL) / 2;
                        green = (green + greenBL) / 2;
                        blue = (blue + blueBL) / 2;
                    }
                    if (j - 1 >= 0) {//has pixel at top
                        COLORREF top = GetPixel(hdc, i, j - 1);
                        int redT = GetRValue(top);
                        int greenT = GetGValue(top);
                        int blueT = GetBValue(top);
                        red = (red + redT) / 2;
                        green = (green + greenT) / 2;
                        blue = (blue + blueT) / 2;

                    }
                    if (j + 1 < rect.bottom) { //has pixel at bottom
                        COLORREF bottom = GetPixel(hdc, i, j + 1);
                        int redB = GetRValue(bottom);
                        int greenB = GetGValue(bottom);
                        int blueB = GetBValue(bottom);
                        red = (red + redB) / 2;
                        green = (green + greenB) / 2;
                        blue = (blue + blueB) / 2;

                    }
                    if (i + 1 < rect.right && j - 1 >= 0) {//has pixel at right & top = upper right corner
                        COLORREF topRight = GetPixel(hdc, i + 1, j - 1);
                        int redTR = GetRValue(topRight);
                        int greenTR = GetGValue(topRight);
                        int blueTR = GetBValue(topRight);
                        red = (red + redTR) / 2;
                        green = (green + greenTR) / 2;
                        blue = (blue + blueTR) / 2;
                    }
                    if (i + 1 < rect.right) { //has pixel at right
                        COLORREF right = GetPixel(hdc, i + 1, j);
                        int redR = GetRValue(right);
                        int greenR = GetGValue(right);
                        int blueR = GetBValue(right);
                        red = (red + redR) / 2;
                        green = (green + greenR) / 2;
                        blue = (blue + blueR) / 2;
                    }
                    if (i + 1 < rect.right && j + 1 < rect.bottom) { //has pixel at right & bottom = bottom right corner
                        COLORREF bottomRight = GetPixel(hdc, i + 1, j + 1);
                        int redBR = GetRValue(bottomRight);
                        int greenBR = GetGValue(bottomRight);
                        int blueBR = GetBValue(bottomRight);
                        red = (red + redBR) / 2;
                        green = (green + greenBR) / 2;
                        blue = (blue + blueBR) / 2;
                    }
                    SetPixel(hdc, i, j, RGB(red, green, blue));

                }
            }
        }
        DeleteObject(hbmp);
        DeleteDC(hmem);
        EndPaint(hwnd, &ps);         // Release DC
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);          // Exit from the program
        return 0;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam); // Call the default window procedure
}


