#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file"
#endif

#include "resource.h"

struct AppState
{
    double centerX = -0.75;
    double centerY = 0.0;
    double scale = 0.0;
    int maxIter = 50;

    int width = 1600;
    int height = 1200;

    HBITMAP hBitmap = nullptr;
    void* pixels = nullptr;
    int pitch = 0;
    BITMAPINFO bmi{};

    bool needRender = true;
    bool owned = false;

    bool dragging = false;
    POINT dragStart{};
    double dragCenterX = -0.75;
    double dragCenterY = 0.0;

    bool selecting = false;
    bool hasSelection = false;
    POINT selStart{};
    RECT selRect{ 0, 0, 0, 0 };

    int rmin = 100;
    int rmax = 255;
    int gmin = 0;
    int gmax = 255;
    int bmin = 0;
    int bmax = 0;
};

class CMFCMandelbrotApp : public CWinApp
{
public:
    CMFCMandelbrotApp() noexcept;

public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()
};
