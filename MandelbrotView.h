#pragma once

#include "Properties.h"

class CMandelbrotDoc;

class CMandelbrotView : public CView
{
protected:
    CMandelbrotView() noexcept;
    DECLARE_DYNCREATE(CMandelbrotView)

public:
    CMandelbrotDoc* GetDocument() const;

protected:
    // === Interaction state ===
    bool   m_bDragging = false;   // active mouse drag
    bool   m_bPanning = false;    // left-button pan drag
    double m_aspect = 1.0;        // view aspect ratio
    CPoint m_ptAnchor;            // rectangle anchor point
    CRect  m_rcCapture;           // rectangle bounds

    // === Internal helpers ===
    void UpdateCaptureRect(CPoint pt);

public:
    virtual void OnInitialUpdate() override;
    virtual void OnDraw(CDC* pDC) override;

protected:
    // === Mouse interaction ===
    afx_msg void OnRButtonDown(UINT nFlags, CPoint pt);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint pt);
    afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

    // === Commands ===
    afx_msg void OnProperties();
    afx_msg void OnViewReset();
    afx_msg void OnIterInc();
    afx_msg void OnIterDec();

    // === Resize ===
    afx_msg void OnSize(UINT nType, int cx, int cy);

    DECLARE_MESSAGE_MAP()
};
