#include "pch.h"
#include "framework.h"
#include "MFCMandelbrot.h"

#include "MainFrm.h"
#include "MandelbrotView.h"
#include "MandelbrotDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMFCMandelbrotApp, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, &CMFCMandelbrotApp::OnAppAbout)
END_MESSAGE_MAP()

CMFCMandelbrotApp::CMFCMandelbrotApp() noexcept
{
    SetAppID(_T("MFCMandelbrot.AppID.NoVersion"));
}

CMFCMandelbrotApp theApp;

BOOL CMFCMandelbrotApp::InitInstance()
{
    CWinApp::InitInstance();
    EnableTaskbarInteraction(FALSE);

    SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CMandelbrotDoc),
        RUNTIME_CLASS(CMainFrame),
        RUNTIME_CLASS(CMandelbrotView));
    AddDocTemplate(pDocTemplate);

    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();
    return TRUE;
}

int CMFCMandelbrotApp::ExitInstance()
{
    return CWinApp::ExitInstance();
}

void CMFCMandelbrotApp::OnAppAbout()
{
    CDialogEx aboutDlg(IDD_ABOUTBOX);
    aboutDlg.DoModal();
}
