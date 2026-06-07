#pragma once

#include "afxframewndex.h"

class CMainFrame : public CFrameWndEx
{
protected:
    CMainFrame() noexcept;
    DECLARE_DYNCREATE(CMainFrame)

public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

protected:
    CMFCMenuBar     m_wndMenuBar;
    CMFCToolBar     m_wndToolBar;
    CMFCStatusBar   m_wndStatusBar;

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnExit();
    afx_msg void OnAbout();
    DECLARE_MESSAGE_MAP()
};
