//Author: Rick Armbruster

#include <iostream>
#include <windows.h>
#include <atlimage.h> 
#include <Gdiplusimaging.h> 

HBITMAP ImageFromClipboard(HWND hWnd)
{
    if (!OpenClipboard(hWnd))
        return NULL;

    HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
    CloseClipboard();
    return hBitmap;
}

void screenshot(POINT a, POINT b)
{
    // copy screen to bitmap
    HDC     hScreen = GetDC(NULL);
    HDC     hDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, abs(b.x - a.x), abs(b.y - a.y));
    HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
    BOOL    bRet = BitBlt(hDC, 0, 0, abs(b.x - a.x), abs(b.y - a.y), hScreen, a.x, a.y, SRCCOPY);

    // save bitmap to clipboard
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hBitmap);
    CloseClipboard();

    // clean up
    SelectObject(hDC, old_obj);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);
    DeleteObject(hBitmap);
}

int SaveImageFromClippboard() {
    HWND hWnd = GetDesktopWindow();
    HBITMAP hBitmap = ImageFromClipboard(hWnd);
    if (hBitmap == NULL)
        return 0;
    CImage image;
    image.Attach(hBitmap);
    image.Save(_T("test.bmp"), Gdiplus::ImageFormatBMP);

    return 1;
}

int main()
{
    POINT a, b;
    a.x = 0;
    a.y = 0;

    b.x = 500;
    b.y = 500;

    screenshot(a, b);
    SaveImageFromClippboard();

    return 0;
}