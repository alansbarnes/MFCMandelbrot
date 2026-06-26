#include "pch.h"
#include "MFCMandelbrot.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
    ON_WM_CREATE()
    ON_COMMAND(ID_EXIT, &CMainFrame::OnExit)
    ON_COMMAND(ID_ABOUT, &CMainFrame::OnAbout)
END_MESSAGE_MAP()

CMainFrame::CMainFrame() noexcept
{
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
    SetWindowText(_T("Simple Mandelbrot Explorer"));
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CFrameWndEx::PreCreateWindow(cs))
        return FALSE;

    return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!m_wndMenuBar.Create(this))
        return -1;

    CMenu menu;
    if (!menu.LoadMenu(IDR_MAINFRAME))
        return -1;

    m_wndMenuBar.CreateFromMenu(menu.GetSafeHmenu());

    if (!m_wndStatusBar.Create(this))
        return -1;

    EnableDocking(CBRS_ALIGN_ANY);
    m_wndMenuBar.EnableDocking(CBRS_ALIGN_TOP);
    DockPane(&m_wndMenuBar);
    RecalcLayout();

    return 0;
}

void CMainFrame::OnExit()
{
    SendMessage(WM_CLOSE);
}

void CMainFrame::OnAbout()
{
    CDialogEx dlg(IDD_ABOUTBOX);
    dlg.DoModal();
}
