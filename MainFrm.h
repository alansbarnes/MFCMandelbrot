#pragma once

class CMainFrame : public CFrameWnd
{
protected:
    CMainFrame() noexcept;
    DECLARE_DYNCREATE(CMainFrame)

public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

protected:
    CStatusBar m_wndStatusBar;
    CToolBar   m_wndToolBar;

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()
};
