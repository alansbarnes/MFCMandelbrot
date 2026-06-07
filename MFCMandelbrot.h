#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file"
#endif

#include "resource.h"

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
