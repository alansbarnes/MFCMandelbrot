#include "pch.h"
#include "framework.h"
#include "MFCMandelbrot.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
END_MESSAGE_MAP()

CMainFrame::CMainFrame() noexcept
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CFrameWnd::PreCreateWindow(cs))
        return FALSE;

    cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE |
        WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

    return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Toolbar
    if (!m_wndToolBar.CreateEx(this) ||
        !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
    {
        return -1;
    }

    // Status bar
    if (!m_wndStatusBar.Create(this))
        return -1;

    return 0;
}
