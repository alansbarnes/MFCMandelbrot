#pragma once

#include "Properties.h"

class CMandelbrotView : public CView
{
protected:
    CMandelbrotView() noexcept;
    double m_aspect = 1.0;
    bool m_bDragging = false;
    CPoint m_ptAnchor;
    CRect m_rcCapture;

    void UpdateCaptureRect(CPoint pt);
    DECLARE_DYNCREATE(CMandelbrotView)

public:
    virtual void OnInitialUpdate() override;
    virtual void OnDraw(CDC* pDC) override;

    void OnLButtonDown(UINT, CPoint pt);
    void OnRButtonDown(UINT, CPoint pt);
    void OnMouseMove(UINT, CPoint pt);
    void OnRButtonUp(UINT, CPoint pt);

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnProperties();
    afx_msg void OnViewReset();
    afx_msg void OnIterInc();
    afx_msg void OnIterDec();
};
